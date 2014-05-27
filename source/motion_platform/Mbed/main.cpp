/**********************************************************
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
        if(com.getInfoOK() == 1) //car
        {
            com.resetInfoOK();
            com.forwardMessage();
        }
        else if(com.getInfoOK() == 2) //lifter
        {
            com.resetInfoOK();
            lifter.lifterMove(com.getMoveDis(), com.getMoveDir(), com.getRotateDis(), com.getRotateDir());
        }
        else if(com.getInfoOK() == 3) //camera_platform
        {
            com.resetInfoOK();
        }
    }
}