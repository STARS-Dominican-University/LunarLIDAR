#include <Wire.h>
#include <LIDARLite.h>
#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


int x = 1;
int o = 12;
int w = 123;

void setup() {
  Serial.begin(9600);
}

void loop() {
  transmit_data(x, o, w);
}

void transmit_data(int x, int theta, int omega){
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
  Serial.println(data);
  delay(10);
}
