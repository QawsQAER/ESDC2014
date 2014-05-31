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

uint8_t Motion_controller::evaluate_image(const cv::Rect &detect,const cv::Rect &ref)
{
		return this->centering(detect);
}

uint8_t Motion_controller::centering(const cv::Rect &detect)
{
	uint8_t okay_image = 1;
	//allow positive or negative error in 50 pixels
	uint16_t threshold_x = 50, threshold_y = 50;
	uint16_t exp_center_x = 320, exp_center_y = 240;

	cv::Point center(detect.x + detect.width / 2,detect.y + detect.height / 2);
	//TODO: use ref also
	if(detect.height > 300)
	{
		//set command move backward 300mm
		uint16_t distance = 300;
		Message m;
		m.CarMoveDown(distance);
		this->cmd_queue.push_back(m);
		printf("Motion_controller::centering(): the height is too large %d\n",detect.height);
		okay_image = 0;
	}

	int diff_x = center.x - exp_center_x;
	int diff_y = center.y - exp_center_y;
	float p = 0.5;
	uint16_t move_x = 0;
	if(abs(diff_x) > threshold_x)
	{
		okay_image = 0;
		//push movement to right or left
		printf("Motion_controller centering(): diff_x is %d\n",diff_x);
		if(diff_x < 0)
		{	
			//should move left
			move_x = ceil(abs(diff_x) * p);
			printf("Motion_controller centering(): moving left %d mm\n",move_x);
		}
		else
		{
			//should move right
			move_x = ceil(abs(diff_x) * p);
			printf("Motion_controller centering(): moving right %d mm\n",move_x);
		}
	}
	if(abs(diff_y) > threshold_y)
	{
		//push lifter movement
	}

	return okay_image;
}
void Motion_controller::send_cmd()
{
	/*
	while(!this->cmd_queue.empty())
	{
		this->Com->send_cmd(cmd_queue.front());
		cmd_queue.pop();
	}
	*/
}