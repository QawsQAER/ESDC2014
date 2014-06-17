/*
 * Copyright (C) Your copyright notice.
 *
 * Author: Edward HUANG@CUHK huangxx_2155@live.com
 *         Tony Yi@CUHK     
 *         Terry Lai@CUHK
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the PG_ORGANIZATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY	THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS-IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "Motion_controller.h"

Motion_controller::Motion_controller()
{
	printf("Motion_controller(): Constructing new Motion_controller\n");

	this->lifter_pos = 0;
	//initialize threshold value
	this->threshold_x = IMG_HORI_THRESHOLD;
	this->threshold_y = IMG_VERT_THRESHOLD;
	printf("Motion_controller(): Threshold_x %d Threshold_y %d\n",this->threshold_x,this->threshold_y);
	this->center_x = IMG_CENTER_X;
	this->center_y = IMG_CENTER_Y;
	this->img_exp_pos_x = IMG_EXP_POS1_X;
	this->img_exp_pos_y = IMG_EXP_POS1_Y;
	this->exp_width = IMG_EXP_WIDTH;
	this->exp_height = IMG_EXP_HEIGHT;

	this->exp_face_height = IMG_EXP_FACE_HEIGHT;
	this->exp_face_width = IMG_EXP_FACE_WIDTH;
	this->threshold_face_x = IMG_HORI_THRESHOLD_FACE;
	this->threshold_face_y = IMG_VERT_THRESHOLD_FACE;
	this->img_exp_face_pos_x = IMG_EXP_FACE_POS_X;
	this->img_exp_face_pos_y = IMG_EXP_FACE_POS_Y;
	this->half = 0;
	this->Com = new Controller_Com("/dev/ttyUSB0");
	//TODO: initilized the reference rectangle

	this->ref = cv::Rect(
		this->img_exp_pos_x - this->exp_width/2,
		this->img_exp_pos_y,
		this->exp_width,
		this->exp_height);
	
	this->face_ref = cv::Rect(
		this->img_exp_face_pos_x - this->exp_face_width/2,
		this->img_exp_pos_y,
		this->exp_face_width,
		this->exp_face_height);
}

Motion_controller::~Motion_controller()
{
	this->set_lifter(LIFTER_MIN);
	delete this->Com;
}

uint8_t Motion_controller::init()
{
	printf("Motion_controller::init() running\n");
	Message msg;

	msg.CameraPlatformYawClk(30);
	msg.sendMessage(this->Com->fd);
	sleep(1);
	msg.CameraPlatformYawCounterClk(60);
	msg.sendMessage(this->Com->fd);
	sleep(1);
	msg.CameraPlatformYawClk(30);
	msg.sendMessage(this->Com->fd);
	sleep(1);
	this->reset_lifter();
	printf("Motion_controller::init() returning\n");
	return 1;
}


uint8_t Motion_controller::evaluate_image(const cv::Rect &detect,const cv::Rect &face,const double &distance)
{

/*
	detect: the detected body region
	face: the detected face region
	threshold_x, threshold_y: the threshold for largest horizontally and vertically
	center_x, center_y: the center of the image
	img_exp_pos_x, img_exp_pos_y: the expected (top left) position  of the detected region
	exp_width, exp_height: the expected width and height
	diff_x: the difference between the detected region center and the image center -> cause centering
	diff_y: the difference between the actual detected region height and the expected region height -> cause backward and forward
*/
	
	//find out the center of the detected region
	cv::Point center(detect.x + detect.width / 2,detect.y + detect.height / 2);
	int32_t diff_x = center.x - this->center_x;
	int32_t diff_y = detect.height - this->exp_height;
	
	printf("Motion_controller::evaluate_image center.x %d center_x %d\n",center.x,this->center_x);
	printf("Motion_controller::evaluate_image detect.height %d, exp_height%d\n",detect.height,this->exp_height);
	printf("Motion_controller::evaluate_image the distance is %lf\n",distance);
	printf("Motion_controller::evaluate_image the final exp pos_x is %u\n",this->img_exp_pos_x);
	if(!(*this->half))
	{
		if(abs(diff_x) > threshold_x)//need to adjust horizontally to the center
		{
			this->centering(detect);
			return 0;
		}	
		else if(abs(diff_y) > threshold_y)//the body is too small or too large need to zoom in or zoom out
		{
			this->zoom_in_out(detect,distance);
			return 0;
		}
		else if(abs(center.x - img_exp_pos_x) > threshold_x || abs(detect.y - img_exp_pos_y) > threshold_y)
		{
			this->adjusting(detect);
			return 1;
		}
	}
	else
	{
		diff_y = face.height - this->exp_face_height;

		if(abs(diff_x) > threshold_x)
		{
			this->centering(detect);
			return 0;
		}
		else if(abs(diff_y) > this->threshold_face_y )//the face is too small or too large, need to zoom in or zoom out
		{
			this->zoom_in_out_by_face(face,distance);
			return 0;
		}
		else if(true)
		{
			this->adjusting_by_face(face);
			return 1;
		}
	}
	return 1;
}

