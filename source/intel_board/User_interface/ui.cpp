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
unsigned short PROT_USED;
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


protacal from board to  phone
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

finish

*/

char msg_code[MESSAGELENGTH];
char content[MESSAGELENGTH];
char tempBuffer[MESSAGELENGTH];

int client_sd;
struct sockaddr_in client_addr;

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


int main(int argc, char** argv){
	
	 //use usr provided prot or not
		
		if(argc==2){
			PROT_USED=atoi(argv[1]);
		}else{
			PROT_USED=PORT;
		}

		
	int server_socket = init_server_socket();

	int val=1;
	if(setsockopt(server_socket,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(long))==-1){
		perror("setsockopt");
		exit(1);
	}

				


	

		int addr_len=sizeof(client_addr);

				printf("before accept client\n");

	
		if((client_sd=accept(server_socket,(struct sockaddr *) &client_addr,&addr_len))<0)
		{
			printf("accept erro: %s (Errno:%d)\n",strerror(errno),errno);
			exit(0);
		}
		printf("after accept client\n");
	
				
	  			 	msg_code[0]=0x03;
					msg_code[1]=0x00;
					msg_code[2]=0x03;
					msg_code[3]=0x00;
					msg_code[4]=0x03;
					msg_code[5]=0x00;
					msg_code[6]=0x03;
					msg_code[7]=0x00;  			
					int i ;
					for (i= 0; i < 10; ++i)
					{
							send_msg();
					}

					read_msg();
				  			 

				


	close(server_socket);
	return 0;
}


void send_msg()
{
			printf("Send msg to client\n");
					memset(msg_code,0,MESSAGELENGTH);
					
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




void read_msg()
{
			printf("Receive msg from client\n");

	memset(tempBuffer,0,MESSAGELENGTH);
	memset(content,0,MESSAGELENGTH);
	
	int receiveByte=0;
	int alreadyReceiveByte=0;

	// receive hello message



	while(1){
		if((receiveByte = recv(client_sd,tempBuffer+alreadyReceiveByte,MAX_MESSAGE_SIZE-alreadyReceiveByte,0))<0){
			 printf("Error: Couldn't receive\n");
			// exit(0);
		}

		alreadyReceiveByte+=receiveByte;

		if(1)
		{/*! = 0x21*/
			if(alreadyReceiveByte>=MESSAGELENGTH)
			{
				memcpy(&content,tempBuffer,sizeof(char)*MESSAGELENGTH);
				printf("%s\n",content);
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
	}



}



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
	server_addr.sin_port=htons(PROT_USED);
	
	if(bind(sd,(struct sockaddr *) &server_addr,sizeof(server_addr))<0){
		printf("bind error: %s (Errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
	
	if(listen(sd,2)<0){
		printf("listen error: %s (Errno:%d)\n",strerror(errno),errno);
		exit(0);
	}
		


	printf("**Server Standby** PORT:%d\n",PROT_USED);
	return sd;

}