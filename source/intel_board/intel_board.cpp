#include "intel_board.h"

intel_board::intel_board(uint8_t mode,uint8_t img_source)
{
	//1 for WEBCAM MODE, 0 for cellphone mode
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
			printf("RUNNING IN IMG_ANALYSIS_MODE\n");
			this->mode = IMG_ANALYSIS_MODE;
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

uint8_t intel_board::init()
{
	if(!this->image_processor->init())
		return -1;

	return 1;
}
uint8_t intel_board::main_function()
{
	printf("Intel board is going to execute its main functionality\n");
	char key;
	if(this->mode == IMG_ANALYSIS_MODE)
	{
		DIR *dir;
		struct dirent *ent;
		if((dir = opendir(dir_path)) == NULL)
		{
			perror("");
			return EXIT_FAILURE;
		}
		while((ent = readdir(dir)) != NULL)
		{
			if(strcmp(ent->d_name,".") == 0 || strcmp(ent->d_name,"..") == 0)
				continue;
			char filename[64];
			strcpy(filename,dir_path);
			strcat(filename,"/");
			strcat(filename,ent->d_name);
			printf("Openning %s\n",filename);
			Image_processor *ptr = this->image_processor;
			ptr->read_image(filename);
			ptr->load_current_img_to_analyzed_img();

			//run the basic body detection algorithm
			ptr->run_body_detection(ptr->current_img,ptr->body_detect);
			//run the basic face detection algorithm
			ptr->run_face_detection(ptr->current_img,ptr->face_detect);

			//mark the face and body after basic detection algorithm
			ptr->analyzed_img = ptr->mark_detected_body(ptr->current_img,ptr->body_detect);
			ptr->analyzed_img = ptr->mark_detected_face(ptr->analyzed_img,ptr->face_detect);
			//show the analyzed img after basic detection algorithm
			ptr->show_analyzed_img();

			//run a basic filter 
			ptr->basic_filter();
			//mark the face and body after basic filter
			ptr->analyzed_img = ptr->mark_detected_body(ptr->current_img,ptr->final_body_detect);
			ptr->analyzed_img = ptr->mark_detected_face(ptr->analyzed_img,ptr->final_face_detect);
			//show the analyzed img after basic filter
			ptr->show_analyzed_img();
		}
	}
	else
	{
		while(1)
		{
			//running image_processor->test()
			this->image_processor->test();
		}
	}
	return 1;
}

