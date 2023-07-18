int IN1Pin =2;
int IN2Pin =3;
int ENAPin =5;

int IN3Pin =4;
int IN4Pin =7;
int ENBPin =6;

void go_left(){
  digitalWrite(IN1Pin,HIGH);
  digitalWrite(IN2Pin,LOW);
  analogWrite(ENAPin, 0);

  digitalWrite(IN3Pin,HIGH);
  digitalWrite(IN4Pin,LOW);
  analogWrite(ENBPin, 80);
}

void go_right(){
  digitalWrite(IN1Pin,HIGH);
  digitalWrite(IN2Pin,LOW);
  analogWrite(ENAPin, 80);

  digitalWrite(IN3Pin,HIGH);
  digitalWrite(IN4Pin,LOW);
  analogWrite(ENBPin, 0);
}

void go_forward(){
digitalWrite(IN1Pin,HIGH);
  digitalWrite(IN2Pin,LOW);
  analogWrite(ENAPin, 150);

  digitalWrite(IN3Pin,HIGH);
  digitalWrite(IN4Pin,LOW);
  analogWrite(ENBPin, 150);
}

void go_reverse(){
digitalWrite(IN2Pin,HIGH);
  digitalWrite(IN1Pin,LOW);
  analogWrite(ENAPin, 150);

  digitalWrite(IN4Pin,HIGH);
  digitalWrite(IN3Pin,LOW);
  analogWrite(ENBPin, 150);
}

void setup() {
  pinMode(IN1Pin,OUTPUT);
  pinMode(IN2Pin,OUTPUT);
  
  pinMode(IN3Pin,OUTPUT);
  pinMode(IN4Pin,OUTPUT);
}

void loop() {
go_forward();
delay (1500);

go_left();
delay (1500);

go_forward();
delay (500);

go_right();
delay (1500);

go_forward();
delay (500);

go_reverse();
delay (800);
}
