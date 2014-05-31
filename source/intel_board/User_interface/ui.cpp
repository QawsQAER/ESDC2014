#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <pthread.h>


#define PORT 60000
#define false 0
#define true 1
#define MAX_MESSAGE_SIZE 255
#define MESSAGELENGTH 2

/*

protacal from phone to board

type operation

connect request 0x00 0x00
start movement  0x00 0x02
confirm picture 0x00 0x03

pattern 1 0x01 0x01  
pattern 2 0x01 0x02
pattern 3 0x01 0x03  
pattern 4 0x01 0x04

car 
forward 0x02 0x01
backward 0x02 0x02
left     0x02 0x03
right    0x02 0x04


camera 
forward 0x03 0x01
backward 0x03 0x02
left     0x03 0x03
right    0x03 0x04

lift
up 0x04 0x01
down 0x04 0x02

connect established 0x00 0x01


pattern 1 ack 0x05 0x01  
pattern 2 ack 0x05 0x02
pattern 3 ack 0x05 0x03  
pattern 4 ack 0x05 0x04

car
forward	ack	  0x05 0x05
backward ack  0x05 0x06
left ack 	  0x05 0x07
right ack     0x05 0x08


camera
forward	ack	  0x05 0x09
backward ack  0x05 0x0a
left ack 	  0x05 0x0b
right ack     0x05 0x0c


lift
up ack		  0x05 0x0d
down ack      0x05 0x0e


*/

char msg_code[MESSAGELENGTH];
char content[MESSAGELENGTH];
<<<<<<< HEAD
<<<<<<< HEAD
char tempBuffer[MESSAGELENGTH];
=======
char tempBuffer[MAX_MESSAGE_SIZE];
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
char tempBuffer[MAX_MESSAGE_SIZE];
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9

int client_sd;
struct sockaddr_in client_addr;

<<<<<<< HEAD
<<<<<<< HEAD
typedef struct Client {
	unsigned int IP_addr;
	unsigned short Port_num;
	unsigned short Listening_Port_num;
	int client_sd;
}Client;


Client client_inf;


int init_server_socket();
void send_msg();
void read_msg();
=======
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9

int init_server_socket();
void send_msg();
int read_msg();







void send_established();
void send_finished_ack();
void send_pattern1_ack();
void send_pattern2_ack();
void send_pattern3_ack();
void send_pattern4_ack();
void send_car_forward_ack();
void send_car_backward_ack();
void send_car_left_ack();
void send_car_right_ack();
void send_camera_forward_ack();
void send_camera_backward_ack();
void send_camera_left_ack();
void send_camera_right_ack();
void send_lift_up_ack();
void send_lift_down_ack();







<<<<<<< HEAD
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9


int main(int argc, char** argv){
	
<<<<<<< HEAD
<<<<<<< HEAD
	 //use usr provided prot or not
		
		if(argc==2){
			PROT_USED=atoi(argv[1]);
		}else{
			PROT_USED=PORT;
		}

		
=======
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
	int server_socket = init_server_socket();

	int val=1;
	if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(long))==-1){
		perror("setsockopt");
		exit(1);
	}

				


	

<<<<<<< HEAD
<<<<<<< HEAD
		int addr_len=sizeof(client_addr);
=======
		socklen_t addr_len=sizeof(client_addr);
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
		socklen_t addr_len=sizeof(client_addr);
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9

				printf("before accept client\n");

	
		if((client_sd=accept(server_socket,(struct sockaddr *) &client_addr,&addr_len))<0)
		{
			printf("accept erro: %s (Errno:%d)\n",strerror(errno),errno);
			exit(0);
		}
<<<<<<< HEAD
<<<<<<< HEAD
		printf("after accept client\n");
	
				
	  			 	msg_code[0]=0x03;
					msg_code[1]=0x00;
					msg_code[2]=0x03;
					msg_code[3]=0x00;
					msg_code[4]=0x03;
					msg_code[5]=0x00;
					msg_code[6]=0x03;
					msg_code[7]=0x00;  			
=======
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9

		printf("after accept client\n");
	
				
	  			 
<<<<<<< HEAD
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
					int i ;
					for (i= 0; i < 10; ++i)
					{
							send_msg();
					}

<<<<<<< HEAD
<<<<<<< HEAD
					read_msg();
				  			 
=======
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
		while(1)
		{


		int read_type=read_msg();
		if(read_type==-1)
		{
			printf("Error: Received undefined message from other client.\n");
			// exit(0);
		}
				 


		}

		 			 
