#include "Image_processor.h"

Image_processor::Image_processor()
{
	printf("Constructing Image processor\n");
	this->state = IMAGE_PROCESS_INIT;
}

Image_processor::~Image_processor()
{
	printf("Destructing Image processor\n");
}

IMAGE_PROCESS_STATE Image_processor::get_state()
{
	return this->state;
}
