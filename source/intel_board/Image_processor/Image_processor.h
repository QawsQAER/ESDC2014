/*

*/

#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

#include <stdint.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
//#include "../camera/camera.h"

#define WINNAME_LENGTH 32
enum IMAGE_PROCESS_STATE
{
	IMAGE_PROCESS_INIT
};
class Image_processor
{
private:
	//the name of the displayed window
	char winname[WINNAME_LENGTH];
	//the state of the Image_processor
	IMAGE_PROCESS_STATE state;
	//the boolean variable which indicates whether there exist a window
	bool win_exist;
public:	
	Image_processor();
	~Image_processor();
	uint8_t capture_image();
	uint8_t analyze_image();
	uint8_t show_image(char *filename);
	uint8_t hello_get_image_and_show();
	IMAGE_PROCESS_STATE get_state();
};

#endif
