/******************************************************

****┏┓          ┏┓
**┏┛┻━━━━━━┛┻┓
**┃                 ┃
**┃      ━━━      ┃
**┃  ┳┛       ┗┳ ┃
**┃                 ┃
**┃ '''    ┻   ''' ┃
**┃                 ┃
**┗━━┓       ┏━━┛
*******┃       ┃
*******┃       ┃
*******┃       ┃
*******┃       ┗━━━━━━━━┓
*******┃                      ┃━┓
*******┃      NO BUG          ┏━┛
*******┃                      ┃
*******┗━┓  ┓  ┏━┏━┓  ━┛
***********┃  ┛  ┛    ┃  ┛  ┛
***********┃  ┃  ┃    ┃  ┃  ┃
***********┗━┛━┛     ┗━┛━┛

This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
This program is running on Mbed Platform 'mbed LPC1768' avaliable in 'http://mbed.org'.
**********************************************************/
#include "mbed.h"
#include "define.h"

#ifndef _CAMERA_PLATFORM_H
#define _CAMERA_PLATFORM_H

/*
PwmOut camera_platform_pwmRoll(p24);
PwmOut camera_platform_pwmPitch(p26);
PwmOut camera_platform_pwmYaw(p25);
*/

class Camera_platform
{
public:
    Camera_platform(MyPwmOut* _pwmRoll, MyPwmOut* _pwmPitch, MyPwmOut* _pwmYaw);
    ~Camera_platform();
    
private:
    MyPwmOut* _pwmRoll;
    MyPwmOut* _pwmPitch;
    MyPwmOut* _pwmYaw;
};

#endif