/*CENTERING FUNCTION BEGIN*/
/*CENTERING FUNCTION BEGIN*/
/*CENTERING FUNCTION BEGIN*/
uint8_t Motion_controller::centering(const cv::Rect &detect)
{
	printf("\nMotion_controller::centering() running\n");
	uint8_t okay_image = 1;
	uint16_t exp_center_x = IMG_CENTER_X, exp_center_y = IMG_CENTER_Y;

	cv::Point center(detect.x + detect.width / 2,detect.y + detect.height / 2);

	int32_t diff_x = center.x - exp_center_x;
	int32_t diff_y = center.y - exp_center_y;
	//compute length per pixel 
	float p = (float) 1700 / (float) detect.height;
	printf("\n\nMotion_controller::centering() length per pixel is %f.\n\n",p);
	uint16_t move_x = 0;
	if(abs(diff_x) > threshold_x)
	{
		okay_image = 0;
		//movement to right or left
		printf("Motion_controller centering(): diff_x is %d\n",diff_x);
		if(diff_x < 0)
		{	
			//should move left
			move_x = this->bound_dis(ceil(abs(diff_x) * p));
			printf("\n\n\nMotion_controller centering(): moving left %d mm\n\n\n",move_x);
			this->move(move_x,2);
		}
		else
		{
			//should move right
			move_x = this->bound_dis(ceil(abs(diff_x) * p));
			printf("\n\n\nMotion_controller centering(): moving right %d mm\n\n\n",move_x);
			this->move(move_x,3);
		}
	}
	return okay_image;
}

uint8_t Motion_controller::centering_by_face(const cv::Rect &face)
{
	float p = (float) IMG_FACE_ACTUAL_HEIGHT / (float) face.height;// mm per pixel
	int32_t diff_x = (face.x - face.width / 2) - this->center_x;
	uint16_t move_x = abs(ceil(p * diff_x)); 
	printf("Motion_controller::centering_by_face(): diff_x is %d\n",diff_x);
	printf("Motion_controller::centering_by_face(): mm per pixel is %f\n",p);
	if(diff_x > 0)
	{
		//the face is on the right hand side
		//move right
		printf("Motion_controller::centering_by_face(): moving right by %u mm\n",move_x);
		this->move(move_x,3);
	}
	else
	{
		//the face is on the left hand side
		//move left
		printf("Motion_controller::centering_by_face() : moving left by %u mm\n",move_x);
		this->move(move_x,2);
	}
	return 1;
}
/*CENTERING FUNCTION END*/
/*CENTERING FUNCTION END*/
/*CENTERING FUNCTION END*/

uint16_t Motion_controller::bound_dis(const uint32_t &dis)
{
	if(dis > 0x0000ffff)
		return 0xffff;
	else
		return (uint16_t) dis;
}

/*ZOOM IN OUT FUNCTION IMPLEMENTATION START*/
/*ZOOM IN OUT FUNCTION IMPLEMENTATION START*/
/*ZOOM IN OUT FUNCTION IMPLEMENTATION START*/

uint8_t Motion_controller::zoom_in_out(const cv::Rect &detect,const double &distance)
{
	printf("\nMotion_controller::zoom_in_out() running\n");
	printf("Motion_controller::zoom_in_out() the target distance is %lf\n",distance);

	this->zoom_in_out_by_default(detect,distance);
	//this->zoom_in_out_by_distance(detect,distance);
	return 1;
}

void Motion_controller::zoom_in_out_by_default(const cv::Rect &detect,const double &distance)
{
	//the car need to adjust the position according to the detection result
	int32_t diff_y = detect.height - this->exp_height;
	if(diff_y < 0)
	{
		//the height too small, need to zoom in
		//to zoom in, move forward
		printf("Motion_controller::zoom_in_out() moving forward\n");
		this->move(DEFAULT_DIS,0);
	}
	else
	{
		//the height too large, need to zoom out
		//to zoom out, move backward
		printf("Motion_controller::zoom_in_out() moving backward\n");
		this->move(DEFAULT_DIS,1);
	}
}

