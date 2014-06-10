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

#define ROLL_MID 1500
#define ROLL_LOW 1000
#define ROLL_HIGH 2000
#define ROLL_ANGLE_MAX 90
#define ROLL_ANGLE_MIN -90
#define ROLL_USPD (2000-1500)/180

#define PITCH_MID 1500
#define PITCH_LOW 1300
#define PITCH_HIGH 2200
#define PITCH_ANGLE_MAX 90
#define PITCH_ANGLE_MIN -25
#define PITCH_USPD (2200-1300)/115

#define YAW_MID 1400
#define YAW_LOW 600
#define YAW_HIGH 2200
#define YAW_ANGLE_MAX 90
#define YAW_ANGLE_MIN -90
#define YAW_USPD (2200-600)/180 //us per degree => 500/90

#define ROLL 0
#define PITCH 1
#define YAW 2

class Camera_platform
{
public:
    Camera_platform(MyPwmOut* _pwmRoll, MyPwmOut* _pwmPitch, MyPwmOut* _pwmYaw);
    ~Camera_platform();
    
    void cameraPlatformMove(uint16_t move_dis, uint8_t move_dir, uint16_t rotate_dis, uint8_t rotate_dir);
    
    void setRollLeft(float _degree);
    void setRollRight(float _degree);
    void setPitchUp(float _degree);
    void setPitchDown(float _degree);
    void setYawClock(float _degree);
    void setYawCClock(float _degree);
    
    void resetCameraPlatform();
    
    uint8_t dir;
    uint8_t angle;
    
private:
    MyPwmOut* _pwmRoll;
    MyPwmOut* _pwmPitch;
    MyPwmOut* _pwmYaw;
    
    float _roll_angle; //record the current roll angle. need to divide 100 to convert to degree
    float _pitch_angle; //record the current pitch angle. need to divide 100 to convert to degree
    float _yaw_angle; //record the current yaw angle. need to divide 100 to convert to degree
    
    void setPWM(uint16_t _pwm_value_us, uint8_t _pwm_channel); //0 is roll, 1 is pitch, 2 is yaw
    uint16_t computePwmValue(float _degree, uint8_t _dir, uint8_t _pwm_channel); //0 is left/up/clock, 1 is right/down/cclock
};

#endif