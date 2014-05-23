#include "Image_processor.h"

Image_processor::Image_processor()
{
	printf("Constructing Image processor\n");
	this->state = IMAGE_PROCESS_INIT;
	strcpy(this->winname,"hello, world");
	this->win_exist = 0;
}

Image_processor::~Image_processor()
{
	printf("Destructing Image processor\n");
}

IMAGE_PROCESS_STATE Image_processor::get_state()
{
	return this->state;
}

uint8_t Image_processor::hello_get_image_and_show()
{
	//get image by path
	cv::Mat image;
	if(this->win_exist)
		cv::destroyWindow(this->winname);
	else
		this->win_exist = 1;
	cv::namedWindow(this->winname,CV_WINDOW_AUTOSIZE);
	cv::imshow(this->winname,image);
	return 1;
}
