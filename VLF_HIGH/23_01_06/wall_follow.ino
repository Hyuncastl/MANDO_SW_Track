#include <NewPing.h>
#define MAX_DISTANCE 250

int IN1 =2;
int IN2 =3;
int ENA =5;

int IN3 =4;
int IN4 =7;
int ENB =6;

float F_Sonar_Distance = 0.0;
float F_Sonar_Distance_old = 0.0;
float F_Sonar_Error = 0.0;

float R_Sonar_Distance = 0.0;
float R_Sonar_Distance_old = 0.0;
float R_Sonar_Error = 0.0;

float L_Sonar_Distance = 0.0;
float L_Sonar_Distance_old = 0.0;
float L_Sonar_Error = 0.0;

int motor_diff=0;
int index;
int line_sensor[5]={0,0,0,0,0};
int mission_flag=-1;

NewPing sonar[3] =
{
  NewPing (8, 8, MAX_DISTANCE),
  NewPing (10, 10, MAX_DISTANCE),
  NewPing (11, 11, MAX_DISTANCE),
 
};

void motor_control_l(int direction, int speed1, int speed2)    //방향 속도 1: 전진 2: 우 3: 좌
{
  switch(direction)
  {
    case 1: digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);          
            analogWrite(ENA, speed1);
            analogWrite(ENB, speed2);
            break;
    case 2: digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
            digitalWrite(IN3, LOW);
            digitalWrite(IN4, HIGH);          
            analogWrite(ENA, speed1);
            analogWrite(ENB, speed2);
            break;
    case 3: digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
            digitalWrite(IN3, HIGH);
            digitalWrite(IN4, LOW);          
            analogWrite(ENA, speed1);
            analogWrite(ENB, speed2);
            break;
  }
}

int read_line_sensor(){
  int i,line_index;
  int sum=0;
 
  for(i=0;i<5;i++){
    line_sensor[i]=digitalRead(A0+i);
    sum+=line_sensor[i];
    Serial.print(line_sensor[i]);
    Serial.print(" ");
  }
  Serial.print("");
  // line_sensor[0] line_sensor[1]  line_sensor[2]  line_sensor[3]  line_sensor[4]
  //      -4        -3    -2    -1      0   1   2     3 4
  if(sum == 1){
    if(line_sensor[0]==1)line_index=-4;
    if(line_sensor[1]==1)line_index=-2;
    if(line_sensor[2]==1)line_index=0;
    if(line_sensor[3]==1)line_index=2;
    if(line_sensor[4]==1)line_index=4;
  }
  else if(sum == 2){
    if((line_sensor[0]==1)&&(line_sensor[1]==1))line_index=-3;
    if((line_sensor[1]==1)&&(line_sensor[2]==1))line_index=-1;
    if((line_sensor[2]==1)&&(line_sensor[3]==1))line_index=1;
    if((line_sensor[3]==1)&&(line_sensor[4]==1))line_index=3;
  }
  else if(sum == 5){
    if((line_sensor[0]==1)&&(line_sensor[1]==1)&&(line_sensor[2]==1)&&(line_sensor[3]==1)&&(line_sensor[4]==1))line_index=-10;
  }
  else line_index = -5555;
  Serial.print(line_index);
  Serial.print("");
  return line_index;
} //라인센서 구동함수

void line_tracing()
{
  index=read_line_sensor();
  switch(index){
    case -10: motor_control_l(1,0,0); break; //정지
    case 0: motor_control_l(1,110,110); break; //전진
    case -4: motor_control_l(3,20,200); break; //좌
    case -2: motor_control_l(3,50,130); break;
    case -3: motor_control_l(3,50,150); break;
    case -1: motor_control_l(3,50,110); break;
    case 4: motor_control_l(2,200,20); break;//우
    case 2: motor_control_l(2,130,50); break;
    case 3: motor_control_l(2,150,50); break;
    case 1: motor_control_l(2,110,50); break;
  }
}

void find_obs()
{
  if(F_Sonar_Distance <=500)
  {
    while(F_Sonar_Distance >500)
    {
      motor_control_l(1,0,0);
    }
   
  }
 
}

void read_sonar_sensor(void)  //초음파 센서 값을 읽는 함수
{
   
 R_Sonar_Distance = sonar[1].ping_cm() * 10;
 L_Sonar_Distance = sonar[2].ping_cm() * 10;
 F_Sonar_Distance = sonar[0].ping_cm() * 10;
 if(R_Sonar_Distance == 0) R_Sonar_Distance = MAX_DISTANCE *10.0;
 if(L_Sonar_Distance == 0) L_Sonar_Distance = MAX_DISTANCE *10.0;
 if(F_Sonar_Distance == 0) F_Sonar_Distance = MAX_DISTANCE *10.0;
}

void update_sonar_old(void)
{
  R_Sonar_Distance_old = R_Sonar_Distance;  //측정된거된값을 과거에 넣는다
  L_Sonar_Distance_old = L_Sonar_Distance;  //측정된거된값을 과거에 넣는다
  F_Sonar_Distance_old = F_Sonar_Distance;  //측정된거된값을 과거에 넣는다
}

void update_sonar_error(void)
{
  R_Sonar_Error = R_Sonar_Distance - 95;  //현재 값에서 옛날 값을 뺀것
  L_Sonar_Error = L_Sonar_Distance - 95;  //현재 값에서 옛날 값을 뺀것
  F_Sonar_Error = F_Sonar_Distance - 95;  //현재 값에서 옛날 값을 뺀것
}

void wall_follow()
  {
  read_sonar_sensor();
 
    if(L_Sonar_Distance > R_Sonar_Distance)
    {
      motor_control_l(1,60,140);
    }
    else if(L_Sonar_Distance < R_Sonar_Distance)
    {
      motor_control_l(1,140,35);
    }
    else if((L_Sonar_Distance == R_Sonar_Distance))
    {
      motor_control_l(1,130,130);      
    }
}




   
void setup() {
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  Serial.begin(115200);
}

void loop() {
      read_line_sensor();
      line_tracing();
      if(index == 0){
      wall_follow();
      }
}
