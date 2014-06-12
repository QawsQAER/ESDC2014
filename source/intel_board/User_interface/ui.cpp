#include "ui.h"

#include <string>
#include <iostream>
#include <cstdio>


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <pthread.h>





/*

protacal from phone to board

type operation

connect request cr  1
start movement  sm  2 
confirm picture cp  3 

pattern 1 p1  4
pattern 2 p2  5
pattern 3 p3  6
pattern 4 p4  7

car 				 
forward  fw	 8
backward bw	 9
left     lw	 10
right    rw    11 


camera 				 
forward   up	 12
backward  do	 13
left      le	 14
right     ri	 15


lift 				
up lu         16
down ld		 17

-------------------------------------

protacal from board to  phone

connect established 0x00 0x01


pattern 1 ack p1
pattern 2 ack p2
pattern 3 ack p3 
pattern 4 ack p4

car
forward	ack	  Up
backward ack  Do
left ack 	  Le
right ack     Ri


camera
forward	ack	  cU
backward ack  cD
left ack 	  cL
right ack     cR


lift
up ack		  lu
down ack      ld

send_finished_ack  fa

*/


UI::UI()
{
	pattern=undefined;
	contention();
}



UI::~UI()
{
	close(server_socket);
}



void UI::contention()
{
	
	 server_socket = init_server_socket();

	int val=1;
	if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(long))==-1){
		perror("setsockopt");
		exit(1);
	}

				


	struct sockaddr_in client_addr;

		socklen_t addr_len=sizeof(struct sockaddr_in);

				// printf("before accept client\n");

	
		if((client_sd=accept(server_socket,(struct sockaddr *) &client_addr,&addr_len))<0)
		{
			printf("accept erro: %s (Errno:%d)\n",strerror(errno),errno);
			exit(0);
		}

		// printf("after accept client\n");
	wait_command();
}			


command_type UI::wait_command()
{
		int read_type=read_msg();

		switch(read_type){
			case -1: 			
			printf("Error: Received undefined message from other client.\n");
			return undefined;

			case 1:
			printf("Command: connect request\n");
			send_established();
			return connect_request;

			case 2:
			printf("Command: start movement\n");
			send_start_ack();
			return start_movement;

			case 3:
			printf("Command: confirm picture\n");
			send_comfrim_ack();
			return confirm_picture;


			case 4:
			//use pattern1
			printf("Command:  pattern1\n");
			send_pattern1_ack();
			return pattern_1;


			case 5:
			printf("Command: pattern 2\n");
			send_pattern2_ack();
			return pattern_2;

			case 6:
			printf("Command: pattern3\n");
			send_pattern3_ack();
			return pattern_3;

			case 7:
			printf("Command: pattern4\n");
			send_pattern4_ack();
			return pattern_4;

			case 8:
			printf("Command: car forward\n");
			send_car_forward_ack();
			return car_forward;

			case 9:
			printf("Command: car backward\n");
			send_car_backward_ack();
			return car_backward;

			case 10:
			printf("Command: car left\n");
			send_car_left_ack();
			return car_left;

			case 11:
			printf("Command: car right\n");
			send_car_right_ack();
			return car_right;

			case 12:
			printf("Command: camera up\n");
			send_camera_forward_ack();
			return camera_forward;

			case 13:
			printf("Command: camera down\n");
			send_camera_backward_ack();
			return camera_backward;

			case 14:
			printf("Command: camera left\n");
			send_camera_left_ack();
			return camera_left;

			case 15:
			printf("Command: camera right\n");
			send_camera_right_ack();
			return camera_right;

			case 16:
			printf("Command: lift up\n");
			send_lift_up_ack();
			return lift_up;

			case 17:
			printf("Command: lift down\n");
			send_lift_down_ack();
			return lift_down;


			default:
			printf("Error: undefined read_type.\n");
			return undefined;


		}

}


	





void UI::send_msg()
{
			// printf("Send msg to client\n");
					
					int already_sent=0;
					while(already_sent<MESSAGELENGTH){
						int len=send(client_sd,msg_code+already_sent,MESSAGELENGTH-already_sent,0);
						if(len<=0){
							perror("send()");
						}else{
							already_sent+=len;
						}
						if(already_sent>=MESSAGELENGTH){
							break;
						}
					}
}


void UI::send_finished_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="fa";
	memcpy(&msg_code,&temp,2*sizeof(char));	
	printf("send_finished_ack\n");
	send_msg();
}



void UI::send_established()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="cr";
	memcpy(&msg_code,&temp,2*sizeof(char));	
	 
	// printf("send content:%s\n",msg_code);
	send_msg();
}


void UI::send_start_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="sm";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}


void UI::send_comfrim_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="cp";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}




