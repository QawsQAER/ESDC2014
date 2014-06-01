#ifndef _UI_H
#define _UI_H
#include "../macro.h"




#define UI_PORT 60000
#define MAX_MESSAGE_SIZE 255
#define MESSAGELENGTH 2



class UI{

	public:

	//establish connection when class created
	UI();

	//close connection when class destroyed
	~UI();


	//use this when 
	int wait_command();
	/*

	send_finished_ack  fa

	connect request cr  1
	start movement  sm  2 
	confirm picture cp  3 

	pattern 1 p1  4
	pattern 2 p2  5
	pattern 3 p3  6
	pattern 4 p4  7

	car 				 
	forward  Up	 8
	backward Do	 9
	left     Le	 10
	right    Ri    11 

	camera 				 
	forward   cU	 12
	backward  cD	 13
	left      cL	 14
	right     cR	 15

	lift 				
	up lu         16
	down ld		 17*/



	//use this when the car finish moving by itself
	void send_finished_ack();





	private:


	char msg_code[MESSAGELENGTH];
	char content[MESSAGELENGTH];
	char tempBuffer[MAX_MESSAGE_SIZE];


	int server_socket;
	int client_sd;




	int init_server_socket();
	int read_msg();
	void send_msg();

	void contention();

	void send_established();
	void send_start_ack();
	void send_comfrim_ack();

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

};




















#endif
/*********************************************************/
