#include <MsTimer2.h>

// Steering Drive Pin 설정
#define alpha 0.38
#define max_val 970//max 1024 safe 960
#define min_val 180//min 64   safe 190

#define MOTOR3_PWM 8
#define MOTOR3_ENA 9
#define MOTOR3_ENB 10

// Front Motor Drive Pin 설정
#define MOTOR1_PWM 2
#define MOTOR1_ENA 3
#define MOTOR1_ENB 4
// Rear Motor Drive Pin 설정
#define MOTOR3_PWM 5
#define MOTOR3_ENA 6
#define MOTOR3_ENB 7

// 1m 당 pulse 수  확인 해야 함
#define m_1_pulse 348
#define m_2_pulse 345      

// pulse 당 m  확인 해야 함
#define pulse_1_m 1./348.
#define pulse_2_m 1./345.

// 100Hz 제어 주기에서 속도와 Δpulse 변환 값
#define vel_1_pulse m_1_pulse/100.
#define vel_2_pulse m_2_pulse/100.

int f_speed = 0, r_speed = 0;  // 속도

void rear_motor_control(int motor1_pwm)
{
   if (motor1_pwm > 0) // forward
  {
    digitalWrite(MOTOR1_ENA, HIGH);   // 방향은 설정에 따라 바꿀 것
    digitalWrite(MOTOR1_ENB, LOW);
    analogWrite(MOTOR1_PWM, motor1_pwm);
  }
  else if (motor1_pwm < 0) // backward
  {
    digitalWrite(MOTOR1_ENA, LOW);
    digitalWrite(MOTOR1_ENB, HIGH);
    analogWrite(MOTOR1_PWM, -motor1_pwm);
  }
  else
  {
    digitalWrite(MOTOR1_ENA, LOW);
    digitalWrite(MOTOR1_ENB, LOW);
    digitalWrite(MOTOR1_PWM, 0);
  }
}

void front_motor_control(int motor3_pwm)
{
   if (motor3_pwm > 0) // forward
  {
    digitalWrite(MOTOR3_ENA, HIGH);
    digitalWrite(MOTOR3_ENB, LOW);
    analogWrite(MOTOR3_PWM, motor3_pwm);
  }
  
  else if (motor3_pwm < 0) // backward
  {
    digitalWrite(MOTOR3_ENA, LOW);
    digitalWrite(MOTOR3_ENB, HIGH);
    analogWrite(MOTOR3_PWM, -motor3_pwm);
  }
  else
  {
    digitalWrite(MOTOR3_ENA, LOW);
    digitalWrite(MOTOR3_ENB, LOW);
    digitalWrite(MOTOR3_PWM, 0);
  }
}

void motor_control(int front_speed, int rear_speed)
{
  //front_motor_control(front_speed);
  rear_motor_control(rear_speed);  
}
///////////////////// Encoder   ///////////////////////////
#include <SPI.h>
#define ENC1_ADD 22
#define ENC2_ADD 23

signed long encoder1count = 0;
signed long encoder2count = 0;

signed long encoder1_error = 0;
signed long encoder2_error = 0;

signed long encoder1_error_d = 0;
signed long encoder2_error_d = 0;


signed long encoder1_target = 0; // 348
signed long encoder2_target = 0; // 345

signed long encoder1_error_old = 0; 
signed long encoder2_error_old = 0; 

float target_distance = 0.57;

float Kp = 0.225;// 0.225
float Ki = 0.22; // 0.2
float Kd = 0.95;// 0.95

float Kp2 = 0.228; //0.228
float Ki2 = 0; //
float Kd2 = 0.87; //0.87
int pwm_output = 0;
int pwm_output2 = 0;

void initEncoders()   //encoder 초기화
{
  // Set slave selects as outputs
  pinMode(ENC1_ADD, OUTPUT);
  pinMode(ENC2_ADD, OUTPUT); 
  // Raise select pins
  // Communication begins when you drop the individual select signsl
  digitalWrite(ENC1_ADD,HIGH);
  digitalWrite(ENC2_ADD,HIGH);
  SPI.begin();
  // Initialize encoder 1
  //    Clock division factor: 0
  //    Negative index input
  //    free-running count mode
  //    x4 quatrature count mode (four counts per quadrature cycle)
  // NOTE: For more information on commands, see datasheet
  digitalWrite(ENC1_ADD,LOW);        // Begin SPI conversation
  SPI.transfer(0x88);                       // Write to MDR0
  SPI.transfer(0x03);                       // Configure to 4 byte mode
  digitalWrite(ENC1_ADD,HIGH);       // Terminate SPI conversation 
  // Initialize encoder 2
  //    Clock division factor: 0
  //    Negative index input
  //    free-running count mode
  //    x4 quatrature count mode (four counts per quadrature cycle)
  // NOTE: For more information on commands, see datasheet
  digitalWrite(ENC2_ADD,LOW);        // Begin SPI conversation
  SPI.transfer(0x88);                       // Write to MDR0
  SPI.transfer(0x03);                       // Configure to 4 byte mode
  digitalWrite(ENC2_ADD,HIGH);       // Terminate SPI conversation 
}

