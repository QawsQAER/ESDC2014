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
	//STATE
	//STATE
	//STATE
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
	this->state = ROBOT_INIT;
	this->photo_mode = SINGLE_PHOTO;
	this->waist_shot = 0;
	this->flag_target_found = 0;
	this->rv_evaluate_image = 0;
	//SUBMODULE
	//SUBMODULE
	//SUBMODULE

	//img_source 1 for WEBCAM MODE, 0 for cellphone mode
	printf("Creating Image_processor()\n");
	this->image_processor = new Image_processor(img_source);

	printf("Creating Motion_controller()\n");
	this->motion_controller = new Motion_controller();
	this->motion_controller->waist_shot = &this->waist_shot;
	printf("Creating UI()\n");
	this->ui = new UI();

	printf("hello intel board\n\n");	
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
	
	// the main state machine
	if(glo_test_mbed)
		this->robot_test_mbed();

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

					//get the distance according to the face detection result (running four point algorithm)
					this->distance = this->image_processor->get_distance(this->image_processor->get_face_detection_result());
					this->state = ROBOT_EVALUATE_IMAGE;
					this->flag_target_found = 1;
				}
			break;

			case ROBOT_EVALUATE_IMAGE:
			{	
				if((rv_evaluate_image = this->robot_evaluate_image()) == EVAL_ADJUSTING)
				{	
					//if the image is good enough
					//store the image and go back wait for the next command
					this->state = ROBOT_WAIT_FOR_ADJUSTMENT;
				}
				else
				{	
					//let the system analyze the image and find out possible method to make it better
					glo_prev_face = this->motion_controller->prev_face;
					this->state = ROBOT_ANALYZE_IMAGE;
				}
			}
			break;
			
			case ROBOT_ANALYZE_IMAGE:
				this->robot_analyze_image();
				this->state = ROBOT_EVALUATE_MOVEMENT;
				break;
			
			case ROBOT_EVALUATE_MOVEMENT:
			{	
				if(this->robot_evaluate_movement())
				{
					// if the robot_evaluate_movement() return 1 -> does not find target in scope repeatedly 3 times 
					this->state = ROBOT_FIND_TARGET;
					this->flag_target_found = 0;
				}
				else
				{
					// if the robot_evaluate_movement() return 0 -> does find target in scope
					this->state = ROBOT_EVALUATE_IMAGE;
				}
				//take another picture and check whether the target is in scope
				printf("intel_board: ROBOT_EVALUATE_MOVEMENT finished\n");
			}
			break;

			case ROBOT_WAIT_FOR_ADJUSTMENT:
				this->robot_wait_for_adjustment();
				this->state = ROBOT_READY;
			break;
	
			default:
				printf("Intel_board:: default case in state machine\n");
				exit(-1);
			break;
		}
	}
	return 1;
}

uint8_t intel_board::robot_init()
{
	printf("intel_board: the robot is in init state\n");
	printf("intel_board: setting the task_counter to 0\n");
	this->task_counter = 0;
	if(!this->image_processor->init())
		return 0;
	if(!this->motion_controller->init())
		return 0;
	printf("intel_board: initilization done\n");
	return 1;
}

void intel_board::robot_get_degree(int32_t *degree,int32_t *dir)
{
	Message msg;
	msg.CompassRequest();
	printf("intel_board::robot_get_degree() getting degree from car\n");
	msg.safe_sendMessage(this->motion_controller->Com->fd);
	printf("intel_board::robot_get_degree() getting degree from phone\n");
	int32_t phone_degree = ui->update_degree();
	printf("intel_board::robot_get_degree() get degree from phone %d\n",phone_degree);
	printf("intel_board::robot_get_degree() get degree from compass %d\n",msg.car_degree);
	degree_rotation(msg.car_degree,phone_degree,degree,dir);
	printf("intel_board::robot_get_degree() degree is %d dir is %d\n",*degree,*dir);
}

