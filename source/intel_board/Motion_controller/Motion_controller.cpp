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
	//initialize threshold value
	this->threshold_x = IMG_HORI_THRESHOLD;
	this->threshold_y = IMG_VERT_THRESHOLD;
	this->center_x = IMG_CENTER_X;
	this->center_y = IMG_CENTER_Y;
	this->exp_x = 320;
	this->exp_y = 240;
	this->exp_width = IMG_EXP_WIDTH;
	this->exp_height = IMG_EXP_HEIGHT;
	printf("Motion_controller(): Constructing new Motion_controller\n");
	this->Com = new Controller_Com("/dev/ttyUSB0");
	//TODO: initilized the reference rectangle
}

Motion_controller::~Motion_controller()
{
	delete this->Com;
}

uint8_t Motion_controller::init()
{
	printf("Motion_controller::init() running\n");
	Message msg;
	msg.CarRotateLeftDegree(360);
	msg.sendMessage(this->Com->fd);
	printf("Motion_controller::init() returning\n");
	return 1;
}


uint8_t Motion_controller::evaluate_image(const cv::Rect &detect,const cv::Rect &ref)
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
	if(abs(diff_x) > threshold_x)//need to adjust horizontally to the center
	{
		this->centering(detect);
		return 0;
	}

	if(abs(diff_y) > threshold_y)//need to zoom in or zoom out
	{
		this->zoom_in_out(detect);
		return 0;
	}

	if(false)//need to move to the expected position
	{
		//never enter this phase for now
		return 0;
	}

	return 1;
}

uint8_t Motion_controller::centering(const cv::Rect &detect)
{
	printf("\nMotion_controller::centering() running\n");
	uint8_t okay_image = 1;
	//allow positive or negative error in 50 pixels
	uint16_t threshold_x = 50, threshold_y = 50;
	uint16_t exp_center_x = 320, exp_center_y = 240;

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

uint8_t Motion_controller::zoom_in_out(const cv::Rect &detect)
{
	printf("\nMotion_controller::zoom_in_out() running\n");
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
	return 1;
}

uint8_t Motion_controller::adjusting(const cv::Rect &detect)
{
	printf("Motion_controller::adjusting() running\n");
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