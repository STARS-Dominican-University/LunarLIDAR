#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define TRANSMITTER_CSN 8
#define TRANSMITTER_CE 7

RF24 radio(TRANSMITTER_CE, TRANSMITTER_CSN);

const byte address[6] = "00001";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}
z
void loop() {
  char data_in[32] = "";
  radio.read(&data_in, sizeof(data_in));
  Serial.println(data_in);
  delay(100);  // Add a delay to avoid overwhelming the serial monitor
}