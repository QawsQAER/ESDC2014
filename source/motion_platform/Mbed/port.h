/**********************************************************
This part is added by project ESDC2014 of CUHK team.
All the code with this header are under GPL open source license.
This program is running on Mbed Platform 'mbed LPC1768' avaliable in 'http://mbed.org'.
**********************************************************/
#include "mbed.h"

#ifndef _PORT_H
#define _PORT_H

DigitalOut IntelToMbed_LED(LED1); //uart port LED between Intel Board and Mbed
DigitalOut MbedToArduino_LED(LED2); //uart port LED between Mbed and Arduino
Serial DEBUG(USBTX, USBRX); //usb serial port between computer and Mbed
Serial IntelToMbed(p13, p14); //uart port between Intel Board and Mbed
Serial MbedToArduino(p28, p27); //uart port between Mbed and Arduino
PwmOut lifter_pwm1(p21);
PwmOut lifter_pwm2(p22);
DigitalOut lifter_enable(p23);
InterruptIn lifter_encoder_A(p18);
DigitalIn lifter_encoder_B(p17);
//PwmOut camera_platform_pwm1(p24);
//PwmOut camera_platform_pwm2(p25);

Communication com(&DEBUG, &IntelToMbed, &MbedToArduino);
Lifter lifter(&lifter_enable, &lifter_pwm1, &lifter_pwm2, &lifter_encoder_A, &lifter_encoder_B);

void IntelToMbedRxHandler()
{
    //__disable_irq();//disable interupt when receiving data from XBEE_UART
    uint8_t _x = IntelToMbed.getc();
    com.putToBuffer(_x); //function inside Communication::
    //__enable_irq();
}

void LifterPlusHandler()
{
    if(lifter.plusCount < lifter.targetPlusCount)
    {
        lifter.plusCount++;
        if(lifter.getDir() == 0) //up
        {
            lifter.setLifterUp();
        }
        else if(lifter.getDir() == 2)//down
        {
            lifter.setLifterDown();
        }
    }
    else
    {
        lifter.targetPlusCount = 0;
        lifter.plusCount = 0;
        lifter.setLifterStop();
    }
}

void init_PORT() //used in main() function
{
    DEBUG.baud(9600);
    
    IntelToMbed.baud(9600);
    IntelToMbed.attach(&IntelToMbedRxHandler); //serial interrupt function
    
    MbedToArduino.baud(9600);
    
    lifter_encoder_A.fall(&LifterPlusHandler); //interrupt
}

#endif