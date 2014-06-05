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

void exit_routine(int arg);
char *dir_path;
intel_board *robot;

int main(int argc, char ** argv) 
{
        /* prints Hello World */
        cout << "Hello Intel ESDC" << endl; 
        //mode default value MANUAL MODE
        uint8_t mode = 1;
        //img_source default using Cellphone
        uint8_t img_source = 0;
        signal(SIGTERM,exit_routine);
        signal(SIGINT,exit_routine);
        if(argc >= 2)
        {
        	//the user has set the mode
        	mode = atoi(argv[1]);
                //if the mode is image processing mode
                switch(mode)
                {
                        case 0://case for auto mode
                                printf("The robot is going to initiate in default mode\n");
                        break;
                        
                        case 1://case for manual mode
                                //if the user has specify the img_source
                                if(argc >= 3)
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
                }
        }
        else
                printf("The robot is going to initiate in default mode\n");


        robot = new intel_board(mode,img_source);
        robot->main_function();

        free(dir_path);
        return 0;
}

void exit_routine(int arg)
{
        printf("\n\n\n\nExecuting the pre-registered exit routine\n");
        delete robot;
        free(dir_path);
        exit(-1);
}