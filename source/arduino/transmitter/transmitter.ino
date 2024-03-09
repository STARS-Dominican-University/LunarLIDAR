#include <Wire.h>
#include <LIDARLite.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// SERVO DEFINITIONS
#define SERVO_MOTOR_BOTTOM_ONE 12
#define SERVO_MOTOR_TOP_ONE 13
#define SERVO_MOTOR_BOTTOM_TWO 5
#define SERVO_MOTOR_TOP_TWO 6
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

// LIDAR DEFINITIONS
#define LIDAR_POWER_ENABLE_PIN1 3
#define LIDAR_POWER_ENABLE_PIN2 2
LIDARLite lidarLite;

// TRANSMISSION DEFINITIONS
#define TRANSMITTER_CSN 8
#define TRANSMITTER_CE 7
#define ADDRESS "00001"
const byte address[6] = ADDRESS;
RF24 radio(TRANSMITTER_CE, TRANSMITTER_CSN);

// HELPER VARIABLES
int cal_cnt = 0;
bool alternate = false;

// PITCH
int pitch_down = false;
int current_pitch = 0;

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
  lidarLite.configure(0);   // Change this number to try out alternate configuratio s

  servo1.attach(SERVO_MOTOR_BOTTOM_ONE);
  servo2.attach(SERVO_MOTOR_TOP_ONE);
  servo3.attach(SERVO_MOTOR_BOTTOM_TWO);
  servo4.attach(SERVO_MOTOR_TOP_TWO);
}

void loop()
{
  // LIDAR DISTANCE
  int x;

  // ASSERT ANTENNA IS TRANSMISSION
  radio.stopListening();

  // Sweep yaw from 0 to 180 degrees
  for (float omega = 0; omega <= 180; omega += 1.8)
  {
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

    servo1.write(omega);
    delay(10);

    transmit_data(x, current_pitch, omega);

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
    servo3.write(omega);
    delay(10);

    transmit_data(x, current_pitch, omega);
  }
  delay(10);
  if (pitch_down)
  {
    servo2.write(current_pitch);
    delay(10);
    current_pitch -= 1.8;
  }
  else
  {
    servo2.write(current_pitch);
    delay(10);
    current_pitch += 1.8;
  }
  if (pitch_down)
  {
    servo4.write(current_pitch);
    delay(10);
    current_pitch -= 1.8;
  }
  else
  {
    servo4.write(current_pitch);
    delay(10);
    current_pitch += 1.8;
  }

  // Sweep yaw from 180 to 0 degrees
  for (float omega = 180; omega >= 0; omega -= 1.8)
  {
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
    servo1.write(omega);
    delay(10);

    transmit_data(x, current_pitch, omega);

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
    servo3.write(omega);
    delay(10);

    transmit_data(x, current_pitch, omega);
  }
  delay(10);

  if (pitch_down)
  {
    servo2.write(current_pitch);
    delay(10);
    current_pitch -= 1.8;
  }
  else
  {
    servo2.write(current_pitch);
    delay(10);
    current_pitch += 1.8;
  }
  if (pitch_down)
  {
    servo4.write(current_pitch);
    delay(10);
    current_pitch -= 1.8;
  }
  else
  {
    servo4.write(current_pitch);
    delay(10);
    current_pitch += 1.8;
  }

  if (current_pitch > 180)
  {
    pitch_down = true;
  }

  if (current_pitch < 0)
  {
    pitch_down = false;
  }
}

void enableLidar(int lidar_pin)
{
  digitalWrite(lidar_pin, HIGH);
  delay(100);
}
void disableLidar(int lidar_pin)
{
  digitalWrite(lidar_pin, LOW);
  delay(100);
}

void transmit_data(int x, int theta, int omega)
{
  char data[32] = "";

  char x_str[12];
  char theta_str[12];
  char omega_str[12];

  itoa(x, x_str, 10);
  itoa(theta, theta_str, 10);
  itoa(omega, omega_str, 10);

  // Concatenate strings into transmit_data
  strcpy(data, "x: ");
  strcat(data, x_str);
  strcat(data, ", o: ");
  strcat(data, theta_str);
  strcat(data, ", w: ");
  strcat(data, omega_str); // Assuming you meant to use omega twice

  // Write data to radio
  radio.write(&data, strlen(data) + 1);
  delay(10);
}