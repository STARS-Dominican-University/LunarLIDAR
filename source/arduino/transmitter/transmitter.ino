#include <Wire.h>
#include <LIDARLite.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define servo motor pins
#define SERVO_MOTOR_PIN1 13
#define SERVO_MOTOR_PIN2 12

// Define the pin connected to the Power Enable pin of the Lidar Lite v3
#define LIDAR_POWER_ENABLE_PIN1 9
#define LIDAR_POWER_ENABLE_PIN2 10

// Define CSN, CE for transmitter
#define TRANSMITTER_CSN 8
#define TRANSMITTER_CE 7

RF24 radio(TRANSMITTER_CE, TRANSMITTER_CSN);

// Represents address for pipe two modules will communicate
const byte address[6] = "00001";

// Initiate lidar object
LIDARLite lidarLite;

// Inititate servo object
Servo servo1; // Servo object for motor 1 on pin 9
Servo servo2; // Servo object for motor 2 on pin 8

int cal_cnt = 0;
bool alternate = false;

void setup()
{

  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();

  // Set the Lidar Power Enable pin as an output
  pinMode(LIDAR_POWER_ENABLE_PIN1, OUTPUT);
  pinMode(LIDAR_POWER_ENABLE_PIN2, OUTPUT);

  // Set up reading
  lidarLite.begin(0, true); // Set configuration to default and I2C to 400 kHz
  lidarLite.configure(0);   // Change this number to try out alternate configurations

  // Attatch servo motor
  servo1.attach(SERVO_MOTOR_PIN1); // Attach servo 1 to pin 9
  servo2.attach(SERVO_MOTOR_PIN2); // Attach servo 2 to pin 8
}

void loop()
{
  radio.stopListening();

  // Rotate motor 1 from 0 to 180 degrees
  for (int omega = 0; omega <= 180; omega++)
  {

    for (float theta = 0; theta <= 180; theta += 1.8)
    {
      int x;

      if (alternate)
      {
        disableLidar(LIDAR_POWER_ENABLE_PIN1);
        enableLidar(LIDAR_POWER_ENABLE_PIN2);
        alternate = false;
      }
      else
      {
        enableLidar(LIDAR_POWER_ENABLE_PIN1);
        disableLidar(LIDAR_POWER_ENABLE_PIN2);
        alternate = true;
      }

      // At the beginning of every 100 readings,
      // take a measurement with receiver bias correction
      if (cal_cnt == 0)
      {
        x = lidarLite.distance(); // With bias correction
      }
      else
      {
        x = lidarLite.distance(false); // Without bias correction
      }

      // Increment reading counter
      cal_cnt++;
      cal_cnt = cal_cnt % 100;

      // CALL ANGLE
      servo2.write(theta); // CALL ANGLE 0
      delay(50);           // Adjust delay as needed for smoother motion

      char transmit_data[32] = "";

      sprintf(transmit_data, "x: %d, o: %d, w: %d", x, int(theta), omega);

      // SEND DATA
      radio.write(&transmit_data, strlen(transmit_data) + 1);
      delay(50);
    }

    servo1.write(omega); // CALL ANGLE w
    delay(100);          // Adjust delay as needed for smoother motion
  }
}

// ENABLE DISABLE
void enableLidar(int lidar_pin)
{
  digitalWrite(lidar_pin, HIGH); // Set the Power Enable pin HIGH to enable power
  delay(100);                    // Add a delay if necessary for the Lidar to initialize
}
void disableLidar(int lidar_pin)
{
  digitalWrite(lidar_pin, LOW); // Set the Power Enable pin LOW to disable power
}
