#include <SoftwareSerial.h>

SoftwareSerial DE10Serial(2, 3);   // RX | TX

void setup() {
  Serial.begin(57600);
  //Serial.println("Start DE-10 Lite");
  DE10Serial.begin(57600);  // HC-05 default speed in AT command mode
}

void loop() {
//The code below allows for commands and messages to be sent from COMPUTER (serial monitor) -> HC-05
  if (Serial.available())           // Keep reading from Arduino Serial Monitor 
    DE10Serial.write(Serial.read());  // and send to HC-05

//The code below allows for commands and messages to be sent from HC-05 -> COMPUTER (serial monitor)    
  if (DE10Serial.available())         // Keep reading from HC-05 and send to Arduino 
    Serial.write(DE10Serial.read());  // Serial Monitor

}
