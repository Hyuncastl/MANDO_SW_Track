#include "IfxPort.h"
#include "Bsp.h"

#define port1         &MODULE_P00,2 
#define port2         &MODULE_P00,6 
#define port3         &MODULE_P00,8

#define WAIT_TIME     5 

int a=0;

void port_cycle(void)
{
    IfxPort_setPinMode(port1, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(port2, IfxPort_Mode_outputPushPullGeneral); 
    IfxPort_setPinMode(port3, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinHigh(port1);
    IfxPort_setPinHigh(port2);
    IfxPort_setPinHigh(port3);
}


void blinkLED(void)
{
    a++;
    waitTime(IfxStm_getTicksFromMilliseconds(BSP_DEFAULT_TIMER, WAIT_TIME));
    if(a%1==0){IfxPort_togglePin(port1);}
    if(a%2==0){IfxPort_togglePin(port2);}
    if(a%4==0){IfxPort_togglePin(port3);}                                                    /* Toggle the state of the LED      */
    
}
