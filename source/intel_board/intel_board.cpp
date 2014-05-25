#include "intel_board.h"

intel_board::intel_board(uint8_t mode,uint8_t img_source)
{
	//1 for WEBCAM MODE
	this->image_processor = new Image_processor(img_source);
	switch(mode)
	{
	case(0):
			printf("RUNNING in AUTO MODE\n");
			this->mode = AUTO_MODE;
			break;
	case(1):
			printf("RUNNING in MANUAL MODE\n");
			this->mode = MANUAL_MODE;
			break;
	case(2):
			printf("RUNNING in DEBUG MODE\n");
			this->mode = DEBUG_MODE;
			break;
	default:
			printf("Invalid Mode\nExiting program\n");
			exit(1);
	}

	printf("hello intel board\n");
	state = ROBOT_INIT;
}

intel_board::~intel_board()
{
	printf("Bye bye intel board\n");
}

uint8_t intel_board::main_function()
{
	printf("Intel board is going to execute its main functionality\n");
	char key;
	while(1)
	{
		//running image_processor->test()
		this->image_processor->test();
	}
	return 1;
}

uint8_t intel_board::robot_init()
{
	printf("Initilizing the robot\n");
	return 1;
}
