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

	//initialize threshold value
	this->threshold_x = IMG_HORI_THRESHOLD;
	this->threshold_y = IMG_VERT_THRESHOLD;
	printf("Motion_controller(): Threshold_x %d Threshold_y %d\n",this->threshold_x,this->threshold_y);
	this->center_x = IMG_CENTER_X;
	this->center_y = IMG_CENTER_Y;
	this->exp_x = 320;
	this->exp_y = 240;
	this->exp_width = IMG_EXP_WIDTH;
	this->exp_height = IMG_EXP_HEIGHT;
	
	this->Com = new Controller_Com("/dev/ttyUSB0");
	//TODO: initilized the reference rectangle
}

Motion_controller::~Motion_controller()
{
	Message msg;
	msg.LifterMoveDownMM(1000);
	msg.sendMessage(this->Com->fd);
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

	msg.LifterMoveDownMM(1000);
	msg.sendMessage(this->Com->fd);
	
	printf("Motion_controller::init() returning\n");
	return 1;
}


uint8_t Motion_controller::evaluate_image(const cv::Rect &detect,const cv::Rect &ref,const double &distance)
{
/*
	threshold_x, threshold_y: the threshold for largest horizontally and vertically
	center_x, center y: the center of the image
	exp_x, exp_y: the expected position of the detected region
	exp_width, exp_height: the expected width and height
	diff_x: the difference between the detected region center and the image center -> cause centering
	diff_y: the difference between the actual detected region height and the expected region height -> cause backward and forward
*/
	
	//find out the center of the detected region
	cv::Point center(detect.x + detect.width / 2,detect.y + detect.height / 2);
	printf("Motion_controller::evaluate_image center.x %d center_x %d\n",center.x,this->center_x);
	printf("Motion_controller::evaluate_image detect.height %d, exp_height%d\n",detect.height,this->exp_height);
	int diff_x = center.x - this->center_x;
	int diff_y = detect.height - this->exp_height;
	printf("Motion_controller::evaluate_image the distance is %lf\n",distance);
	if(abs(diff_x) > threshold_x)//need to adjust horizontally to the center
	{
		this->centering(detect);
		return 0;
	}	
	else if(abs(diff_y) > threshold_y)//need to zoom in or zoom out
	{
		this->zoom_in_out(detect,distance);
		//return 1;// only run once zoom in out.
		return 0;
	}
	else if(abs(center.x - IMG_EXP_POS1_X) > threshold_x)
	{
		this->adjusting(detect);
		return 1;
	}
	return 1;
}

uint8_t Motion_controller::centering(const cv::Rect &detect)
{
	printf("\nMotion_controller::centering() running\n");
	uint8_t okay_image = 1;
	uint16_t exp_center_x = IMG_CENTER_X, exp_center_y = IMG_CENTER_Y;

	cv::Point center(detect.x + detect.width / 2,detect.y + detect.height / 2);

	int diff_x = center.x - exp_center_x;
	int diff_y = center.y - exp_center_y;
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
			Message msg;
			msg.CarMoveLeftMM(move_x);
			msg.sendMessage(this->Com->fd);
		}
		else
		{
			//should move right
			move_x = this->bound_dis(ceil(abs(diff_x) * p));
			printf("\n\n\nMotion_controller centering(): moving right %d mm\n\n\n",move_x);
			Message msg;
			msg.CarMoveRightMM(move_x);
			msg.sendMessage(this->Com->fd);
		}
	}

	return okay_image;
}

