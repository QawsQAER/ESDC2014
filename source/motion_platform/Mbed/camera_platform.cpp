/**********************************************************
This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
This program is running on Mbed Platform 'mbed LPC1768' avaliable in 'http://mbed.org'.
**********************************************************/
#include "camera_platform.h"

Camera_platform::Camera_platform(MyPwmOut* _pwmRoll, MyPwmOut* _pwmPitch, MyPwmOut* _pwmYaw)
{
    this->_pwmRoll = _pwmRoll;
    this->_pwmPitch = _pwmPitch;
    this->_pwmYaw = _pwmYaw;
}

Camera_platform::~Camera_platform()
{
    delete _pwmRoll;
    delete _pwmPitch;
    delete _pwmYaw;
}