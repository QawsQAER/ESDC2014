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

#include <iostream>
#include "intel_board/intel_board.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
using namespace std;

void exit_routine(int32_t arg);
void generate_dir();
void print_usage_and_exit();

char *glo_PATH_TEMP = NULL;
cv::Rect glo_prev_face;
unsigned char continuity = 1;
int source_mode;
uint8_t glo_multi_target = 0;
uint8_t glo_num_target = 1;
uint8_t glo_test_mbed = 0;
int32_t glo_argc;

intel_board *robot;

int main(int32_t argc, char ** argv) 
{
	/* prints Hello World */
	glo_argc = argc;
	if(argc<2)
	{
		print_usage_and_exit();
	}

	/*use phone or canon*/
	source_mode=atoi(argv[1]);

	cout << "Hello Intel ESDC" << endl;
	/*
	   mode, default value MANUAL MODE
	   img_source, default value using Cellphone 
	*/
	uint8_t mode = 1;
	uint8_t img_source = 0;
	glo_PATH_TEMP = (char *) malloc(sizeof(char) * FILENAME_LENGTH);
	
	generate_dir();
	signal(SIGTERM,exit_routine);
	signal(SIGINT,exit_routine);
	if(argc >= 3)//the user has set the mode
	{
		mode = atoi(argv[2]);
		printf("mode is %u\n",mode);
		if(mode == 2)
		{
			printf("The robot is going to launch as mbed debug mode\n");
			glo_test_mbed = 1;
		}
		else if(mode == 3)
		{
			glo_multi_target = 1;
			if(argc < 4)
				print_usage_and_exit();
			else
				glo_num_target = atoi(argv[3]);
		}
    }
    else // the user has not set the mode
    	printf("The robot is going to initiate in default mode\n");


    robot = new intel_board(mode,img_source);
    robot->main_function();
	exit_routine(0);
	return 0;
}

void exit_routine(int arg)
{
        printf("\n\n\n\nExecuting the pre-registered exit routine\n");
        delete robot;
        free(glo_PATH_TEMP);
        exit(0);
}

void generate_dir()
{
	printf("generate_dir running\n");
	
	char *filename =(char *) malloc(sizeof(char) * FILENAME_LENGTH);
	time_t timestamp = time(NULL);
	struct tm *current_time = gmtime(&timestamp);
	//get the filename in format of month-day_hour:minute:second
	sprintf(filename,"/home/intelcup/Desktop/%d_%d_%d_%d_%d",
		current_time->tm_mon,
		current_time->tm_mday,
		current_time->tm_hour,
		current_time->tm_min,
		current_time->tm_sec);
	strcpy(glo_PATH_TEMP,filename);
	free(filename);
	printf("creating dir named as %s\n",glo_PATH_TEMP);
	if(mkdir(glo_PATH_TEMP,S_IRWXU) < 0)
	{
		printf("fail to create dir %s\n",glo_PATH_TEMP);
		exit(0);
	}
	strcat(glo_PATH_TEMP,"/");
}

void print_usage_and_exit()
{
	printf("Usage: 1./ESDC 0 [mode]for PHONE\n");
	printf("Usage: 2./ESDC 1 [mode]for CANON\n");
	printf("[mode] = 1 or non specified -> normal mode\n");
	printf("[mode] = 2 -> mbed debugging\n");
	printf("[mode] = 3 -> multi targets, please indicates target number in the 3th arguemment\n");
	exit_routine(glo_argc);
}