#include "message.h"


//public
Message::Message()
{
	this->_IntelCarCmd = (struct IntelCarCmd *) malloc(sizeof(struct IntelCarCmd));
}

Message::~Message()
{
	delete _IntelCarCmd;
}

void CarMoveUpMM(uint16_t _mm)
{
	uint8_t move_dir = 0x00;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void CarMoveDownMM(uint16_t _mm)
{
	uint8_t move_dir = 0x02;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void CarMoveLeftMM(uint16_t _mm)
{
	uint8_t move_dir = 0x03;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void CarMoveRightMM(uint16_t _mm)
{
	uint8_t move_dir = 0x01;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void CarRotateLeftDegree(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc2;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void CarRotateRightDegree(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc3;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void LifterMoveUpMM(uint16_t _mm)
{
	uint8_t move_dir = 0x00;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void LifterMoveDowmMM(uint16_t _mm)
{
	uint8_t move_dir = 0x02;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void CameraPlatformRollLeft(uint16_t _degree)
{
	uint8_t rotate_dir = 0xe0;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void CameraPlatformRollRight(uint16_t _degree)
{
	uint8_t rotate_dir = 0xf0;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void CameraPlatformPitchUp(uint16_t _degree)
{
	uint8_t rotate_dir = 0xcc;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void CameraPlatformPitchDown(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc8;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void CameraPlatformYawClk(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc3;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void CameraPlatformYawCounterClk(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc2;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}

//private
void Message::setCarMove(uint8_t move_dir, uint16_t move_dis)
{
	resetStruct();
	_IntelCarCmd->action_type = CAR_ACTION;
	_IntelCarCmd->move_dis = move_dis;
	_IntelCarCmd->move_dir = move_dir;
	calCheckSum(CAR_ACTION, move_dir, move_dis);
	sendMessage();
}
void Message::setCarRotate(uint8_t rotate_dir, uint16_t rotate_dis)
{
	resetStruct();
	_IntelCarCmd->action_type = CAR_ACTION;
	_IntelCarCmd->rotate_dis = rotate_dis;
	_IntelCarCmd->rotate_dir = rotate_dir;
	calCheckSum(CAR_ACTION, rotate_dir, rotate_dis);
	sendMessage();
}
void Message::setLifterMove(uint8_t move_dir, uint16_t move_dis)
{
	resetStruct();
	_IntelCarCmd->action_type = LIFTER_ACTION;
	_IntelCarCmd->move_dis = move_dis;
	_IntelCarCmd->move_dir = move_dir;
	calCheckSum(LIFTER_ACTION, move_dir, move_dis);
	sendMessage();
}
void Message::setCameraPlatformRotate(uint8_t rotate_dir, uint16_t rotate_dis)
{
	resetStruct();
	_IntelCarCmd->action_type = CAM_PLATFORM_ACTION;
	_IntelCarCmd->rotate_dis = rotate_dis;
	_IntelCarCmd->rotate_dir = rotate_dir;
	calCheckSum(CAM_PLATFORM_ACTION, rotate_dir, rotate_dis);
	sendMessage();
}
void Message::sendMessage()
{

}
void Message::calCheckSum(uint8_t action_type, uint8_t _dir, uint16_t _dis)
{
	_IntelCarCmd->check_sum = action_type + (_dis >> 8) + (_dis & 0xff) + _dir;
}
void Message::resetStruct()
{
	_IntelCarCmd->starter = STARTER;
	_IntelCarCmd->action_type = 0x00;
	_IntelCarCmd->move_dis = 0x0000;
	_IntelCarCmd->move_dir = 0x00;
	_IntelCarCmd->rotate_dis = 0x0000;
	_IntelCarCmd->rotate_dir = 0x00;
	_IntelCarCmd->check_sum = 0x00;
}