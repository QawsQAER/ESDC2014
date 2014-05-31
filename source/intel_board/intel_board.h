/*
 *
 */
#ifndef _INTEL_BOARD_H
#define _INTEL_BOARD_H


#include "Image_processor/Image_processor.h"
//#include "Motion_controller/Motion_controller.h"
#include "lib/message.h"

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

//for openning a directory
#include <sys/types.h>
#include <dirent.h>

//this is defined, declared and allocated space in ESDC.cpp
extern char *dir_path;

enum ROBOT_STATE {
	ROBOT_INIT,
	ROBOT_READY, //actually waiting for command
	ROBOT_NO_TARGET,
	ROBOT_FIND_TARGET,
	ROBOT_EVALUATE_IMAGE,
	ROBOT_ANALYZE_IMAGE,
	ROBOT_APPROACH_REF,
	ROBOT_WAIT_FOR_ADJUSTMENT,
};

enum ROBOT_MODE{
	AUTO_MODE,
	MANUAL_MODE,
	IMG_ANALYSIS_MODE
};

class intel_board
{
private:
	Image_processor *image_processor;
	ROBOT_STATE state;
	ROBOT_MODE mode;
	Motion_controller *motion_controller;
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
};

#endif
