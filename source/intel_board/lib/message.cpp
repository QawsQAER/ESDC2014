#include "message.h"


//public
Message::Message()
{
	this->_IntelCarCmd = (struct IntelCarCmd *) malloc(sizeof(struct IntelCarCmd));
}

Message::~Message()
{
	free(_IntelCarCmd);
}

void Message::sendMessage(int fd)
{
	int write_error = 0;
	uint8_t move_dis_upper_8 = (_IntelCarCmd->move_dis) >> 8;
	uint8_t move_dis_lower_8 = (_IntelCarCmd->move_dis) & 0x00ff;
	uint8_t rotate_dis_upper_8 = (_IntelCarCmd->rotate_dis) >> 8;
	uint8_t rotate_dis_lower_8 = (_IntelCarCmd->rotate_dis) & 0x00ff;

	write_error += write(fd, (void *) &(_IntelCarCmd->starter),sizeof(uint8_t));
	write_error += write(fd, (void *) &(_IntelCarCmd->action_type),sizeof(uint8_t));
	write_error += write(fd, (void *) &(move_dis_upper_8),sizeof(uint8_t));
	write_error += write(fd, (void *) &(move_dis_lower_8),sizeof(uint8_t));
	write_error += write(fd, (void *) &(_IntelCarCmd->move_dir),sizeof(uint8_t));
	write_error += write(fd, (void *) &(rotate_dis_upper_8),sizeof(uint8_t));
	write_error += write(fd, (void *) &(rotate_dis_lower_8),sizeof(uint8_t));
	write_error += write(fd, (void *) &(_IntelCarCmd->rotate_dir),sizeof(uint8_t));
	write_error += write(fd, (void *) &(_IntelCarCmd->check_sum),sizeof(uint8_t));

	if(write_error != 9)
	{
		printf("Message::sendMessage(int fd) error: error happened when writing bytes to the file descriptor!\n");
	}
}

void Message::CarMoveUpMM(uint16_t _mm)
{
	uint8_t move_dir = 0x00;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void Message::CarMoveDownMM(uint16_t _mm)
{
	uint8_t move_dir = 0x02;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void Message::CarMoveLeftMM(uint16_t _mm)
{
	uint8_t move_dir = 0x03;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void Message::CarMoveRightMM(uint16_t _mm)
{
	uint8_t move_dir = 0x01;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void Message::CarRotateLeftDegree(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc2;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void Message::CarRotateRightDegree(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc3;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void Message::LifterMoveUpMM(uint16_t _mm)
{
	uint8_t move_dir = 0x00;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void Message::LifterMoveDowmMM(uint16_t _mm)
{
	uint8_t move_dir = 0x02;
	uint16_t move_dis = _mm;
	setCarMove(move_dir, move_dis);
}
void Message::CameraPlatformRollLeft(uint16_t _degree)
{
	uint8_t rotate_dir = 0xe0;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformRollRight(uint16_t _degree)
{
	uint8_t rotate_dir = 0xf0;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformPitchUp(uint16_t _degree)
{
	uint8_t rotate_dir = 0xcc;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformPitchDown(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc8;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformYawClk(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc3;
	uint16_t rotate_dis = _degree * 100;
	setCarRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformYawCounterClk(uint16_t _degree)
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
	_IntelCarCmd->rotate_dir = 0xc0;//1100 0000
	calCheckSum();
}
void Message::setCarRotate(uint8_t rotate_dir, uint16_t rotate_dis)
{
	resetStruct();
	_IntelCarCmd->action_type = CAR_ACTION;
	_IntelCarCmd->rotate_dis = rotate_dis;
	_IntelCarCmd->rotate_dir = rotate_dir;
	calCheckSum();
}
void Message::setLifterMove(uint8_t move_dir, uint16_t move_dis)
{
	resetStruct();
	_IntelCarCmd->action_type = LIFTER_ACTION;
	_IntelCarCmd->move_dis = move_dis;
	_IntelCarCmd->move_dir = move_dir;
	calCheckSum();
}
void Message::setCameraPlatformRotate(uint8_t rotate_dir, uint16_t rotate_dis)
{
	resetStruct();
	_IntelCarCmd->action_type = CAM_PLATFORM_ACTION;
	_IntelCarCmd->rotate_dis = rotate_dis;
	_IntelCarCmd->rotate_dir = rotate_dir;
	calCheckSum();
}
void Message::calCheckSum()
{
	_IntelCarCmd->check_sum = _IntelCarCmd->action_type + (_IntelCarCmd->move_dis >> 8)
				+ (_IntelCarCmd->move_dis & 0x00ff) + _IntelCarCmd->move_dir + (_IntelCarCmd->rotate_dis >> 8)
				+ (_IntelCarCmd->rotate_dis & 0x00ff) + _IntelCarCmd->rotate_dir;

	printf("%x ", _IntelCarCmd->action_type);
	printf("%x ", _IntelCarCmd->move_dis >> 8);
	printf("%x ", _IntelCarCmd->move_dis & 0x00ff);
	printf("%x ", _IntelCarCmd->move_dir);
	printf("%x ", _IntelCarCmd->rotate_dis >> 8);
	printf("%x ", _IntelCarCmd->rotate_dis & 0x00ff);
	printf("%x ", _IntelCarCmd->rotate_dir);
	printf("%x ", _IntelCarCmd->check_sum);
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
