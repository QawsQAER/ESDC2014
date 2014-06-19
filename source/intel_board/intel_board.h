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

#ifndef _INTEL_BOARD_H
#define _INTEL_BOARD_H


#include "Image_processor/Image_processor.h"
#include "Motion_controller/Motion_controller.h"
#include "User_interface/ui.h"
//#include "Motion_controller/Motion_controller.h"
#include "lib/message.h"
#include "macro.h"

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

//for openning a directory
#include <sys/types.h>
#include <dirent.h>

//this is defined, declared and allocated space in ESDC.cpp
extern char *dir_path;

class intel_board
{
private:
	//STATE
	/*
		state: indicates the current state of the robot
		mode: indicates what mode is the robot operating in, auto mode, manual mode or image analysis mode
		photo_mode: indicates whether to take photo for single, double or more people
		task_counter: to count the number of task
		waist_shot: whether it's for waist shot or not.
	*/
	ROBOT_STATE state;
	ROBOT_MODE mode;
	PHOTO_MODE photo_mode;
	uint16_t task_counter;
	uint8_t waist_shot;
	
	//SUBMODULE
	Image_processor *image_processor;
	Motion_controller *motion_controller;
	UI *ui;

	//MISC
	double distance;
	
public:
	intel_board(uint8_t mode,uint8_t img_source);
	~intel_board();

	uint8_t main_function();

	//this function will initilize the whole system
	uint8_t robot_init();
	//this function will be called after init,
	//it shall be waiting command from the user
	uint8_t robot_ready();

	uint8_t robot_no_target();
	
	//this function will be called to find a target 
	uint8_t robot_find_target();
	uint8_t robot_evaluate_image();
	uint8_t robot_analyze_image();
	uint8_t robot_approach_ref();
	uint8_t robot_wait_for_adjustment();

	uint8_t robot_exit();

	void robot_countdown(uint8_t sec);
	uint8_t robot_only_image_analysis();

	void robot_act_by_cmd(const command &cmd);
};

#endif
