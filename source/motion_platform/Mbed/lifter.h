/**********************************************************
This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
This program is running on Mbed Platform 'mbed LPC1768' avaliable in 'http://mbed.org'.
**********************************************************/
#include "mbed.h"

#ifndef _LIFTER_H
#define _LIFTER_H

/*
DigitalOut lifter_enable(p23);
PwmOut lifter_pwm1(p21);
PwmOut lifter_pwm2(p22);
DigitalIn lifter_encoder_A(p26);
DigitalIn lifter_encoder_B(p25);
*/

//100 plus => 8 mm
#define RPMM 13 //plusCount per mm

class Lifter
{
public:
    Lifter(DigitalOut* _enable, PwmOut* _pwm1, PwmOut* _pwm2, InterruptIn* encoder_A, DigitalIn* encoder_B);
    void lifterUp(uint16_t mm);
    void lifterDown(uint16_t mm);
    void lifterMove(uint16_t move_dis, uint8_t move_dir, uint16_t rotate_dis, uint8_t rotate_dir);
    uint8_t getDir();
    
    uint32_t plusCount;
    uint32_t targetPlusCount;
    
    void setLifterStop();
    void setLifterUp();
    void setLifterDown();
    
private:
    DigitalOut* _enable;
    PwmOut* _pwm1;
    PwmOut* _pwm2;
    InterruptIn* _encoder_A; //6 plus per round
    DigitalIn* _encoder_B; //5v is up, 0v is down. looks like no use
    uint8_t _dir;
};

#endif