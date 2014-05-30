/*

*/

#ifndef _MOTION_CONTROLLER_H
#define _MOTION_CONTROLLER_H

//for queue
#include <queue>
//for standard int type
#include <stdint.h>
#include "Controller_Com.h"
#include <opencv2/opencv.hpp>
#include "../lib/message.h"

class Motion_controller
{
private:
	cv::Rect ref;
	std::queue<struct IntelCarCmd> cmd_queue;
	Controller_Com *Com;
public:
	Motion_controller();
	~Motion_controller();
	static void cal_cmd();
	static void send_cmd();
};

#endif
