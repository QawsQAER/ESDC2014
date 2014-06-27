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

void generate_dir();
void exit_routine(int arg);
char *dir_path;
char *PATH_TEMP = NULL;
unsigned char continuity = 1;



intel_board *robot;

int main(int argc, char ** argv) 
{
	/* prints Hello World */

	if(argc!=2)
	{
		printf("Usage: 1./ESDC 0 [mode]for PHONE\n");
		printf("Usage: 2./ESDC 1 [mode]for CANON\n");
		exit(0);
	}

	/*use phone or canon*/
	source_mode_temp=argv[1];

	cout << "Hello Intel ESDC" << endl;
	/*
	   mode, default value MANUAL MODE
	   img_source, default value using Cellphone 
	*/
	uint8_t mode = 1;
	uint8_t img_source = 0;
	PATH_TEMP = (char *) malloc(sizeof(char) * FILENAME_LENGTH);
	generate_dir();
	signal(SIGTERM,exit_routine);
	signal(SIGINT,exit_routine);
	if(argc >= 3)//the user has set the mode
	{
		mode = atoi(argv[2]);
		switch(mode)
		{
			case 0://case for auto mode
				printf("The robot is going to initiate in default mode\n");
			break;
	
			case 1://case for manual mode
				if(argc >= 3)//if the user has specify the img_source
					img_source = atoi(argv[2]);
			break;

			case 2://case for image processing mode
				if(argc != 3)
				{
					printf("Please specify the directory for image processing\n");
					exit(-1);
				}
				else
				{
					dir_path = (char *) malloc(sizeof(char) * FILENAME_LENGTH);
					strcpy(dir_path,argv[2]);
				}
				break;
			default://a undefined mode
				printf("Invalid mode specified by the user\n");
				exit(-1);
			break; 
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
        free(dir_path);
        free(PATH_TEMP);
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
	strcpy(PATH_TEMP,filename);
	free(filename);
	printf("creating dir named as %s\n",PATH_TEMP);
	if(mkdir(PATH_TEMP,S_IRWXU) < 0)
	{
		printf("fail to create dir %s\n",PATH_TEMP);
		exit(0);
	}
	strcat(PATH_TEMP,"/");
}
