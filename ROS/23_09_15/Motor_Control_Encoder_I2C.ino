/////////////////////// DC Motor Control /////////////////////
#include "src/PinChangeInt.h"
volatile long EncoderPos = 0;
#define MOTOR_PWM 5 
#define MOTOR_DIR 4
#define Encoder_Pin_A1      2                       // Quadrature encoder A pin
#define Encoder_Pin_B1      3                       // Quadrature encoder B pin

void Interrupt_Setup(void)
{
  pinMode(Encoder_Pin_A1, INPUT_PULLUP);              // quadrature encoder input A
  pinMode(Encoder_Pin_B1, INPUT_PULLUP);              // quadrature encoder input B
  attachInterrupt(0, Encoder, FALLING);               // update encoder position
  TCCR1B = TCCR1B & 0b11111000 | 1;                   // To prevent Motor Noise  
}

void Encoder(void)  
{                                                                     // pulse and direction, direct port reading to save cycles  
  //Serial.println("interrupt");
  if(digitalRead(Encoder_Pin_B1)==HIGH)    EncoderPos++;             // if(digitalRead(encodPinB1)==HIGH)   count --;
  else                                     EncoderPos--;             // if(digitalRead(encodPinB1)==LOW)   count ++;
}

/////////////////////// DC Motor Control /////////////////////
/////////////////////////// I2C  //////////////////////////
unsigned  char a[7];
#include <Wire.h>
int sensor_flag = 0;

void ReceiveEvent(int howMany)
{
  Serial.println("receive");
    // '#' +   '*'  //
  a[0] = Wire.read();
  a[1] = Wire.read();
  a[2] = Wire.read();
  a[3] = Wire.read();
  a[4] = Wire.read();
  a[5] = Wire.read();
  a[6] = Wire.read();
}

/////////////////////////// I2C //////////////////////////
void RequestEvent() 
{
  unsigned char s[8] = {0,};
  int temp;
  temp = EncoderPos*10;
  s[0]='#';
  s[1]= (temp&0xff00)>>8;
  s[2]= (temp&0x00ff); 
  s[3]= (EncoderPos&0xff000000)>>24;    // encoder MSB 8bit
  s[4]= (EncoderPos&0x00ff0000)>>16;
  s[5]= (EncoderPos&0x0000ff00)>>8;
  s[6]= (EncoderPos&0x000000ff);        // encoder LSB 8bit
  s[7]='*'; 
  
  Wire.write(s,8); // respond 
  sensor_flag = 0;
  Serial.println("send");
}

void Encoder_Value(int byteCount)
{
  if(byteCount ==8){
      for(int i=0;i<8;i++){
        a[i]=Wire.read();
        Serial.print(a[i]);
        Serial.print(" ");
      }
  Serial.println("\nreceive");
  }
}

/////////////////////////// I2C //////////////////////////
/////////////////////////// Setup //////////////////////////
void setup()
{
  Interrupt_Setup();
  Wire.begin(0x05);
  Wire.onRequest(RequestEvent);             // register events
  Wire.onReceive(Encoder_Value);
  Serial.begin(115200);
  delay(2000);
}

/////////////////////////// Setup //////////////////////////
/////////////////////////// Loop //////////////////////////
void loop()
{
  Serial.println(EncoderPos);
  delay(100);
}

/////////////////////////// Loop //////////////////////////