long readEncoder(int encoder_no) 
{
  // Initialize temporary variables for SPI read
  unsigned int count_1, count_2, count_3, count_4;
  long count_value;   
  digitalWrite(ENC1_ADD + encoder_no-1,LOW);      // Begin SPI conversation
   // digitalWrite(ENC4_ADD,LOW);      // Begin SPI conversation
  SPI.transfer(0x60);                     // Request count
  count_1 = SPI.transfer(0x00);           // Read highest order byte
  count_2 = SPI.transfer(0x00);           
  count_3 = SPI.transfer(0x00);           
  count_4 = SPI.transfer(0x00);           // Read lowest order byte
  digitalWrite(ENC1_ADD+encoder_no-1,HIGH);     // Terminate SPI conversation 
  //digitalWrite(ENC4_ADD,HIGH);      // Begin SPI conversation
  
// Calculate encoder count
  count_value= -(((long)count_1<<24) + ((long)count_2<<16) + ((long)count_3<<8 ) + (long)count_4);
  
  return count_value;
  }
  
void clearEncoderCount(int encoder_no) {    
  // Set encoder1's data register to 0
  digitalWrite(ENC1_ADD+encoder_no-1,LOW);      // Begin SPI conversation  
  // Write to DTR
  SPI.transfer(0x98);    
  // Load data
  SPI.transfer(0x00);  // Highest order byte
  SPI.transfer(0x00);           
  SPI.transfer(0x00);           
  SPI.transfer(0x00);  // lowest order byte
  digitalWrite(ENC1_ADD+encoder_no-1,HIGH);     // Terminate SPI conversation 
  delayMicroseconds(100);  // provides some breathing room between SPI conversations
  // Set encoder1's current data register to center
  digitalWrite(ENC1_ADD+encoder_no-1,LOW);      // Begin SPI conversation  
  SPI.transfer(0xE0);    
  digitalWrite(ENC1_ADD+encoder_no-1,HIGH);     // Terminate SPI conversation 
}

void control_callback()
{
   encoder1count = readEncoder(1);
   encoder2count = readEncoder(2);
   PID_Control();
}

void PID_Control()
{
  encoder1_target += target_distance * vel_1_pulse;
  encoder2_target += target_distance * vel_2_pulse;
  
  encoder1_error   =    encoder1_target -  encoder1count ;
  encoder1_error_d =    encoder1_error -  encoder1_error_old ;
  encoder2_error   =    encoder2_target -  encoder2count ;
  encoder2_error_d =    encoder2_error -  encoder2_error_old ;
   
  pwm_output = Kp * encoder1_error + Kd * encoder1_error_d;
  pwm_output = (pwm_output >=  255) ?  255 : pwm_output;
  pwm_output = (pwm_output <= -255) ? -255 : pwm_output;

  pwm_output2 = Kp2 * encoder2_error + Kd2 * encoder2_error_d;
  pwm_output2 = (pwm_output2 >=  255) ?  255 : pwm_output2;
  pwm_output2 = (pwm_output2 <= -255) ? -255 : pwm_output2;

  if (fabs(encoder1_error) <= 2 || fabs(encoder2_error) <= 2)
  {
    motor_control(0, 0);
  }
  else{
    motor_control(pwm_output, pwm_output2);
    }
  encoder1_error_old = encoder1_error;
  encoder2_error_old = encoder2_error;  
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
 // Front Motor Drive Pin Setup
  pinMode(MOTOR1_PWM, OUTPUT);
  pinMode(MOTOR1_ENA, OUTPUT);  // L298 motor control direction
  pinMode(MOTOR1_ENB, OUTPUT);
  // Rear Motor Drive Pin Setup
  pinMode(MOTOR3_PWM, OUTPUT);
  pinMode(MOTOR3_ENA, OUTPUT);  // L298 motor control direction
  pinMode(MOTOR3_ENB, OUTPUT);
  
  initEncoders();          // initialize encoder
  clearEncoderCount(1); 
  clearEncoderCount(2); 
  MsTimer2::set(10,control_callback);
  MsTimer2::start();
}

void loop()
{  
  /*Serial.print("Encoder 1 : ");*/   //Serial.print(encoder1count); Serial.print(" ");
  /*Serial.print("Encoder 2 : ");*/    Serial.print(encoder2count); Serial.print(" ");
  /*Serial.print("Encoder error 1 : ");*/    //Serial.print(encoder1_error); Serial.print(" ");
  /*Serial.print("Encoder error 2 : ");*/    //Serial.print(encoder2_error); Serial.println(" ");
  /*Serial.print("Motor 1 Dist.: ");*/   //Serial.println(encoder1count * pulse_2_m);
  /*Serial.print("Motor 2 Dist.: ");*/   Serial.println(encoder2count * pulse_2_m);
  //Serial.print("PWM : ");    Serial.println(pwm_output);
  //Serial.print("PWM2 : ");    Serial.println(pwm_output2);
}
