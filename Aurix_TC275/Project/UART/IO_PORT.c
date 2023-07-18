#include "IfxPort.h"

#define port1         &MODULE_P23,0
#define port2         &MODULE_P23,2
#define port3         &MODULE_P22,0
#define port4         &MODULE_P22,2
#define port5         &MODULE_P22,3
#define port6         &MODULE_P21,3
#define port7         &MODULE_P21,5
#define port8         &MODULE_P20,1

#define port1_2       &MODULE_P00,0
#define port2_2       &MODULE_P00,1
#define port3_2       &MODULE_P00,2
#define port4_2       &MODULE_P00,3
#define port5_2       &MODULE_P00,5
#define port6_2       &MODULE_P00,6
#define port7_2       &MODULE_P00,7
#define port8_2       &MODULE_P00,8

//#define LED_1         &MODULE_P00,5
//#define LED_2         &MODULE_P00,6
//#define BUTTON        &MODULE_P00,7
//#define WAIT_TIME     500

int v = 0;
int plus = 0;
int add[8]={0,};
int add_plus[8]={0,};


void repeat_data()
{
    for(int i=0; i<8; i++)
    {
        add[i] = 0;
        add_plus[i] = 0;
    }
}
void pin_pull_up(void)
{
    IfxPort_setPinMode(port1_2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(port2_2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(port3_2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(port4_2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(port5_2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(port6_2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(port7_2, IfxPort_Mode_outputPushPullGeneral);
    IfxPort_setPinMode(port8_2, IfxPort_Mode_outputPushPullGeneral);

    IfxPort_setPinState(port1_2, IfxPort_State_low);
    IfxPort_setPinState(port2_2, IfxPort_State_low);
    IfxPort_setPinState(port3_2, IfxPort_State_low);
    IfxPort_setPinState(port4_2, IfxPort_State_low);
    IfxPort_setPinState(port5_2, IfxPort_State_low);
    IfxPort_setPinState(port6_2, IfxPort_State_low);
    IfxPort_setPinState(port7_2, IfxPort_State_low);
    IfxPort_setPinState(port8_2, IfxPort_State_low);
    //repeat_data();
}
void pin_pull_down(void)
{
    IfxPort_setPinMode(port1, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(port2, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(port3, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(port4, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(port5, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(port6, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(port7, IfxPort_Mode_inputPullUp);
    IfxPort_setPinMode(port8, IfxPort_Mode_inputPullUp);
    IfxPort_setPinState(port1, IfxPort_State_high);
    IfxPort_setPinState(port2, IfxPort_State_high);
    IfxPort_setPinState(port3, IfxPort_State_high);
    IfxPort_setPinState(port4, IfxPort_State_high);
    IfxPort_setPinState(port5, IfxPort_State_high);
    IfxPort_setPinState(port6, IfxPort_State_high);
    IfxPort_setPinState(port7, IfxPort_State_high);
    IfxPort_setPinState(port8, IfxPort_State_high);
    //repeat_data();
}

void out_putt(int v)
{
    if(v >= 128){ add[7] = 1; v -= 128;}
        else{ add[7] = 0;}
    if(v >= 64){ add[6] = 1; v -= 64;}
        else{ add[6] = 0;}
    if(v >= 32){ add[5] = 1; v -= 32;}
        else{ add[5] = 0;}
    if(v >= 16){ add[4] = 1; v -= 16;}
        else{ add[4] = 0;}
    if(v >= 8){ add[3] = 1; v -= 8;}
        else{ add[3] = 0;}
    if(v >= 4){ add[2] = 1; v -= 4;}
        else{ add[2] = 0;}
    if(v >= 2){ add[1] = 1; v -= 2;}
        else{ add[1] = 0;}
    if(v >= 1){ add[0] = 1; v -= 1;}
        else{ add[0] = 0;}

    if(add[0] == 1){IfxPort_setPinState(port1_2, IfxPort_State_high);}
        else{IfxPort_setPinState(port1_2, IfxPort_State_low);}
    if(add[1] == 1){IfxPort_setPinState(port2_2, IfxPort_State_high);}
        else{IfxPort_setPinState(port2_2, IfxPort_State_low);}
    if(add[2] == 1){IfxPort_setPinState(port3_2, IfxPort_State_high);}
        else{IfxPort_setPinState(port3_2, IfxPort_State_low);}
    if(add[3] == 1){IfxPort_setPinState(port4_2, IfxPort_State_high);}
        else{IfxPort_setPinState(port4_2, IfxPort_State_low);}
    if(add[4] == 1){IfxPort_setPinState(port5_2, IfxPort_State_high);}
        else{IfxPort_setPinState(port5_2, IfxPort_State_low);}
    if(add[5] == 1){IfxPort_setPinState(port6_2, IfxPort_State_high);}
        else{IfxPort_setPinState(port6_2, IfxPort_State_low);}
    if(add[6] == 1){IfxPort_setPinState(port7_2, IfxPort_State_high);}
        else{IfxPort_setPinState(port7_2, IfxPort_State_low);}
    if(add[7] == 1){IfxPort_setPinState(port8_2, IfxPort_State_high);}
        else{IfxPort_setPinState(port8_2, IfxPort_State_low);}
}



int in_putt(uint8 GHI)
{
  if(IfxPort_getPinState(port1)==0) { add_plus[0]=1;}
    else{add_plus[0]=0;}
  if(IfxPort_getPinState(port2)==0) { add_plus[1]=2;}
    else{add_plus[1]=0;}
  if(IfxPort_getPinState(port3)==0) { add_plus[2]=4;}
    else{add_plus[2]=0;}
  if(IfxPort_getPinState(port4)==0) { add_plus[3]=8;}
    else{add_plus[3]=0;}
  if(IfxPort_getPinState(port5)==0) { add_plus[4]=16;}
    else{add_plus[4]=0;}
  if(IfxPort_getPinState(port6)==0) { add_plus[5]=32;}
    else{add_plus[5]=0;}
  if(IfxPort_getPinState(port7)==0) { add_plus[6]=64;}
    else{add_plus[6]=0;}
  if(IfxPort_getPinState(port8)==0) { add_plus[7]=128;}
    else{add_plus[7]=0;}

  for(int i=0; i<8; i++)
  {
      plus += add_plus[i];
  }
   return plus;
}
void push_button(void)
{
    if(add[0] == 1){IfxPort_setPinState(port1, IfxPort_State_high);}
        else{IfxPort_setPinState(port1, IfxPort_State_low);}
    if(add[1] == 1){IfxPort_setPinState(port2, IfxPort_State_high);}
        else{IfxPort_setPinState(port2, IfxPort_State_low);}
    if(add[2] == 1){IfxPort_setPinState(port3, IfxPort_State_high);}
        else{IfxPort_setPinState(port3, IfxPort_State_low);}
    if(add[3] == 1){IfxPort_setPinState(port4, IfxPort_State_high);}
        else{IfxPort_setPinState(port4, IfxPort_State_low);}
    if(add[4] == 1){IfxPort_setPinState(port5, IfxPort_State_high);}
        else{IfxPort_setPinState(port5, IfxPort_State_low);}
    if(add[5] == 1){IfxPort_setPinState(port6, IfxPort_State_high);}
        else{IfxPort_setPinState(port6, IfxPort_State_low);}
    if(add[6] == 1){IfxPort_setPinState(port7, IfxPort_State_high);}
        else{IfxPort_setPinState(port7, IfxPort_State_low);}
    if(add[7] == 1){IfxPort_setPinState(port8, IfxPort_State_high);}
        else{IfxPort_setPinState(port8, IfxPort_State_low);}
}
