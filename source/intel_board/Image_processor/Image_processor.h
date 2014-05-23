/*

*/

#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

#include <stdint.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>

enum IMAGE_PROCESS_STATE
{
	IMAGE_PROCESS_INIT
};
class Image_processor
{
private:
	IMAGE_PROCESS_STATE state;
public:	
	Image_processor();
	~Image_processor();
	uint8_t capture_image();
	uint8_t analyze_image();
	IMAGE_PROCESS_STATE get_state();
};

#endif
