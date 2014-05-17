#ifndef _MESSAGE_H
#define _MESSAGE_H

#define CAR_ACTION_DISPLACEMENT 0
#define CAR_ACTION_ROTATION 1
#define CAR_ACTION_CAM_MOVEMENT 2
struct IntelCarCommand
{
	/*
	0 indicates car movement, without rotation
	1 indicates car rotation
	2 indicates car camera movement
	*/
	uint8_t action_type;
	
	// move distance 
	/*
	only meaningful when action_type = 0 or action_type = 2,
	when action_type = 2, it means the displacement of the camera height
	*/
	uint8_t move_dis; 
	
	/*
	this field only meaningful when action_type = 0 or action_type = 1
	(degree * 100,say, if you are rotating 36.5, the rotate_degree would be 365)
	*/
	uint8_t deg; 
	
	/*
	this field only meaningful when action_type = 1
	rotate clockwise or counter-clockwise, 1 for clockwise, 0 for counter-clockwise
	*/
	uint8_t rotate_dir;
};

#endif