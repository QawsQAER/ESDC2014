#ifndef _UI_H
#define _UI_H
#include "../macro.h"




#define UI_PORT 60000
#define MAX_MESSAGE_SIZE 255
#define MESSAGELENGTH 2



class UI{

public:
	UI();
	~UI();


void contention();
void send_msg();
int wait_command();








private:


char msg_code[MESSAGELENGTH];
char content[MESSAGELENGTH];
char tempBuffer[MAX_MESSAGE_SIZE];


int server_socket;
int client_sd;


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
int init_server_socket();
int read_msg();


};






















#endif
/*********************************************************/
