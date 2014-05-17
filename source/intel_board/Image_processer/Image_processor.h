/*

*/

#ifndef _IMAGE_PROCESSOR_H
#define _IMAGE_PROCESSOR_H

class Image_processor
{
private:

public:	
	Image_processor();
	~Image_processor();
	static uint32_t capture_image();
	static uint32_t analyze_image();
};

#endif