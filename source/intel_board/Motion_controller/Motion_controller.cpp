#include "Motion_controller.h"

Motion_controller::Motion_controller()
{
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
	return 1;
}
uint8_t Motion_controller::evaluate_image(const cv::Rect &detect,const cv::Rect &ref)
{
	if(this->eval_state == EVAL_COMPLETE)
		return 1;
	else
	{	
		switch(this->eval_state)
		{
			case EVAL_CENTERING:
				if(this->centering(detect))
					this->eval_state = EVAL_ZOOMING;
			break;
			case EVAL_ZOOMING:
				if(this->zoom_in_out(detect))
					this->eval_state = EVAL_ADJUSTING;
			break;
			case EVAL_ADJUSTING:
				if(this->adjusting(detect))
					this->eval_state = EVAL_COMPLETE;
			break;
			default:
			break;

		}
		return 0;
	}
}

uint8_t Motion_controller::centering(const cv::Rect &detect)
{
	printf("Motion_controller::centering() running\n");
	uint8_t okay_image = 1;
	//allow positive or negative error in 50 pixels
	uint16_t threshold_x = 20, threshold_y = 20;
	uint16_t exp_center_x = 320, exp_center_y = 240;

	cv::Point center(detect.x + detect.width / 2,detect.y + detect.height / 2);

	int diff_x = center.x - exp_center_x;
	int diff_y = center.y - exp_center_y;
	//compute length per pixel 
	float p = (float) 1700 / (float) detect.height;

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
	printf("Motion_controller::zoom_in_out() running\n");
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