void intel_board::robot_orientation_adjust()
{
	printf("intel_board::robot_orientation_adjust() entering\n");
	int32_t degree = 0,dir = 0;
	
	while(true)
	{
		uint16_t phone_ori = (uint16_t) this->ui->update_degree();
		if(!this->motion_controller->orientation_adjust(phone_ori))
			break;
		//original implementation of robot_orientation_adjust
		/*
		printf("intel_board::robot_orientation_adjust() sending compass request\n");
		this->robot_get_degree(&degree,&dir);
		printf("intel_board::robot_orientation_adjust() dir is %d, degree is %u after uint16_t conversion\n",dir,(uint16_t) degree);
		if(degree > ORIENTATION_THRESHOLD)
		{
			if(dir > 0)
				this->motion_controller->rotate((uint16_t) degree,(uint8_t) CAR_ROTATE_RIGHT);
			else
				this->motion_controller->rotate((uint16_t) degree,(uint8_t) CAR_ROTATE_LEFT);
		}
		else
		{
			printf("intel_board::robot_orientation_adjust() exiting\n");
			break;
			return ;
		}*/
	}
}
uint8_t intel_board::robot_ready()
{
	printf("intel_board: the robot is in ready state\n");

	//fetch degree
	this->motion_controller->set_initial_car_orientation((uint16_t) this->ui->update_degree());
	this->robot_orientation_adjust();
	this->waist_shot = 1;
	this->flag_target_found = 0;
	command_type cmd;
	printf("intel_board::robot_ready() waiting for user command\n");
	while(cmd = ui->wait_command())
	{
		if(cmd == start_movement)
		{
			this->task_counter++;
			printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
			printf("INTEL BOARD STARTING A NEW TASK\n!!");
			printf("intel_board: the robot is going to find target\n");
			this->state = ROBOT_FIND_TARGET;
			return 1;
		}
		
		switch(cmd)
		{
			case(pattern_1):
				this->waist_shot = 0;
				this->motion_controller->set_pattern(1);
			break;

			case(pattern_2):
				this->waist_shot = 0;
				this->motion_controller->set_pattern(2);
			break;

			case(pattern_3):
				this->waist_shot = 1;
				this->motion_controller->set_pattern(3);
			break;
			
			case(pattern_4):
				this->waist_shot = 1;
				this->motion_controller->set_pattern(4);
			break;
			
			case(set_waist_shot):
				this->waist_shot = this->waist_shot ? 0:1;
			break;

			default:
				printf("intel_board::robot_ready() undefined reaction for command %d\n",cmd);
			break;
		}
	}
	return 0;
}

uint8_t intel_board::robot_find_target()
{
	printf("intel_board::robot_find_target() running\n");
	uint8_t state = 0;
	uint8_t counter = 0;
	uint16_t dis = 350;
	uint16_t degree = 30;
	uint8_t sec = 0;
	int8_t rv = 0;

	while(true)
	{

		int32_t degree = 0,dir = 0;
		//this->robot_get_degree(&degree,&dir);
		rv = this->robot_target_in_scope(ENABLE_BODY_DETECT | ENABLE_FACE_DETECT);

		printf("intel_board::robot_find_target rv is %d\n",rv);
		if(rv < 0)
			continue;

		this->robot_show_image();

		//this->robot_find_target_strategy1(state,counter);
		if(rv == 0)
		{
			printf("intel+board::robot_find_target going to execute startegy\n");
			this->robot_find_target_strategy2(state);
			this->robot_countdown(sec);
		}
		else 
			break;
	}

	
	printf("intel_board::robot_find_target(): TARGET FOUND!\n");
	printf("intel_board::robot_find_target() exiting\n");
	return 1;
}

