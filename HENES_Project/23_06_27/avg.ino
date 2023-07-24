#include<MsTimer2.h>

#define alpha 0.3

float old_avg = 0.01;     // xavg(k-1)
float avg     = 0.0;      // xavg(k)
int ad_value  = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  MsTimer2::set(100, TimerADC);
  MsTimer2::start();
}

void TimerADC()
{
  ad_value = analogRead(A0);

  avg = alpha * old_avg + (1.0 - alpha) * ad_value;
  old_avg = avg;

  Serial.print(ad_value);
  Serial.print(" ");

  //Serial.print("avg   = ");
  Serial.println(avg);
  
}

void loop() {

}
