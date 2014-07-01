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
#include "communication.h"
#include "port.h"

int main()
{
    init_PORT();
  
    while(1)
    {
        com.parseMessage();
        if(com.getInfoOK(0) == 1) //car
        {
            buzzer.buzzerON();
            com.forwardMessage();
            com.ACK(&lifter, &camera_platform);
            com.resetInfoOK(0);
            com.resetInfoOK(1);
            buzzer.buzzerOFF();
        }
        else if(com.getInfoOK(0) == 2) //lifter
        {
            buzzer.buzzerON();
            lifter.lifterMove(com.getMoveDis(), com.getMoveDir(), com.getRotateDis(), com.getRotateDir());
            com.ACK(&lifter, &camera_platform);
            com.resetInfoOK(0);
            com.resetInfoOK(1);
            buzzer.buzzerOFF();
        }
        else if(com.getInfoOK(0) == 3) //camera_platform
        {
            buzzer.buzzerON();
            camera_platform.cameraPlatformMove(com.getMoveDis(), com.getMoveDir(), com.getRotateDis(), com.getRotateDir());
            com.ACK(&lifter, &camera_platform);
            com.resetInfoOK(0);
            com.resetInfoOK(1);
            buzzer.buzzerOFF();
        }
        else if(com.getInfoOK(0) == 4) //compass
        {
            buzzer.buzzerON();
            com.ACK(&lifter, &camera_platform);
            com.resetInfoOK(0);
            com.resetInfoOK(1);
            buzzer.buzzerOFF();
        }
    }
}