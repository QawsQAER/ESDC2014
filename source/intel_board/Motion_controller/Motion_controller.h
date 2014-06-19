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
#include "../macro.h"


#define EVAL_CENTERING 0
#define EVAL_ZOOMING 1
#define EVAL_ADJUSTING 2
#define EVAL_COMPLETE 3

class Motion_controller
{
private:
	//this is the reference rectangle, where the person should be
	std::queue<Message> cmd_queue;
	uint8_t eval_state;
	uint16_t lifter_pos; //unit mm.
	

	/*
	variable for reference in image evaluation:

		center_x, center y -> the center of the image.
		threshold_x, threshold_y -> the threshold for detected body region horizontally and vertically.
		threshold_face_x, threshold_face_y -> the threshold for detected face region horizontally and vertically

		img_exp_pos_x, img_exp_pos_y -> the expected position of x and y of the detected body (top center point)
		exp_width, exp_height -> the expected height and width of the detected body

		img_exp_face_pos_x, img_exp_face_pos_y -> the expected x and y of the detected body (top center point)
		exp_face_height, exp_face_width -> the expected height and width of the detected face
	*/
	uint16_t threshold_x, threshold_y;
	uint16_t img_exp_pos_x, img_exp_pos_y, exp_width, exp_height;
	uint16_t center_x, center_y;

	uint16_t threshold_face_y, threshold_face_x;
	uint16_t exp_face_height, exp_face_width;
	uint16_t img_exp_face_pos_x, img_exp_face_pos_y;
	//-------------------------------------------------------------------------------------------------//
	//-------------------------------------------------------------------------------------------------//

	uint8_t centering(const cv::Rect &detect,const cv::Rect &face);
	uint8_t centering_by_face(const cv::Rect &face);
	uint8_t zoom_in_out(const cv::Rect &detect,const double &distance);
	uint8_t adjusting(const cv::Rect &detect);
	uint8_t adjusting_by_face(const cv::Rect &face);

	uint16_t bound_dis(const uint32_t &dis);

	void zoom_in_out_by_distance(const cv::Rect &detect,const double &distance);
	void zoom_in_out_by_default(const cv::Rect &detect,const double &distance);
	void zoom_in_out_by_face(const cv::Rect &face,const double &distance);

	void lift(const uint16_t &mm, const uint8_t &dir);
public:

	uint8_t *waist_shot;
	cv::Rect ref;
	cv::Rect face_ref;
	Controller_Com *Com;
	uint8_t need_to_center;
	uint8_t need_to_zoom;
	uint8_t need_to_adjust;
	
	Motion_controller();
	~Motion_controller();

	uint8_t init();
	//this function will only be called when a target is detected by the Image processor
	uint8_t evaluate_image(const cv::Rect &detect,const cv::Rect &ref,const double &distance);
	//this function will read the detection result and then try to get a command such that the next detect result will be closer to the reference

	void move(const uint16_t &mm,const uint8_t &dir);
	void rotate(const uint16_t &degree,const uint8_t &dir);

	void set_pattern(uint8_t pattern);

	void reset_lifter();
	void set_lifter(const uint16_t &mm);
};

#endif