void Motion_controller::zoom_in_out_by_distance(const cv::Rect &detect,const double &distance)
{
	printf("Motion_controller::zoom_in_out() the target distance is %lf\n",distance);

	double img_exp_dis = IMG_EXP_DIS;
	printf("Motion_controller::zoom_in_out() the img_exp_dis is %lf\n",img_exp_dis);
	printf("Motion_controller::zoom_in_out() the diff is %lf\n",distance - img_exp_dis);
	printf("Motion_controller::zoom_in_out() the abs diff is %lf\n",abs(distance - img_exp_dis));
	printf("Motion_controller::zoom_in_out() the ceil abs diff is %ud\n",ceil(abs(distance - img_exp_dis)));
	if(distance > img_exp_dis)
	{
		//the target too far away from the camera
		uint16_t move_z = ceil(abs(distance - img_exp_dis));
		printf("Motion_controller::zoom_in_out() moving forward %lf\n",move_z);
		this->move(move_z,0);
	}
	else
	{
		//the target too close to the camera
		uint16_t move_z = ceil(abs(distance - img_exp_dis));
		printf("Motion_controller::zoom_in_out() moving backward %lf\n",move_z);
		this->move(move_z,1);
	}
}

void Motion_controller::zoom_in_out_by_face(const cv::Rect &face,const double &distance)
{
	int32_t diff_y = face.height - this->exp_face_height;
	printf("Motion_controller::zoom_in_out_by_face() running\n");
	if(diff_y > 0)
	{
		//the face is too large
		printf("Motion_controller::zoom_in_out_by_face: moving backward, face detect size(%d,%d)\n",face.width,face.height);
		this->move(DEFAULT_DIS,1);
	}
	else
	{
		//the face is too small
		printf("Motion_controller::zoom_in_out_by_face: moving forward, face detect size(%d,%d)\n",face.width,face.height);
		this->move(DEFAULT_DIS,0);
	}
	printf("Motion_controller::zoom_in_out_by_face exiting\n");
	return ;
}
/*ZOOM IN OUT FUNCTION IMPLEMENTATION END*/
/*ZOOM IN OUT FUNCTION IMPLEMENTATION END*/
/*ZOOM IN OUT FUNCTION IMPLEMENTATION END*/

/*ADJUSTING FUNCTION IMPLEMENTATION START*/
/*ADJUSTING FUNCTION IMPLEMENTATION START*/
/*ADJUSTING FUNCTION IMPLEMENTATION START*/
uint8_t Motion_controller::adjusting(const cv::Rect &detect)
{
/*
	diff_x is the difference between the detected body region center's x coordinate, and the expected position's x coordinate
	diff_y is the difference between the detected body region's top left y coordinate, and the expected position's y coordinate
	p is the length per pixel, assuming that every detected region's height is 1700 mm 
*/
	printf("Motion_controller::adjusting() running\n");
	int diff_x = (detect.x + detect.width/2) - img_exp_pos_x;
	float p = (float) 1700 / (float) detect.height;
	if(diff_x > 0)
	{
		//move right
		uint16_t move_x = this->bound_dis(ceil(abs(diff_x) * p));
		printf("\n\n\nMotion_controller adjusting(): moving right %d mm\n\n\n",move_x);
		this->move(move_x,3);
	}
	else
	{
		//move left
		uint16_t move_x = this->bound_dis(ceil(abs(diff_x) * p));
		printf("\n\n\nMotion_controller adjusting(): moving left %d mm\n\n\n",move_x);
		this->move(move_x,2);
	}

	int diff_y = detect.y - img_exp_pos_y;
	printf("Motion_controller adjusting(): detect.y is %d\n",detect.y);
	printf("Motion_controller adjusting(): img_exp_pos_y is %d\n",img_exp_pos_y);
	uint16_t move_y = ceil(abs(diff_y) * p);
	printf("Motion_controller adjusting(): length per pixel is %f, diff_y is %d\n",p,diff_y);
	if(diff_y > 0)
	{
		//moving down
		printf("\n\n\nMotion_controller adjusting(): moving down %d mm\n\n\n",move_y);
		this->lift(move_y,LIFTER_DOWN);
	}
	else
	{
		//moving up
		printf("\n\n\nMotion_controller adjusting(): moving up %d mm\n\n\n",move_y);
		this->lift(move_y,LIFTER_UP);
	}
	return 1;
}

uint8_t Motion_controller::adjusting_by_face(const cv::Rect &face)
{
	cv::Point face_top(face.x + face.width / 2, face.y);

	//move horizontally
	int32_t diff_x = face_top.x - this->img_exp_face_pos_x;
	//move vertically
	int32_t diff_y = face_top.y - this->img_exp_face_pos_y;
	return 1;
}
/*ADJUSTING FUNCTION IMPLEMENTATION END*/
/*ADJUSTING FUNCTION IMPLEMENTATION END*/
/*ADJUSTING FUNCTION IMPLEMENTATION END*/

