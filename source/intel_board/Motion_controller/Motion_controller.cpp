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
	//TODO: use ref also
	if(detect.height > 300)
	{
		//set command move backward 300mm
		uint16_t distance = 300;
		Message m;
		m.CarMoveDown(distance);
		this->cmd_queue.push_back(m);
		return 0;
	}
	else
		return 1;
}

void Motion_controller::send_cmd()
{
	while(!this->cmd_queue.empty())
	{
		this->Com->send_cmd(cmd_queue.front());
		cmd_queue.pop();
	}
}