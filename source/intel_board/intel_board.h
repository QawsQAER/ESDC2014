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

enum ROBOT_STATE {
	ROBOT_INIT,
	ROBOT_READY,
	ROBOT_NO_TARGET,
	ROBOT_FOUND_TARGET,
	ROBOT_ANALYZE_IMAGE,
	ROBOT_APPROACH_REF
};

enum ROBOT_MODE{
	AUTO_MODE,
	MANUAL_MODE,
	DEBUG_MODE,
	IMG_ANALYSIS_MODE
};

class intel_board
{
private:
	Image_processor *image_processor;
	ROBOT_STATE state;
	ROBOT_MODE mode;
	/*
	uint8_t robot_ready();
	uint8_t robot_no_target();
	uint8_t robot_found_target();
	uint8_t robot_analyze_image();
	uint8_t robot_approach_ref();
	*/
public:
	intel_board(uint8_t mode,uint8_t img_source);
	~intel_board();
	//this function will call the init functions of all its components
	uint8_t init();
	uint8_t main_function();
};

#endif