uint8_t intel_board::robot_find_target_strategy1(uint8_t &state,uint8_t &counter)
{
	this->robot_show_image();	
	//TODO: may adjust the position according to the initial detection results
	printf("intel_board::robot_find_target(): finding target again\n");
	
	switch(state)
	{
		case 0://repeat state
			if(counter < 2)
			//do nothing
				counter++;
			else
				state = 1;
		break;
		
		case 1://forward [dis] mm
			this->motion_controller->move(DEFAULT_DIS_SMALL,0);
			state = 2;
		break;
		
		case 2://backward [dis] mm
			this->motion_controller->move(DEFAULT_DIS_SMALL * 2,1);
			state = 3;
		break;
		
		case 3: //right side [dis] mm
			this->motion_controller->move(DEFAULT_DIS_SMALL,0);
			this->motion_controller->move(DEFAULT_DIS_SMALL,2);
			state = 4;
		break;
		
		case 4: //left side [dis * 2] mm
			this->motion_controller->move(DEFAULT_DIS_SMALL * 2,3);
			state = 5;
		break;
		
		case 5:
			//move right [dis] mm
			this->motion_controller->move(DEFAULT_DIS_SMALL,2);
			//rotate 30 degree
			this->motion_controller->rotate(DEFAULT_DEGREE,CAR_ROTATE_RIGHT);
			state = 0;
			counter = 0;
		break;
		
		default:
		break;
	}
	return 1;
}

uint8_t intel_board::robot_find_target_strategy2(uint8_t &state)
{
	if(state < 2)
	{
		state++;
	}
	else
	{
		this->motion_controller->rotate(DEFAULT_DEGREE,CAR_ROTATE_RIGHT);
		state = 0;
	}
	return 0;
}
uint8_t intel_board::robot_evaluate_image()
{
	printf("intel_board: robot_evaluate_image() running\n");
	if(this->image_processor->final_face_detect.size() != 0)
		return this->motion_controller->evaluate_image(
			this->image_processor->get_detection_result(),
			this->image_processor->get_face_detection_result(),
			this->distance);
	else
		printf("intel_board::robot_evaluate_image() error: evaluating an image without detection result!\n");
		return 0;
}

uint8_t intel_board::robot_analyze_image()
{
	printf("intel_board::robot_analyze_image() running\n");
	//analyze the image and get the expected movement to take
	//currently it is done in robot_evaluate_image()
	printf("intel_board::robot_analyze_image() exiting\n");
	return 1;
}

uint8_t intel_board::robot_evaluate_movement()
{
	//evaluate movement do the following works
	printf("intel_board::robot_evaluate_movement() running\n");
	
	uint8_t flags;
	if((*this->motion_controller->waist_shot))
		flags = ENABLE_FACE_DETECT;
	else
		flags = ENABLE_FACE_DETECT | ENABLE_BODY_DETECT;

	if(this->rv_evaluate_image == EVAL_CENTERING)
	{
		flags = flags | ENABLE_SIDE_FILTERING;
		printf("intel_board::robot_evaluate_movement() last movement is centering\n");
	}
	else if(this->rv_evaluate_image == EVAL_ZOOM_IN)
	{
		flags = flags | ENABLE_SIZE_FILTERING_SMALL;
		printf("intel_board::robot_evaluate_movement() last movement is zoom in\n");
	}
	else if(this->rv_evaluate_image == EVAL_ZOOM_OUT)
	{
		flags = flags | ENABLE_SIZE_FILTERING_LARGE;
		printf("intel_board::robot_evaluate_movement() last movement is zoom out\n");
	}
	
	int8_t rv = 0;
	uint8_t count_retry = 0,find_target_again = 0;
	while(true)
	{
		rv = this->robot_target_in_scope(flags);
		if(rv < 0)
			continue;
		if(rv == 0)
		{
			if(count_retry < 3)
				count_retry++;
			else
			{
				find_target_again = 1;
				break;
			}	
			this->robot_show_image();
		}
		else if(rv > 0)
			break;
	}
	
	this->robot_show_image();
	printf("Intel_board: GOING TO EVALUATE THE NEW IMAGE\n");
	printf("Intel_board: GOING TO EVALUATE THE NEW IMAGE\n");
	printf("Intel_board: GOING TO EVALUATE THE NEW IMAGE\n");

	//whether go back to find target
	return find_target_again;
}

