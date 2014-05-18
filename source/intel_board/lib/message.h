#ifndef _MESSAGE_H
#define _MESSAGE_H

#define CAR_ACTION 0
#define LIFTER_ACTION 1
#define CAM_PLATFORM_ACTION 2

#define STARTER 0x7E

struct IntelCarCmd
{
	/*
	starter indicates the start of the message
	*/
	uint16_t starter;

	/*
	0 indicates car movement, with rotation
	1 indicates lifter movement. only up and down
	2 indicates camera platform movement, with roll/pitch/yaw
	*/
	uint8_t action_type;
	 
	/*
	move distance
	only meaningful when action_type = 0 or action_type = 1.
    distance is measured in mm.
	*/
	uint16_t move_dis;
    
	/*
	move direction
	only meaningful when action_type = 0 or action_type = 1.
    for car, directions are up/right/down/left w.r.t 0/1/2/3
    for lifter, directions are up/down w.r.t 0/2
    */
	uint8_t move_dir;
	
	/*
	rotate angle
	this field only meaningful when action_type = 0 or action_type = 2.
	(degree * 100,say, if you are rotating 36.5, the rotate_degree would be 3650)
	*/
	uint16_t rotate_dis;
	
	/*
	rotate direction
	this field only meaningful when action_type = 0 or action_type = 2.
	ratate_dir = (MSB)XXXX(LSB).
	when action_type = 1, first bit(MSB) is 0.
	when action_type = 0 or 2, first bit(MSB) is 1.
	when second bit is 0, means roll left. when second bit is 1, means roll right.
	when third bit is 0, means pitch down. when second bit is 1, means pitch up.
	when last bit is 0, means yaw counterclockwise. when second bit is 1, means yaw clockwise.
	*/
	uint8_t rotate_dir;

	/*
	check_sum = action_type + (move_dis >> 8) + (move_dis | 0x07) + move_dir + (rotate_dis >> 8) + (rotate_dis | 0x07) + rotate_dir
	*/
	uint8_t check_sum;
};


//position
struct pos
{
	uint8_t x; //mm, forward
	uint8_t y; //mm, rightward
	uint8_t z; //mm, upward
};

//orientation
struct ori 
{
	uint8_t roll; //degree * 100 (0-->36000), along x axis
	uint8_t pitch; //degree * 100 (0-->36000), along y axis
	uint8_t yaw; //degree * 100 (0-->36000), along z axis
};
#endif