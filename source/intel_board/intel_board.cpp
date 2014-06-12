/*
 * Copyright (C) Your copyright notice.
 *
 * Author: Edward HUANG@CUHK huangxx_2155@live.com
 *         Tony Yi@CUHK     
 *         Terry Lai@CUHK
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the PG_ORGANIZATION nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY	THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS-IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "intel_board.h"

intel_board::intel_board(uint8_t mode,uint8_t img_source)
{

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
	//img_source 1 for WEBCAM MODE, 0 for cellphone mode
	printf("Creating Image_processor()\n");
	this->image_processor = new Image_processor(img_source);
	printf("Creating Motion_controller()\n");
	this->motion_controller = new Motion_controller();
	printf("Creating UI()\n");
	this->ui = new UI();
	printf("hello intel board\n\n");
	state = ROBOT_INIT;
}

intel_board::~intel_board()
{
	printf("~intel_board(): deleting Image_processor\n");
	delete this->image_processor;
	printf("~intel_board(): deleting Motion_controller\n");
	delete this->motion_controller;
	printf("~intel_board(): deleting UI\n");
	delete this->ui;
	printf("Bye bye intel board\n");
}

uint8_t intel_board::main_function()
{
	printf("Intel board is going to execute its main functionality\n");
	char key;
	if(this->mode == IMG_ANALYSIS_MODE)
	{
		this->robot_only_image_analysis();
	}
	else
	{

		// the main state machine
		while(1)
		{
			switch(this->state)
			{
				case ROBOT_INIT:
					if(this->robot_init())
					{
						this->state = ROBOT_READY;
					}
					else
					{	
						printf("intel_board robot_init() return 0\n");
						exit(-1);
					}
					break;
				
				case ROBOT_READY:
					//the robot will be waiting for user's specific command to continue
					this->robot_ready();
					break;
				
				case ROBOT_FIND_TARGET:
					//working on finding the target
					if(this->robot_find_target())
					{
						this->state = ROBOT_EVALUATE_IMAGE;
						this->distance = this->image_processor->get_distance(this->image_processor->final_face_detect[0]);
						//printf("\n\n@@@@\nTHE TARGET IS %lf away from the camera\n@@@@@@@\n\n",this->distance);
					}
					break;

				case ROBOT_EVALUATE_IMAGE:
					if(this->robot_evaluate_image())
						//if the image is good enough
						//store the image and go back wait for the next command
						this->state = ROBOT_WAIT_FOR_ADJUSTMENT;
					else
						//let the system analyze the image and find out possible method to make it better
						this->state = ROBOT_ANALYZE_IMAGE;
					break;
				
				case ROBOT_ANALYZE_IMAGE:
					this->robot_analyze_image();
					this->state = ROBOT_APPROACH_REF;
					break;
				
				case ROBOT_APPROACH_REF:
					this->robot_approach_ref();
					//take another picture and check whether the target is in scope
					printf("intel_board: ROBOT_APPROACH_REF finished\n");
					this->state = ROBOT_FIND_TARGET;
					break;
				case ROBOT_WAIT_FOR_ADJUSTMENT:
					this->robot_wait_for_adjustment();
					this->state = ROBOT_READY;
					break;
			}
		}
	}
	return 1;
}

uint8_t intel_board::robot_init()
{
	printf("intel_board: the robot is in init state\n");
	if(!this->image_processor->init())
		return 0;
	if(!this->motion_controller->init())
		return 0;
	printf("intel_board: initilization done\n");
	return 1;
}

uint8_t intel_board::robot_ready()
{
	printf("intel_board: the robot is in ready state\n");
	command_type cmd = ui->wait_command();
	printf("intel_board: the robot has received %d\n",cmd);
	
	if(cmd == pattern_1 || cmd == pattern_2 || cmd == pattern_3 || cmd == pattern_4)
	{
		command_type cmd = ui->wait_command();
		if(cmd == start_movement)
		{
			printf("intel_board: the robot is going to find target\n\n\n");
			this->state = ROBOT_FIND_TARGET;
			return 1;
		}
	}	
}

uint8_t intel_board::robot_find_target()
{
	printf("intel_board::robot_find_target() running\n");
	uint8_t state = 0;
	uint8_t counter = 0;
	uint16_t dis = 350;
	uint16_t degree = 30;
	uint8_t sec = 1;

	this->robot_countdown(sec);	
	while(!this->image_processor->target_in_scope())
	{
		//TODO: may adjust the position according to the initial detection results
		this->robot_countdown(sec);
		//rotate 30 degree every time if no target is detected
		printf("intel_board::robot_find_target(): finding target again\n");

		switch(state)
		{
			case 0://repeat state
				if(counter < 3)
				//do nothing
					counter++;
				else
					state = 1;
				break;
			case 1://forward 300mm
				this->motion_controller->move(dis,0);
				state = 2;
				break;
			case 2://backward 600mm
				this->motion_controller->move(dis * 2,1);
				state = 3;
				break;
			case 3:
				this->motion_controller->move(dis,0);
				this->motion_controller->move(dis,2);
				state = 4;
				break;
			case 4:
				this->motion_controller->move(dis * 2,3);
				state = 5;
				break;
			case 5:
				this->motion_controller->rotate(degree,0);
				state = 0;
				counter = 0;
				break;
			default:
			break;
		}
		printf("intel_board: sleep till the camera is stable\n");
	}
	printf("intel_board::robot_find_target(): TARGET FOUND!\n\n\n");
	return 1;
}

uint8_t intel_board::robot_evaluate_image()
{
	printf("intel_board: robot_evaluate_image() running\n");
	cv::Rect dummy;
	if(this->image_processor->final_face_detect.size() != 0)
		return this->motion_controller->evaluate_image(this->image_processor->get_detection_result(),dummy,this->distance);
	else
		printf("intel_board::robot_evaluate_image() error: evaluating an image without detection result!\n");
		return 0;
}

uint8_t intel_board::robot_analyze_image()
{
	printf("intel_board::robot_analyze_image() running\n");
	//analyze the image and get the expected movement to take
	//currently it is done in robot_evaluate_image()
	return 1;
}

uint8_t intel_board::robot_approach_ref()
{
	printf("intel_board::robot_approach_ref() running\n");
	//have the motion_controller send the command to the car
	this->motion_controller->send_cmd();
	printf("intel_board::robot_approach_ref() send cmd finished\n");
	return 1;
}

uint8_t intel_board::robot_wait_for_adjustment()
{

	printf("intel_board::robot_wait_for_adjustment() running\n");
	//TODO:
	//should be waiting for adjustment here.

	this->image_processor->cam->save_photo_af();
	this->ui->send_finished_ack();
	while(ui->wait_command() != confirm_picture)
	{
		
	}
	return 1;
}

uint8_t intel_board::robot_only_image_analysis()
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
		this->image_processor->init();
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
		//ptr->run_body_detection(ptr->current_img,ptr->body_detect);
		//run the basic face detection algorithm
		ptr->run_body_detection(ptr->current_img,ptr->body_detect);
		ptr->run_face_detection(ptr->current_img,ptr->face_detect);
		//mark the face and body after basic detection algorithm
		//ptr->analyzed_img = ptr->mark_detected_body(ptr->current_img,ptr->body_detect);
		//ptr->analyzed_img = ptr->mark_detected_face(ptr->analyzed_img,ptr->face_detect);
		//show the analyzed img after basic detection algorithm
		//ptr->show_analyzed_img();
		//run a basic filter 
		ptr->basic_filter();
		//mark the face and body after basic filter
		ptr->analyzed_img = ptr->mark_detected_body(ptr->current_img,ptr->final_body_detect);
		ptr->analyzed_img = ptr->mark_detected_face(ptr->analyzed_img,ptr->final_face_detect);
		//show the analyzed img after basic filter
		this->robot_evaluate_image();
		ptr->show_analyzed_img();
	}
}

void intel_board::robot_countdown(uint8_t sec)
{
	useconds_t usec = 1000000;
	//this->ui->send_finished_ack();
	printf("intel_board: counting down\n");
	for(uint8_t count = sec;count > 0;count--)
	{
		printf("intel_board: counting down %d\n",count);
		usleep(usec);
	}
	
}