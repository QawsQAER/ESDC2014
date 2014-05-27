/*

*/

#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

#include <stdint.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include "../macro.h"
#include <vector>
#include <time.h>
//#include "../camera/camera.h"

#define WINNAME_LENGTH 32

#ifndef IMG_SOURCE_CELLPHONE
#define IMG_SOURCE_CELLPHONE 0
#endif

#ifndef IMG_SOURCE_WEBCAM
#define IMG_SOURCE_WEBCAM 1
#endif

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

	//this variable indicates where the frame for analysis comes from
	uint8_t img_source;

	cv::VideoCapture *cap;
	//this variable stores the current image that is being processed
	cv::Mat current_img;
	cv::Mat analyzed_img;

	//this function will get image from cell phone, and load the image into the current_img
	uint8_t get_image_from_cellphone();
	//this function will get image from camera, and load the image into the current_img
	uint8_t get_image_from_webcam();

public:	
	Image_processor(uint8_t img_source);
	~Image_processor();

	//this function will return a Mat variable.
	uint8_t capture_image();

	//this function will stored the image into the hard disk,
	//naming it according to the time since Epoch
	uint8_t save_current_image();

	uint8_t analyze_image();
	
	/*this function will use current_img as image source and gives out analyzed result*/
	uint8_t basic_pedestrain_detection();
	
	uint8_t show_analyzed_img();
	uint8_t get_image_and_show();
	IMAGE_PROCESS_STATE get_state();
	void test();
};

#endif
