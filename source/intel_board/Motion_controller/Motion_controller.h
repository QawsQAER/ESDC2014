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
	//this is the reference rectangle, where the person should be
	cv::Rect ref;
	std::queue<struct IntelCarCmd> cmd_queue;
	Controller_Com *Com;
public:
	Motion_controller();
	~Motion_controller();
	//this function will read the detection result and then try to get a command such that the next detect result will be closer to the reference
	void calculate_cmd(const cv::Rect &detect);
	void send_cmd();

	void move_forward(const uint16_t &mm);&
	void move_backward(const uint16_t &mm);
	void move_left(const uint16_t &mm);
	void move_right(const uint16_t &mm);
	void rotate(const uint8_t &dir,const uint16_t &degree);

	//this function will compute the distance of the target to the camera
	double compute_distance(const cv::Rect &ref,const cv::Rect &detect, uint16_t actual_height,uint16_t focus_length);
};

#endif
