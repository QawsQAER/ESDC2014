/******************************************************

***┏┓           ┏┓
**┏┛┻━━━━━━━━━━━┛┻┓
**┃               ┃
**┃      ━━━      ┃
**┃  ┳┛       ┗┳  ┃
**┃               ┃
**┃ '''   ┻   ''' ┃
**┃               ┃
**┗━━━┓       ┏━━━┛
******┃       ┃
******┃       ┃
******┃       ┃
******┃       ┗━━━━━━━━━━━━┓
******┃                    ┃━━┓
******┃      NO BUG        ┏━━┛
******┃                    ┃
******┗━┓  ┓  ┏━━━━┏━━┓  ━━┛
********┃  ┛  ┛    ┃  ┛  ┛
********┃  ┃  ┃    ┃  ┃  ┃
********┗━━┛━━┛    ┗━━┛━━┛

This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
******************************************************/
#include "message.h"
#define DEBUG_MODE 1

//public
Message::Message()
{
	this->_IntelCarCmd = (struct IntelCarCmd*) malloc(sizeof(struct IntelCarCmd));
	this->_ACK = (struct ACK*) malloc(sizeof(struct ACK));
}

Message::~Message()
{
	free(_IntelCarCmd);
	free(_ACK);
}

void Message::sendMessage(int fd)
{
	printf("Message::sending Message\n");
	int write_error = 0;
	uint8_t move_dis_upper_8 = (_IntelCarCmd->move_dis) >> 8;
	uint8_t move_dis_lower_8 = (_IntelCarCmd->move_dis) & 0x00ff;
	uint8_t rotate_dis_upper_8 = (_IntelCarCmd->rotate_dis) >> 8;
	uint8_t rotate_dis_lower_8 = (_IntelCarCmd->rotate_dis) & 0x00ff;

	write_error += write(fd, (void *) &(_IntelCarCmd->starter), sizeof(uint8_t));
	write_error += write(fd, (void *) &(_IntelCarCmd->action_type), sizeof(uint8_t));
	write_error += write(fd, (void *) &(move_dis_upper_8), sizeof(uint8_t));
	write_error += write(fd, (void *) &(move_dis_lower_8), sizeof(uint8_t));
	write_error += write(fd, (void *) &(_IntelCarCmd->move_dir), sizeof(uint8_t));
	write_error += write(fd, (void *) &(rotate_dis_upper_8), sizeof(uint8_t));
	write_error += write(fd, (void *) &(rotate_dis_lower_8), sizeof(uint8_t));
	write_error += write(fd, (void *) &(_IntelCarCmd->rotate_dir), sizeof(uint8_t));
	write_error += write(fd, (void *) &(_IntelCarCmd->check_sum), sizeof(uint8_t));

	if(write_error != 9)
	{
		printf("void Message::sendMessage(int fd) error: error happened when writing bytes to the file descriptor!\n");
		return;
	}

	receiveACK(fd);
	printf("Message::sendMessage() exiting\n");
}

int Message::receiveACK(int fd)
{
	printf("Message::receiveACK() entering.\n");
	memset(_ACK, 0 , sizeof(struct ACK));
	uint8_t buff[4];
	uint8_t read_num = 0;

	//printf("Message::receiveACK() looping.\n");
	while(read_num < sizeof(struct ACK))
	{
		int temp = read(fd, buff + read_num, sizeof(struct ACK));

		//printf("Message::receiveACK() temp = %d.\n", temp);
		
		if(temp == -1)
		{
			printf("int Message::receiveACK(int fd) error: error happened when reading bytes from the file descriptor!\n");
			return -1;
		}

		read_num += temp;
		printf("while loop\n");
	}

	if(DEBUG_MODE)
	{
		for(int i = 0; i < 4; i++)
		{
			printf("%x ", buff[i]);
		}

			printf("\nMessage::receiveACK() exiting loop.\n");

	}
	


	
	_ACK->starter = buff[0];
	_ACK->O = buff[1];
	_ACK->K = buff[2];
	_ACK->check_sum = buff[3];

	if(DEBUG_MODE)printf("Message::receiveACK() exiting assignment.\n");

	if(_ACK->starter == STARTER && _ACK->O == 0x4f && _ACK->K == 0x4b && _ACK->check_sum == 0x9a)
	{
		if(DEBUG_MODE)printf("Message::receiveACK() returning 1.\n");
		return 1;
	}
	else if(_ACK->starter == STARTER )
	{
		uint16_t temp_degree;
		memcpy((void *)&temp_degree,&_ACK->O,1);
		memcpy((void *)&temp_degree+1,&_ACK->K,1);
		this->car_degree= temp_degree;
		return 1;
	}
	printf("int Message::receiveACK(int fd) error: ACK is invalid!\n");
	return 0;
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
	setLifterMove(move_dir, move_dis);
}
void Message::LifterMoveDownMM(uint16_t _mm)
{
	uint8_t move_dir = 0x02;
	uint16_t move_dis = _mm;
	setLifterMove(move_dir, move_dis);
}
void Message::CameraPlatformRollLeft(uint16_t _degree)
{
	uint8_t rotate_dir = 0xe0;
	uint16_t rotate_dis = _degree * 100;
	setCameraPlatformRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformRollRight(uint16_t _degree)
{
	uint8_t rotate_dir = 0xf0;
	uint16_t rotate_dis = _degree * 100;
	setCameraPlatformRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformPitchUp(uint16_t _degree)
{
	uint8_t rotate_dir = 0xcc;
	uint16_t rotate_dis = _degree * 100;
	setCameraPlatformRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformPitchDown(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc8;
	uint16_t rotate_dis = _degree * 100;
	setCameraPlatformRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformYawClk(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc3;
	uint16_t rotate_dis = _degree * 100;
	setCameraPlatformRotate(rotate_dir, rotate_dis);
}
void Message::CameraPlatformYawCounterClk(uint16_t _degree)
{
	uint8_t rotate_dir = 0xc2;
	uint16_t rotate_dis = _degree * 100;
	setCameraPlatformRotate(rotate_dir, rotate_dis);
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



void Message::CompassRequest()
{
	resetStruct();
	_IntelCarCmd->action_type = COMPASS_REQUEST;
	calCheckSum();
}