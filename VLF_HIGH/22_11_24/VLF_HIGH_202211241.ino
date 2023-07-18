// C++ code
//
#define F_Sonar 8                         //센서 핀 위치
#define R_Sonar 10
#define L_Sonar 11

float Front_Sonar(){                      //전방 센서 함수 정의
  float duration=0;                       //초기값 설정
  float distance=0;
  pinMode(F_Sonar,OUTPUT);                // 전방센서 출력모드로 설정
  digitalWrite(F_Sonar,LOW);              //전원 입력
  digitalWrite(F_Sonar,HIGH);
  delay(10);                              //1초마다 측정 값을 보여준다
  digitalWrite(F_Sonar,LOW);
  pinMode(F_Sonar,INPUT);                 //전방센서 입력모드로 설정
  duration = pulseIn(F_Sonar,HIGH);       //duration 변수 값 설정
  distance = (duration*340)/2/1000;       //distance 변수 값 설정 MM단위로 나타내기 위해 1000으로 나눠줌
  
  return distance;
}

float Right_Sonar(){                    //오른쪽 센서 함수의정의
  float duration=0;                     //초기값 설정
  float distance=0;
  pinMode(R_Sonar,OUTPUT);              // 오른쪽센서 출력모드로 설정
  digitalWrite(R_Sonar,LOW);            //전원 입력
  digitalWrite(R_Sonar,HIGH);
  delay(10);                            //1초마다 측정 값을 보여준다
  digitalWrite(R_Sonar,LOW);
  pinMode(R_Sonar,INPUT);              //오른쪽센서 입력모드로 설정
  duration = pulseIn(R_Sonar,HIGH);    //duration 변수 값 설정
  distance = (duration*340)/2/1000;    //distance 변수 값 설정 MM단위로 나타내기 위해 1000으로 나눠줌
  
  return distance;
}

float Left_Sonar(){                     //왼쪽 센서 함수 정의
  float duration=0;                     //초기값 설정
  float distance=0;
  pinMode(L_Sonar,OUTPUT);              // 오른쪽센서 출력모드로 설정
  digitalWrite(L_Sonar,LOW);            //전원 입력            
  digitalWrite(L_Sonar,HIGH);
  delay(10);                            //1초마다 측정 값을 보여준다
  digitalWrite(L_Sonar,LOW);
  pinMode(L_Sonar,INPUT);               //왼쪽센서 입력모드로 설정
  duration = pulseIn(L_Sonar,HIGH);     //duration 변수 값 설정
  distance = (duration*340)/2/1000;     //distance 변수 값 설정 MM단위로 나타내기 위해 1000으로 나눠줌
  
  return distance;
}



void setup(){
  pinMode( F_Sonar, OUTPUT);           //전방센서 출력으로 설정
  pinMode( R_Sonar, OUTPUT);           //오른쪽센서 출력으로 설정
  pinMode( L_Sonar, OUTPUT);           //왼쪽센서 출력으로 설정
  Serial.begin(115200);                //초기 위치 값 
}

float robot_width =200; //차량의 폭
float mazor_width = 500; //미로의 폭
float mazor_width_tolerance=40; //오차 범위
int flag_mazor_1=0; //1번 구간에 내가 위치해 있다

void loop(){
  float L_distance, R_distance, F_distance;   //센서 변수 설정
  float  measure_width;                       
  L_distance = Left_Sonar();
  F_distance = Front_Sonar();
  R_distance = Right_Sonar();
  
 /*Serial.print("L"); Serial.print("F"); Serial.print("R");*/
   Serial.print(L_distance);              //거리 값을 측정 후 보여줌    
  Serial.print("L_distance");             //센서 안내문구 출력
   Serial.print(R_distance);
  Serial.print("R_distance");
   Serial.print(F_distance);
  Serial.print("F_distance");
  
 measure_width= L_distance + robot_width + R_distance;
  if((measure_width >= (mazor_width - mazor_width_tolerance))&&(measure_width <= (mazor_width + mazor_width_tolerance)))
    //측정된 폭이 미로의 폭에서 오차범위를 뺀 값보다 크거나 같을 경우 AND 측정된 폭이 미로의 폭에서 오차범위를 더한 값보다 작거나 같은 경우
     {
       if((F_distance>mazor_width)&&(F_distance<mazor_width + 400))
         // 전방거리가 미로의 폭보다 클 경우 AND 전방거리가 미로의 폭보다 작을 경우(+400)
       {
         flag_mazor_1=1; //1번 구간의 초기값 설정
         Serial.print("flag_mazor_1 set"); //1번구간 안내문구 출력
       }
     }
     else
     {
       flag_mazor_1=0; //1번 구간의 else 초기값 
     }

}
