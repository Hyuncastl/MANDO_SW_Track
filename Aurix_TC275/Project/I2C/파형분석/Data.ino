//slave
#include <Wire.h>

#define SLAVE 2
volatile char data_1 = 64;
volatile int  data_1_master = 0;
volatile char data_2 = 128;
volatile int  data_2_master = 0;
volatile char data_3 = 0;
volatile int  data_3_master = 0;
volatile char data_4 = 2;
volatile int  data_4_master = 0;


void setup()
{
  Wire.begin(SLAVE);
  Serial.begin(115200);


 Wire.onRequest(requestEvent);
 Wire.onReceive(receiveEvent);
}


void loop ()
{
 

}


void requestEvent()
{
  Wire.write(data_1);
  Wire.write(data_2);
  Wire.write(data_3);
  Wire.write(data_4);
}
void receiveEvent()
{
   data_1_master = Wire.read();
   data_2_master = Wire.read();
   data_3_master = Wire.read();
   data_4_master = Wire.read();

   // Serial.println(first_data_master); // 마스터로부터 수신된 데이터 출력
}