<<<<<<< HEAD
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9

				


	close(server_socket);
	return 0;
}


<<<<<<< HEAD
<<<<<<< HEAD
void send_msg()
{
			printf("Send msg to client\n");
					memset(msg_code,0,MESSAGELENGTH);
=======


void send_msg()
{
			printf("Send msg to client\n");
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======


void send_msg()
{
			printf("Send msg to client\n");
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
					
					int already_sent=0;
					while(1){
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




<<<<<<< HEAD
<<<<<<< HEAD
void read_msg()
{
			printf("Receive msg from client\n");

	memset(tempBuffer,0,MESSAGELENGTH);
=======
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
void send_established()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x00;
msg_code[1]=0x01;
send_msg();
}

void send_finished_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x00;
msg_code[1]=0x04;
send_msg();
}


void send_pattern1_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x01;
send_msg();
}

void send_pattern2_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x02;
send_msg();
}

void send_pattern3_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x03;
send_msg();
}


void send_pattern4_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x04;
send_msg();
}

void send_car_forward_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x05;
send_msg();
}

void send_car_backward_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x06;
send_msg();
}

void send_car_left_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x07;
send_msg();
}


void send_car_right_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x08;
send_msg();
}


void send_camera_forward_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x09;
send_msg();
}

void send_camera_backward_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x0a;
send_msg();
}

void send_camera_left_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x0b;
send_msg();
}


void send_camera_right_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x0c;
send_msg();
}

void send_lift_up_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x0d;
send_msg();
}


void send_lift_down_ack()
{
memset(msg_code,0,MESSAGELENGTH);
msg_code[0]=0x05;
msg_code[1]=0x0e;
send_msg();
}











int read_msg()
{
	printf("Receive msg from client\n");

	memset(tempBuffer,0,MAX_MESSAGE_SIZE);
<<<<<<< HEAD
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
	memset(content,0,MESSAGELENGTH);
	
	int receiveByte=0;
	int alreadyReceiveByte=0;

	// receive hello message



<<<<<<< HEAD
<<<<<<< HEAD
	while(1){
		if((receiveByte = recv(client_sd,tempBuffer+alreadyReceiveByte,MAX_MESSAGE_SIZE-alreadyReceiveByte,0))<0){
=======
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
	while(alreadyReceiveByte<MESSAGELENGTH){


		if((receiveByte = recv(client_sd,tempBuffer+alreadyReceiveByte,MAX_MESSAGE_SIZE-alreadyReceiveByte,0))<0)
		{
<<<<<<< HEAD
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
			 printf("Error: Couldn't receive\n");
			// exit(0);
		}

		alreadyReceiveByte+=receiveByte;

<<<<<<< HEAD
<<<<<<< HEAD
		if(1)
		{/*! = 0x21*/
=======
		
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
		
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
			if(alreadyReceiveByte>=MESSAGELENGTH)
			{
				memcpy(&content,tempBuffer,sizeof(char)*MESSAGELENGTH);
				printf("%s\n",content);
<<<<<<< HEAD
<<<<<<< HEAD
				 receiveByte=0;
				alreadyReceiveByte = 0;
				// content=ntohl(content);
			}
			else{continue;}
		
		}
		else
		{
			printf("Error: Received undefined message from other client.\n");
			exit(0);
		}
=======
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
				 


				switch (content[0]){

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


				}

			}
			else{continue;}
<<<<<<< HEAD
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
	}



}



<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9







<<<<<<< HEAD
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
int init_server_socket(){

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
<<<<<<< HEAD
<<<<<<< HEAD
	server_addr.sin_port=htons(PROT_USED);
=======
	server_addr.sin_port=htons(PORT);
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
	server_addr.sin_port=htons(PORT);
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
	
	if(bind(sd,(struct sockaddr *) &server_addr,sizeof(server_addr))<0){
		printf("bind error: %s (Errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	if(listen(sd,2)<0){
		printf("listen error: %s (Errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
		


<<<<<<< HEAD
<<<<<<< HEAD
	printf("**Server Standby** PORT:%d\n",PROT_USED);
=======
	printf("**Server Standby** PORT:%d\n",PORT);
>>>>>>> 832ff13e7ed61c990077dccbdf7673e773ceac7e
=======
	printf("**Server Standby** PORT:%d\n",PORT);
>>>>>>> 3af6513febf7f52551889e07a264a5f01d3aedf9
	return sd;

}
