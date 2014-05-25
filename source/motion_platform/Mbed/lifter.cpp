/**********************************************************
This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
This program is running on Mbed Platform 'mbed LPC1768' avaliable in 'http://mbed.org'.
**********************************************************/
#include "lifter.h"

Lifter::Lifter(DigitalOut* _enable, PwmOut* _pwm1, PwmOut* _pwm2, InterruptIn* _encoder_A, DigitalIn* _encoder_B)
{
    plusCount = 0;
    targetPlusCount = 0;
    _dir = 0;
    
    this->_enable = _enable;
    this->_pwm1 = _pwm1;
    this->_pwm2 = _pwm2;
    this->_encoder_A = _encoder_A;
    this->_encoder_B = _encoder_B;
    
    setLifterStop();
}

void Lifter::lifterUp(uint16_t mm)
{
    targetPlusCount = mm * RPMM;
    //targetPlusCount = mm;
    setLifterUp();
}

void Lifter::lifterDown(uint16_t mm)
{
    targetPlusCount = mm * RPMM;
    //targetPlusCount = mm;
    setLifterDown();
}

uint8_t Lifter::getDir()
{
    /*
    if(*_encoder_B == 1) //up
    {
        return true;
    }
    else //down
    {
        return false;
    }
    */
    return _dir;
}

void Lifter::lifterMove(uint16_t move_dis, uint8_t move_dir, uint16_t rotate_dis, uint8_t rotate_dir)
{
    plusCount = targetPlusCount = 0;
    
    if(_dir = move_dir == 0x00) //up
    {
        lifterUp(move_dis);
    }
    else if(_dir = move_dir == 0x02) //down
    {
        lifterDown(move_dis);
    }
}

void Lifter::setLifterStop()
{
    *_enable = 0;
    *_pwm1 = 1.0f;
    *_pwm2 = 1.0f;
}
void Lifter::setLifterUp()
{
    *_enable = 0;
    *_pwm1 = 1.0f;
    *_pwm2 = 0.0f;
}
void Lifter::setLifterDown()
{
    *_enable = 0;
    *_pwm1 = 0.0f;
    *_pwm2 = 1.0f;
}
