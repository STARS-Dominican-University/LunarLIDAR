#include <Wire.h>
#include <LIDARLite.h>

// Define the pin connected to the Power Enable pin of the Lidar Lite v3
#define LIDAR_POWER_ENABLE_PIN1 9
#define LIDAR_POWER_ENABLE_PIN2 10

LIDARLite lidarLite;
int cal_cnt = 0;
bool alternate = false;

void setup() {
  Serial.begin(9600); // Initialize serial connection to display distance readings

  // Set the Lidar Power Enable pin as an output
  pinMode(LIDAR_POWER_ENABLE_PIN1, OUTPUT);
  pinMode(LIDAR_POWER_ENABLE_PIN2, OUTPUT);

  // Set up reading
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0); // Change this number to try out alternate configurations

}

void loop() {
  int dist;

  if(alternate){
    disableLidar(LIDAR_POWER_ENABLE_PIN1);
    enableLidar(LIDAR_POWER_ENABLE_PIN2);
    alternate = false;
  }
  else{
    enableLidar(LIDAR_POWER_ENABLE_PIN1);
    disableLidar(LIDAR_POWER_ENABLE_PIN2);
    alternate = true;
  }

  // At the beginning of every 100 readings,
  // take a measurement with receiver bias correction
  if ( cal_cnt == 0 ) {
    dist = lidarLite.distance();      // With bias correction
  } else {
    dist = lidarLite.distance(false); // Without bias correction
  }

  // Increment reading counter
  cal_cnt++;
  cal_cnt = cal_cnt % 100;


  // Display distance
  Serial.print(dist);
  Serial.println(" cm");
  delay(10);
}


// ENABLE DISABLE
void enableLidar(int lidar_pin) {
  digitalWrite(lidar_pin, HIGH); // Set the Power Enable pin HIGH to enable power
  delay(100); // Add a delay if necessary for the Lidar to initialize
}
void disableLidar(int lidar_pin) {
  digitalWrite(lidar_pin, LOW); // Set the Power Enable pin LOW to disable power
}