uint8_t Motion_controller::zoom_in_out(const cv::Rect &detect,const double &distance)
{
	printf("\nMotion_controller::zoom_in_out() running\n");
	printf("Motion_controller::zoom_in_out() the target distance is %lf\n",distance);
	printf("Motion_controller::zoom_in_out() the target distance is %lf\n",distance);
	printf("Motion_controller::zoom_in_out() the target distance is %lf\n",distance);
	printf("Motion_controller::zoom_in_out() the target distance is %lf\n",distance);
	printf("Motion_controller::zoom_in_out() the target distance is %lf\n",distance);

	if(distance > IMG_EXP_DIS)
	{
		//the target too far away from the camera
		uint16_t move_z = abs(ceil(distance - IMG_EXP_DIS));
		printf("Motion_controller::zoom_in_out() moving forward %lf\n",move_z);
		Message msg;
		msg.CarMoveUpMM(move_z);
		msg.sendMessage(this->Com->fd);
	}
	else
	{
		//the target too close to the camera
		uint16_t move_z = abs(ceil(distance - IMG_EXP_DIS));
		printf("Motion_controller::zoom_in_out() moving backward %lf\n",move_z);
		Message msg;
		msg.CarMoveDownMM(move_z);
		msg.sendMessage(this->Com->fd);
	}

	/*
	// the car now will adjust its position so it's only 1 meter from the target
	if(distance < 0)
	{
		printf("Motion_controller::zoom_in_out ERROR: invalid distance %lf\n",distance);
	}
	else if(distance > 1000) //1000mm away from the target, move closer
	{
		printf("Motion_controller::zoom_in_out() moving forward %lf\n",distance - 1000);
		Message msg;
		msg.CarMoveUpMM((uint16_t) distance - 1000);
		msg.sendMessage(this->Com->fd);
	}
	else
	{
		printf("Motion_controller::zoom_in_out() moving backward\n");
		Message msg;
		msg.CarMoveDownMM((uint16_t) 1000 - distance);
		msg.sendMessage(this->Com->fd);
	}
	*/
	
	/*
	//the car need to adjust the position according to the detection result
	int diff_y = detect.height - this->exp_height;
	if(diff_y < 0)
	{
		//the height too small, need to zoom in
		//to zoom in, move forward
		printf("Motion_controller::zoom_in_out() moving forward\n");
		Message msg;
		msg.CarMoveUpMM(DEFAULT_DIS);
		msg.sendMessage(this->Com->fd);
	}
	else
	{
		//the height too large, need to zoom out
		//to zoom out, move backward
		printf("Motion_controller::zoom_in_out() moving backward\n");
		Message msg;
		msg.CarMoveDownMM(DEFAULT_DIS);
		msg.sendMessage(this->Com->fd);
	}
	*/
	return 1;
}

uint8_t Motion_controller::adjusting(const cv::Rect &detect)
{
	printf("Motion_controller::adjusting() running\n");
	cv::Point center(detect.x + detect.width / 2,detect.y + detect.height / 2);
	int diff_x = center.x - IMG_EXP_POS1_X;
	float p = (float) 1700 / (float) detect.height;
	if(diff_x > 0)
	{
		//move right
		uint16_t move_x = this->bound_dis(ceil(abs(diff_x) * p));
		printf("\n\n\nMotion_controller adjusting(): moving right %d mm\n\n\n",move_x);
		Message msg;
		msg.CarMoveRightMM(move_x);
		msg.sendMessage(this->Com->fd);
	}
	else
	{
		//move left
		uint16_t move_x = this->bound_dis(ceil(abs(diff_x) * p));
		printf("\n\n\nMotion_controller adjusting(): moving left %d mm\n\n\n",move_x);
		Message msg;
		msg.CarMoveLeftMM(move_x);
		msg.sendMessage(this->Com->fd);
	}

	int32_t tmp = IMG_EXP_POS1_Y;
	int diff_y = (int32_t) detect.y - tmp;
	printf("Motion_controller adjusting(): detect.y is %d\n",detect.y);
	printf("Motion_controller adjusting(): IMG_EXP_POS1_Y is %d\n",tmp);
	uint16_t move_y = ceil(abs(diff_y) * p);
	printf("Motion_controller adjusting(): length per pixel is %f, diff_y is %d\n",p,diff_y);
	if(diff_y > 0)
	{
		//moving down
		
		printf("\n\n\nMotion_controller adjusting(): moving down %d mm\n\n\n",move_y);
		Message msg;
		msg.LifterMoveDownMM(move_y);
		msg.sendMessage(this->Com->fd);
	}
	else
	{
		//moving up
		printf("\n\n\nMotion_controller adjusting(): moving up %d mm\n\n\n",move_y);
		Message msg;
		msg.LifterMoveUpMM(move_y);
		msg.sendMessage(this->Com->fd);
	}
	return 1;
}

uint16_t Motion_controller::bound_dis(const uint32_t &dis)
{
	if(dis > 0xffff)
		return 0xffff;
	else
		return (uint16_t) dis;
}
void Motion_controller::send_cmd()
{
	return ;
	printf("Motion_controller::send_cmd(): sending messages to the car\n");
	while(!this->cmd_queue.empty())
	{
		//this->Com->send_cmd(cmd_queue.front());
		//cmd_queue.front().sendMessage(this->Com->fd);
		cmd_queue.pop();
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