void Motion_controller::set_pattern(uint8_t pattern)
{
	printf("Motion_controller::set_pattern() running\n");
	printf("Motion_controller::set_pattern() pattern is %u\n",pattern);
	switch(pattern)
	{
		case(1):
			this->img_exp_pos_x = IMG_EXP_POS1_X;
			this->img_exp_pos_y = IMG_EXP_POS1_Y;
		break;
		case(2):
			this->img_exp_pos_x = IMG_EXP_POS2_X;
			this->img_exp_pos_y = IMG_EXP_POS2_Y;
		break;
		case(3):
			this->img_exp_pos_x = IMG_EXP_POS3_X;
			this->img_exp_pos_y = IMG_EXP_POS3_Y;
		break;
		case(4):
			this->img_exp_pos_x = IMG_EXP_POS4_X;
			this->img_exp_pos_y = IMG_EXP_POS4_Y;
		break;
		default:
			printf("Motion_controller::set_pattern() pattern undefined\n");
		break;
	}
	printf("Motion_controller::set_pattern() the img_exp_pos_x is %u, img_exp_pos_y is %u\n",this->img_exp_pos_x,this->img_exp_pos_y);
	printf("Motion_controller::set_pattern() exiting\n");
	this->ref = cv::Rect(this->img_exp_pos_x - this->exp_width / 2,this->img_exp_pos_y,this->exp_width,this->exp_height);
	return ;
}


/*MOVEMENT FUNCTION START*/
/*MOVEMENT FUNCTION START*/
/*MOVEMENT FUNCTION START*/

void Motion_controller::reset_lifter()
{
	printf("Motion_controller::reset_lifter() running\n");
	Message msg;
	msg.LifterMoveDownMM(LIFTER_MAX);
	msg.sendMessage(this->Com->fd);
	
	msg.LifterMoveUpMM(LIFTER_INIT_POS);
	msg.sendMessage(this->Com->fd);
	this->lifter_pos = LIFTER_INIT_POS;
	printf("Motion_controller::reset_lifter() exiting\n");
}

void Motion_controller::lift(const uint16_t &mm, const uint8_t &dir)
{
	Message msg;
	printf("Motion_controller::lift current at %u\n",this->lifter_pos);
	if(dir == LIFTER_UP)
	{
		if(this->lifter_pos + mm > LIFTER_MAX)
			this->lifter_pos = LIFTER_MAX;
		else
			this->lifter_pos += mm;
		printf("Motion_controller::lift moving up to %u\n",this->lifter_pos);
		msg.LifterMoveUpMM(mm);
	}
	else if(dir == LIFTER_DOWN)
	{
		if(this->lifter_pos - mm < LIFTER_MIN)
			this->lifter_pos = LIFTER_MIN;
		else
			this->lifter_pos -= mm;
		printf("Motion_controller::lift down up to %u\n",this->lifter_pos);
		msg.LifterMoveDownMM(mm);
	}
	msg.sendMessage(this->Com->fd);

}

void Motion_controller::set_lifter(const uint16_t &mm)
{
	if(mm > this->lifter_pos)
	{
		this->lift(mm - this->lifter_pos,LIFTER_UP);
	}
	else
	{
		this->lift(this->lifter_pos - mm,LIFTER_DOWN);
	}
}

void Motion_controller::move(const uint16_t &mm,const uint8_t &dir)
{
	Message msg;
	switch(dir)
	{
		case 0://forward
			msg.CarMoveUpMM(mm);
			msg.sendMessage(this->Com->fd);
		break;
			
		case 1://backward
			msg.CarMoveDownMM(mm);
			msg.sendMessage(this->Com->fd);
		break;
		case 2://left
			msg.CarMoveLeftMM(mm);
			msg.sendMessage(this->Com->fd);
		break;
		case 3://right
			msg.CarMoveRightMM(mm);
			msg.sendMessage(this->Com->fd);
		break;
		default:
		break;
	}
	return ;
}

void Motion_controller::rotate(const uint16_t &degree,const uint8_t &dir)
{
	Message msg;
	if(dir == 0)
	{
		msg.CarRotateRightDegree(degree);
	}
	else
	{
		msg.CarRotateLeftDegree(degree);
	}
	msg.sendMessage(this->Com->fd);
	return ;
}

/*MOVEMENT FUNCTION END*/
/*MOVEMENT FUNCTION END*/
/*MOVEMENT FUNCTION END*/