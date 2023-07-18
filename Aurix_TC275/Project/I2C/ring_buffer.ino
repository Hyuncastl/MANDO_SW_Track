#define RING_BUFFER_SIZE 8
#define TEMP_BUFFER_SIZE 64
static char ring_buff[RING_BUFFER_SIZE]={0,};
static int data = 0;
#define check_pin 43
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  pinMode(check_pin,OUTPUT);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  char read_temp[TEMP_BUFFER_SIZE];
  if(Serial1.available() > 0)
  {
    byte leng = Serial1.readBytes(read_temp, TEMP_BUFFER_SIZE);
    Serial1.println(" ");
    Serial1.print("Input data Lengh : ");
    Serial1.println(leng);
    for(int i = 0; i < leng; i++)
    {
        ring_buff[data] = read_temp[i];
        Serial1.print(read_temp[i]);
        data++;
        if(data >= RING_BUFFER_SIZE)
        {
        for(int i = 0; i < leng; i++)
        {
          ring_buff[i] = 48;
        }
          data = 0;
        }
    }
    Serial1.println(" ");    Serial1.print("data no : "); Serial1.println(data);
    for(int j = 0; j < RING_BUFFER_SIZE; j++)
    { 
      Serial.print("data :");
      Serial1.print(ring_buff[j]);
      
    if(ring_buff[j] > RING_BUFFER_SIZE)
    {
      ring_buff[j] = 48;   //아스키 코드 0
    }
    }
    
    Serial1.println(" ");
  }
    digitalWrite(check_pin,HIGH);
    delay(2);
    digitalWrite(check_pin,LOW);

}
