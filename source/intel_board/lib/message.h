#ifndef _MESSAGE_H
#define _MESSAGE_H

#define CAR_ACTION 0
#define LIFTER_ACTION 1
#define CAM_PLATFORM_ACTION 2

#define STARTER 0x7E

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct IntelCarCmd
{
	/*
	starter indicates the start of the message
	*/
	uint8_t starter;

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
	ratate_dir = (MSB)XXXXXXXX(LSB).
	when action_type = 1, all bits are 0s.
	when action_type = 0 or 2, first 2 bits are 1s.

	when third bit is 1 and fourth bit is 0, means roll left. when second bit is 1, means roll right.
	when fiveth bit is 1 and sixth bit is 0, means pitch down. when second bit is 1, means pitch up.
	when sixth bit is 1 and last bit is 0, means yaw counterclockwise. when second bit is 1, means yaw clockwise.
	*/
	uint8_t rotate_dir;

	/*
	check_sum = action_type + (move_dis >> 8) + (move_dis & 0xff) + move_dir + (rotate_dis >> 8) + (rotate_dis & 0xff) + rotate_dir
	*/
	uint8_t check_sum;
};

class Message
{
public:
	Message();
	~Message();
	void CarMoveUpMM(uint16_t _mm);
	void CarMoveDownMM(uint16_t _mm);
	void CarMoveLeftMM(uint16_t _mm);
	void CarMoveRightMM(uint16_t _mm);

	void CarRotateLeftDegree(uint16_t _degree);
	void CarRotateRightDegree(uint16_t _degree);

	void LifterMoveUpMM(uint16_t _mm);
	void LifterMoveDowmMM(uint16_t _mm);

	void CameraPlatformRollLeft(uint16_t _degree);
	void CameraPlatformRollRight(uint16_t _degree);
	void CameraPlatformPitchUp(uint16_t _degree);
	void CameraPlatformPitchDown(uint16_t _degree);
	void CameraPlatformYawClk(uint16_t _degree);
	void CameraPlatformYawCounterClk(uint16_t _degree);

private:
	struct IntelCarCmd* _IntelCarCmd;

	void setCarMove(uint8_t move_dir, uint16_t move_dis);
	void setCarRotate(uint8_t rotate_dir, uint16_t rotate_dis);
	void setLifterMove(uint8_t move_dir, uint16_t move_dis);
	void setCameraPlatformRotate(uint8_t rotate_dir, uint16_t rotate_dis);

	void sendMessage();
	void calCheckSum(uint8_t action_type, uint8_t _dir, uint16_t _dis);
	void resetStruct();
};

#endif
