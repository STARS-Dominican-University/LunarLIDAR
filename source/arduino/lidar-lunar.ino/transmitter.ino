#include <Wire.h>
#include <LIDARLite.h>
#include <Servo.h>


// Define the pin connected to the Power Enable pin of the Lidar Lite v3
#define LIDAR_POWER_ENABLE_PIN1 9
#define LIDAR_POWER_ENABLE_PIN2 10

LIDARLite lidarLite;
int cal_cnt = 0;
bool alternate = false;
Servo servo1;  // Servo object for motor 1 on pin 9
Servo servo2;  // Servo object for motor 2 on pin 8
int repetitions = 1;

void setup() {
  Serial.begin(9600);  // Initialize serial connection to display distance readings

  // Set the Lidar Power Enable pin as an output
  pinMode(LIDAR_POWER_ENABLE_PIN1, OUTPUT);
  pinMode(LIDAR_POWER_ENABLE_PIN2, OUTPUT);

  // Set up reading
  lidarLite.begin(0, true);  // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0);    // Change this number to try out alternate configurations

  servo1.attach(5);  // Attach servo 1 to pin 9
  servo2.attach(4);  // Attach servo 2 to pin 8
}

void loop() {
  // Rotate motor 1 from 0 to 180 degrees
  for (int omega = 0; omega <= 180; omega++) {

    for (float theta = 0; theta <= 180; theta += 1.8) {
      int x;

      if (alternate) {
        disableLidar(LIDAR_POWER_ENABLE_PIN1);
        enableLidar(LIDAR_POWER_ENABLE_PIN2);
        alternate = false;
      } else {
        enableLidar(LIDAR_POWER_ENABLE_PIN1);
        disableLidar(LIDAR_POWER_ENABLE_PIN2);
        alternate = true;
      }

      // At the beginning of every 100 readings,
      // take a measurement with receiver bias correction
      if (cal_cnt == 0) {
        x = lidarLite.distance();  // With bias correction
      } else {
        x = lidarLite.distance(false);  // Without bias correction
      }

      // Increment reading counter
      cal_cnt++;
      cal_cnt = cal_cnt % 100;

      // CALL ANGLE
      servo2.write(theta);  // CALL ANGLE 0
      delay(15);            // Adjust delay as needed for smoother motion

      // Display distance
      Serial.print("x: " + String(x) + ",o: " + String(theta) + ", w: " + String(omega));
      delay(10);
    }

    servo1.write(omega);  // CALL ANGLE w
    delay(1000);          // Adjust delay as needed for smoother motion
  }
}


// ENABLE DISABLE
void enableLidar(int lidar_pin) {
  digitalWrite(lidar_pin, HIGH);  // Set the Power Enable pin HIGH to enable power
  delay(100);                     // Add a delay if necessary for the Lidar to initialize
}
void disableLidar(int lidar_pin) {
  digitalWrite(lidar_pin, LOW);  // Set the Power Enable pin LOW to disable power
}
