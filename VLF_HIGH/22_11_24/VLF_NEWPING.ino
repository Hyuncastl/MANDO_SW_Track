#define IN1Pin 3
#define IN2Pin 2
#define ENAPin 5

#define IN3Pin 4
#define IN4Pin 7
#define ENBPin 6

#define F_Sonar 8
#define R_Sonar 10
#define L_Sonar 11

float F_distance,L_distance,R_distance;

void go_forward(){                                    //직진하는 함수 설정
  digitalWrite(IN1Pin,HIGH);
  digitalWrite(IN2Pin,LOW);
  analogWrite(ENAPin,150);

  digitalWrite(IN3Pin,HIGH);
  digitalWrite(IN4Pin,LOW);
  analogWrite(ENBPin,150);
} 

void go_left(){                                     //좌회전하는 함수 설정
  digitalWrite(IN1Pin,LOW);
  digitalWrite(IN2Pin,HIGH);
  analogWrite(ENAPin, 20);

  digitalWrite(IN3Pin,HIGH);
  digitalWrite(IN4Pin,LOW);
  analogWrite(ENBPin, 80);
}

void go_right(){                                  //우회전하는 함수 설정
  digitalWrite(IN1Pin,HIGH);
  digitalWrite(IN2Pin,LOW);
  analogWrite(ENAPin, 80);

  digitalWrite(IN3Pin,LOW);
  digitalWrite(IN4Pin,HIGH);
  analogWrite(ENBPin, 20);
}


void go_stop(){                                        //정지하는 함수 설정
  digitalWrite(IN1Pin,HIGH);
  digitalWrite(IN2Pin,HIGH);

  digitalWrite(IN3Pin,HIGH);
  digitalWrite(IN4Pin,HIGH);
}

void go_reverse(){                                   //후진하는 함수 설정
digitalWrite(IN2Pin,HIGH);
  digitalWrite(IN1Pin,LOW);
  analogWrite(ENAPin, 150);

  digitalWrite(IN4Pin,HIGH);
  digitalWrite(IN3Pin,LOW);
  analogWrite(ENBPin, 150);
}

float Front_Sonar(){                           //전방 센서 함수를 float으로 설정
  float duration=0;
  float distance=0;
  pinMode(F_Sonar,OUTPUT);
  digitalWrite(F_Sonar,LOW);
  digitalWrite(F_Sonar,HIGH);
  delay(10);
  digitalWrite(F_Sonar,LOW);
  pinMode(F_Sonar,INPUT);  
  duration = pulseIn(F_Sonar,HIGH);
  distance = (duration*340)/2/1000;
  return distance;
  }
 
  float Right_Sonar(){                        //우측 센서 함수를 float으로 설정
  float duration=0;
  float distance=0;
  pinMode(R_Sonar,OUTPUT);
  digitalWrite(R_Sonar,LOW);
  digitalWrite(R_Sonar,HIGH);
  delay(10);
  digitalWrite(R_Sonar,LOW);
  pinMode(R_Sonar,INPUT);  
  duration = pulseIn(R_Sonar,HIGH);
  distance = (duration*340)/2/1000;
  
  return distance;  
}

float Left_Sonar(){                        //좌측 센서 함수를 float으로 설정
  float duration=0;
  float distance=0;
  pinMode(L_Sonar,OUTPUT);
  digitalWrite(L_Sonar,LOW);
  digitalWrite(L_Sonar,HIGH);
  delay(10);
  digitalWrite(L_Sonar,LOW);
  pinMode(L_Sonar,INPUT);  
  duration = pulseIn(L_Sonar,HIGH);
  distance = (duration*340)/2/1000;
  return distance;
}

  
void setup() {
  pinMode(IN1Pin,OUTPUT);
  pinMode(IN2Pin,OUTPUT);
  pinMode(IN3Pin,OUTPUT);
  pinMode(IN4Pin,OUTPUT);
  Serial.begin(115200);
}

void loop() {
F_distance = Front_Sonar();
R_distance = Right_Sonar();
L_distance = Left_Sonar();
if(F_distance<= 250){                           //전방 센서가 측정 값이내 일 때 정지
go_stop();
delay(600);
if((R_distance<=250)&&(L_distance<=250)){        //양쪽센서가 동시에 측정 값이내 일 때 AND문 활용해 오른쪽으로 유턴
   go_right();
   delay (3600);
   go_stop();
  delay(600); 
}

else if(R_distance <=250){                        //오른쪽만 막힌 경우 
   go_left();
  delay (1800);
   go_stop();
  delay(600); 
}
 else if(L_distance <=250){                      //왼쪽도 막히면 우회전
   go_right();
    delay(1800);
    go_stop();
    delay(600);
    }
}
else{
   go_forward();
}
}