uint8_t intel_board::robot_wait_for_adjustment()
{

	printf("intel_board::robot_wait_for_adjustment() running\n");
	//TODO:
	//should be waiting for adjustment here.

	ui->send_notification();

	command_type cmd;
	while((cmd = ui->wait_command()) != confirm_picture)
	{
		this->robot_act_by_cmd(cmd);
	}

	this->image_processor->cam->save_photo_af();
	this->robot_target_in_scope(ENABLE_FACE_DETECT);
	this->robot_show_image();
	
	this->ui->send_finished_ack(this->image_processor->current_img_path);

	this->motion_controller->set_lifter(LIFTER_INIT_POS);
	printf("intel_board:: task %d finished\n\n\n",this->task_counter);
	return 1;
}

void intel_board::robot_countdown(uint8_t sec)
{
	useconds_t usec = 1000000;
	printf("intel_board::robot_countdown counting down\n");
	for(uint8_t count = sec;count > 0;count--)
	{
		printf("intel_board::robot_countdown counting down %d\n",count);
		usleep(usec);
	}
	printf("intel_board::robot_countdown finished\n");
}

void intel_board::robot_act_by_cmd(const command_type & cmd)
{
	switch(cmd)
	{
		case(car_forward):
			this->motion_controller->move(DEFAULT_DIS_SMALL,CAR_FORWARD);
		break;
		
		case(car_backward):
			this->motion_controller->move(DEFAULT_DIS_SMALL,CAR_BACKWARD);
		break;
		
		case(car_left):
			this->motion_controller->move(DEFAULT_DIS_SMALL,CAR_LEFT);
		break;

		case(car_right):
			this->motion_controller->move(DEFAULT_DIS_SMALL,CAR_RIGHT);
		break;

		case(lift_up):
			this->motion_controller->lift(LIFTER_SEG,LIFTER_UP);
		break;

		case(lift_down):
			this->motion_controller->lift(LIFTER_SEG,LIFTER_DOWN);
		break;

		default:
			printf("intel_board::robot_act_by_cmd has not yet implemented reaction to %d\n",cmd);
		break;	
	}
}




void intel_board::robot_show_image()
{
	if(glo_multi_target)
		this->image_processor->mark_exp_region(this->image_processor->face_region);
	else if(this->waist_shot)
		this->image_processor->mark_exp_region(this->motion_controller->face_ref);
	else		
		this->image_processor->mark_exp_region(this->motion_controller->ref);
	this->image_processor->show_analyzed_img(this->task_counter);
	return ;
}

uint8_t intel_board::robot_target_in_scope(const uint8_t &flags)
{
	uint8_t rv = 0;
	this->motion_controller->buzzer(BUZZER_TAKE_PHOTO);

	if(glo_multi_target)
		rv = this->image_processor->multi_targets_in_scope(flags,glo_num_target);
	else
		rv = this->image_processor->one_target_in_scope(flags); //does not apply compass filtering now
	if(rv == 0)
	{	
		this->motion_controller->buzzer(BUZZER_TARGET_NOT_FOUND);
	}
	return rv;
}


void intel_board::robot_test_mbed()
{
	uint8_t state = 0;
	this->motion_controller->reset_lifter();
	while(true)
	{
		switch(state)
		{
			case 0:
				this->motion_controller->move(DEFAULT_DIS,CAR_FORWARD);
				state++;
			break;
			case 1:
				this->motion_controller->move(DEFAULT_DIS,CAR_BACKWARD);
				state++;
			break;
			case 2:
				this->motion_controller->move(DEFAULT_DIS,CAR_RIGHT);
				state++;
			break;
			case 3:
				this->motion_controller->move(DEFAULT_DIS,CAR_LEFT);
				state++;
			break;
			case 4:
				//this->motion_controller->set_lifter(0);
				state++;
			break;
			case 5:
				//this->motion_controller->set_lifter(LIFTER_INIT_POS);
				state++;
			break;
			case 6:
				this->motion_controller->rotate(30,0);
				state = 0;
			break;
		}
	}
	return ;
}
