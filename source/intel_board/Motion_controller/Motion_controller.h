/*

*/

#ifndef _MOTION_CONTROLLER_H
#define _MOTION_CONTROLLER_H

//for queue
#include <queue>
//for standard int type
#include <stdint.h>
#include <stdio.h>
#include "Controller_Com.h"
#include <opencv2/opencv.hpp>
#include "../lib/message.h"

#define EVAL_CENTERING 0
#define EVAL_ZOOMING 1
#define EVAL_ADJUSTING 2
#define EVAL_COMPLETE 3

class Motion_controller
{
private:
	//this is the reference rectangle, where the person should be
	cv::Rect *ref;
	std::queue<Message> cmd_queue;
	uint8_t eval_state;

	uint8_t centering(const cv::Rect &detect);
	uint8_t zoom_in_out(const cv::Rect &detect);
	uint8_t adjusting(const cv::Rect &detect);

	uint16_t bound_dis(const uint32_t &dis);
public:
	Controller_Com *Com;
	
	Motion_controller();
	~Motion_controller();

	uint8_t init();
	//this function will only be called when a target is detected by the Image processor
	uint8_t evaluate_image(const cv::Rect &detect,const cv::Rect &ref);
	//this function will read the detection result and then try to get a command such that the next detect result will be closer to the reference
	void calculate_cmd(const cv::Rect &detect);
	void send_cmd();

	void move(const uint16_t &mm,const uint8_t &dir);
	void rotate(const uint16_t &degree,const uint8_t &dir);

	//this function will compute the distance of the target to the camera
	double compute_distance(const cv::Rect &ref,const cv::Rect &detect, uint16_t actual_height,uint16_t focus_length);
};

#endif
