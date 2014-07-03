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

#include "buzzer.h"

extern "C" void mbed_reset();
 
Buzzer::Buzzer(MyDigitalOut* buzzer)
{
    this->_buzzer = buzzer;
    ON();
    wait(0.1);
    OFF();
    wait(0.1);
    ON();
    wait(0.1);
    OFF();
    wait(0.1);
    ON();
    wait(0.2);
    OFF();
}

Buzzer::~Buzzer(){}

void Buzzer::ON()
{
    *_buzzer = 0;
}

void Buzzer::OFF()
{
    *_buzzer = 1;
}

void Buzzer::setFlag()
{
    flag=1;
}

void Buzzer::cleanFlag()
{
    flag=0;
}

void Buzzer::check_time_out()
{
    if(flag == 1)
    {
        ON();
        wait(5);
        mbed_reset();
    }
    else
    {
        OFF();
    }
}

void Buzzer::time_out_init()
{
    setFlag();
    time_out.detach();
    time_out.attach(this, &Buzzer::check_time_out, TIME_OUT);
}
