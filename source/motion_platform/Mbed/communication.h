/**********************************************************
This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
**********************************************************/

/*
The communication protocol is inside source/intel_board/lib/message.h
*/
#include "lifter.h"
#include "camera_platform.h"
#include "mbed.h"

#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#define BUFFER_SIZE 1024
#define STARTER 0x7e

class Communication
{
public:
    Communication(MySerial* _DEBUG, MySerial *_IntelToMbed, MySerial *_MbedToArduino);
    ~Communication();

    void putToBuffer(uint8_t _x);
    uint8_t getByte();
    uint16_t get2Bytes();
    void putByte(uint8_t _x, uint8_t _i); //_i = 0 is _DEBUG, _i = 1 is _IntelToMbed, _i = 2 is _MbedToArduino
    void put2Bytes(uint16_t _x, uint8_t _i);
    void parseMessage();
    void forwardMessage(); //forward the message to the Arduino

    uint8_t getInfoOK();
    void resetInfoOK();
    uint16_t getMoveDis();
    uint16_t getRotateDis();
    uint8_t getMoveDir();
    uint8_t getRotateDir();

private:
    void init();
    uint8_t* buffer;
    uint16_t in;
    uint16_t out;
    uint8_t state;
    uint8_t check_sum;
    uint8_t info_ok; //0 is not ok, 1 is car, 2 is lifter and 3 is camera platform

    uint8_t action_type;
    uint16_t move_dis;
    uint8_t move_dir;
    uint16_t rotate_dis;
    uint8_t rotate_dir;
    
    MySerial *_DEBUG;
    MySerial *_IntelToMbed;
    MySerial *_MbedToArduino;
};

#endif