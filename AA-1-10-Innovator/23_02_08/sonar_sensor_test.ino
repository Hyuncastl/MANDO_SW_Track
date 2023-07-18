// ---------------------------------------------------------------------------
// Example NewPing library sketch that pings 3 sensors 20 times a second.
// ---------------------------------------------------------------------------
#define FL_TRIG_ECHO 9
#define FR_TRIG_ECHO 10
#define R_TRIG_ECHO 11
float FL_Sonar_distance , FR_Sonar_distance , R_Sonar_distance = 0.0;

#include <NewPing.h>

#define SONAR_NUM 3      // Number of sensors.
#define MAX_DISTANCE 100 // Maximum distance (in cm) to ping.

NewPing sonar[SONAR_NUM] = {   // Sensor object array.
  NewPing(9, 9, MAX_DISTANCE), // Each sensor's trigger pin, echo pin, and max distance to ping. 
  NewPing(10, 10, MAX_DISTANCE), 
  NewPing(11, 11, MAX_DISTANCE)
};


void read_sonar_sensor()
{
    FL_Sonar_distance = sonar[0].ping_cm()*10.0;
    FR_Sonar_distance = sonar[1].ping_cm()*10.0;
    R_Sonar_distance = sonar[2].ping_cm()*10.0;
    if(FL_Sonar_distance == 0){FL_Sonar_distance = MAX_DISTANCE * 10.0;}
    if(FR_Sonar_distance == 0){FR_Sonar_distance = MAX_DISTANCE * 10.0;}
    if(R_Sonar_distance == 0){R_Sonar_distance = MAX_DISTANCE * 10.0;}
}
void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
}

void loop() { 
  read_sonar_sensor();

    
    Serial.println(FL_Sonar_distance);
    Serial.println(FR_Sonar_distance);
    Serial.println(R_Sonar_distance);
    
     delay(1000);
  }
 