void UI::send_pattern1_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="p1";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}

void UI::send_pattern2_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="p2";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}

void UI::send_pattern3_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="p3";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}


void UI::send_pattern4_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="p4";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}

void UI::send_car_forward_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="Up";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}

void UI::send_car_backward_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="Do";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}

void UI::send_car_left_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="Le";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}


void UI::send_car_right_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="Ri";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}


void UI::send_camera_forward_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="cU";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}

void UI::send_camera_backward_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="cD";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}

void UI::send_camera_left_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="cL";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}


void UI::send_camera_right_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="cR";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}

void UI::send_lift_up_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="lu";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}


void UI::send_lift_down_ack()
{
	memset(msg_code,0,MESSAGELENGTH);
	char temp[]="ld";
	memcpy(&msg_code,&temp,2*sizeof(char));	

	send_msg();
}











int UI::read_msg()
{
	printf("Receive msg from client\n");

	memset(tempBuffer,0,MAX_MESSAGE_SIZE);
	memset(content,0,MESSAGELENGTH);
	
	int receiveByte=0;
	int alreadyReceiveByte=0;

	// receive hello message



	while(alreadyReceiveByte<MESSAGELENGTH){


		if((receiveByte = recv(client_sd,tempBuffer+alreadyReceiveByte,MAX_MESSAGE_SIZE-alreadyReceiveByte,0))<0)
		{
			 printf("Error: Couldn't receive\n");
			// exit(0);
		}

		alreadyReceiveByte+=receiveByte;

		
			if(alreadyReceiveByte>=MESSAGELENGTH)
			{
				memcpy(&content,tempBuffer,sizeof(char)*MESSAGELENGTH);
				// printf("%s\n",content);
				 


				if(strcmp(content,"crcr")==0)
					return 1;
				else if (strcmp(content,"smsm")==0)
					return 2;
				else if (strcmp(content,"cpcp")==0)
					return 3;
				else if (strcmp(content,"p1p1")==0)
					return 4;
				else if (strcmp(content,"p2p2")==0)
					return 5;
				else if (strcmp(content,"p3p3")==0)
					return 6;
				else if (strcmp(content,"p4p4")==0)
					return 7;
				else if (strcmp(content,"UpUp")==0)
					return 8;
				else if (strcmp(content,"DoDo")==0)
					return 9;
				else if (strcmp(content,"LeLe")==0)
					return 10;
				else if (strcmp(content,"RiRi")==0)
					return 11;
				else if (strcmp(content,"cUcU")==0)
					return 12;
				else if (strcmp(content,"cDcD")==0)
					return 13;
				else if (strcmp(content,"cLcL")==0)
					return 14;
				else if (strcmp(content,"cRcR")==0)
					return 15;
				else if (strcmp(content,"lulu")==0)
					return 16;
				else if (strcmp(content,"ldld")==0)
					return 17;
				else
					return -1;


				/*switch (content[0]){

				case 0x00:
				if(content[1]==0x00)
					return 1;
				else if(content[1]==0x02)
					return 2;
				else if(content[1]==0x03)
					return 3;
				else 
					return -1;
				break;

				case 0x01:
				if(content[1]==0x01)
					return 4;
				else if(content[1]==0x02)
					return 5;
				else if(content[1]==0x03)
					return 6;
				else if(content[1]==0x04)
					return 7;
				else 
					return -1;
				break;



				case 0x02:
				if(content[1]==0x01)
					return 8;
				else if(content[1]==0x02)
					return 9;
				else if(content[1]==0x03)
					return 10;
				else if(content[1]==0x04)
					return 11;
				else 
					return -1;
				break;


				case 0x03:
				if(content[1]==0x01)
					return 12;
				else if(content[1]==0x02)
					return 13;
				else if(content[1]==0x03)
					return 14;
				else if(content[1]==0x04)
					return 15;
				else 
					return -1;
				break;


				case 0x04:
				if(content[1]==0x01)
					return 16;
				else if(content[1]==0x02)
					return 17;
				else 
					return -1;
				break;

				default:
				return -1;


				}*/

			}
			else{continue;}
	}



}










int UI::	init_server_socket(){

	int sd=socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in server_addr;

	long val=1;
	if(setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(long))==-1){
		perror("setsockopt");
		exit(1);
	}
	memset(&server_addr,0,sizeof(server_addr));

	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	server_addr.sin_port=htons(UI_PORT);
	
	if(bind(sd,(struct sockaddr *) &server_addr,sizeof(server_addr))<0){
		printf("bind error: %s (Errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	if(listen(sd,2)<0){
		printf("listen error: %s (Errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
		


	printf("**Server Standby** PORT:%d\n",UI_PORT);
	return sd;

}