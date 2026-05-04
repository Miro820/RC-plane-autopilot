// Based of off "Wire Controller Reader" by Nicholas Zambetti [http://www.zambetti.com](http://www.zambetti.com
// Slightly modified by Miro Mangelschots

#include <Wire.h>

int MPU_adress = 0x68;

void setup() {
  Wire.begin(); //start I²C communication
  Serial.begin(115200);
}

void loop() {
  Wire.requestFrom(MPU_adress, 6); // request 6 bytes of data from MPU_adress
  while (Wire.available()) { // Peripheral may send less than requested
    uint8_t data = Wire.read(); // Receive data
    Serial.print(data); // print data
  }
  delay(500); 
}
