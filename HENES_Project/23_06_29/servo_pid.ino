#define alpha 0.38
#define max_val 970//max 1024 safe 960
#define min_val 180//min 64   safe 190
#include <MsTimer2.h>
#define MOTOR3_PWM 8
#define MOTOR3_ENA 9
#define MOTOR3_ENB 10

float old_avg = 0.0; // Xavg(k-1)
float avg     = 0.0; // Xvag(k)
int ad_value = 0;
//int Motor_Pwm = 30; // 
int Steering_Angle=540;

// 190 255 320 385 450 515 580
// 580 645 710 775 840 905 960 
// 

float Kp = 1.35;//1.3 , 1.35 
float Ki = 0.178; // 0.178
float Kd = 2.4;//2.3, 2.4
double error, error_old;
double error_s, error_d;
int pwm_output = 30;

void Timer_ISR(void){
  ad_value = analogRead(A15); // Pin 지정 581(중앙 값)
  avg = alpha * old_avg + (1.0 - alpha)*ad_value;
  old_avg = avg;
  PID_Control();
  Protect_steer();
 // ad_value = ad_value + 1;
  
}
void Protect_steer(){
  if(max_val < avg) pwm_output = 0;
  else if(min_val > avg) pwm_output = 0;
  steer_motor_control(pwm_output);
}
void PID_Control()
{
  error = Steering_Angle - avg ;
  error_s += error;
  error_d = error - error_old;
  error_s = (error_s >=  100) ?  100 : error_s;
  error_s = (error_s <= -100) ? -100 : error_s;

  pwm_output = Kp * error + Kd * error_d + Ki * error_s;
  pwm_output = (pwm_output >=  55) ?  55 : pwm_output;
  pwm_output = (pwm_output <= -55) ? -55 : pwm_output;

  if (fabs(error) <= 2.2)
  {
    steer_motor_control(0);
    error_s = 0;
  }
  else          steer_motor_control(pwm_output);
  error_old = error;  
}
void steer_motor_control(int motor_pwm)
{
  if( (avg>= max_val  ) || (avg <= min_val  ) )
  {

    // Serial.println("HH");
     digitalWrite(MOTOR3_ENA, LOW);
     digitalWrite(MOTOR3_ENB, LOW);
     analogWrite(MOTOR3_PWM, 0);
     return;    
  }
  
  if (motor_pwm > 0) // forward
  {
    digitalWrite(MOTOR3_ENA, LOW);
    digitalWrite(MOTOR3_ENB, HIGH);
    analogWrite(MOTOR3_PWM, motor_pwm);
  }
  else if (motor_pwm < 0) // backward
  {
    digitalWrite(MOTOR3_ENA, HIGH);
    digitalWrite(MOTOR3_ENB, LOW);
    analogWrite(MOTOR3_PWM, -motor_pwm);
  }
  else // stop
  {
    digitalWrite(MOTOR3_ENA, LOW);
    digitalWrite(MOTOR3_ENB, LOW);
    analogWrite(MOTOR3_PWM, 0);
  }
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  MsTimer2::set(10,Timer_ISR);
  MsTimer2::start();
   //Steer
  pinMode(MOTOR3_PWM, OUTPUT);
  pinMode(MOTOR3_ENA, OUTPUT);  // L298 motor control direction
  pinMode(MOTOR3_ENB, OUTPUT);  // L298 motor control PWM
  
}


void loop() {
  // put your main code here, to run repeatedly:
  

  /*avg = alpha * old_avg + (1.0 - alpha)*ad_value;
  old_avg = avg;
*/
  Serial.print("avg = ");Serial.print(avg);Serial.print(" ");
  Serial.print("pwm_output = ");Serial.println(pwm_output);

}
