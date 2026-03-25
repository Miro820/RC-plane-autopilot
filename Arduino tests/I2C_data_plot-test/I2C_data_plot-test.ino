
#include <Wire.h>

int MPU_adress = 0x68;

void setup() {
  Wire.begin(); //start I²C communication
  Serial.begin(115200);

  Wire.beginTransmission(MPU_adress);
  Wire.write(0x00); // Wake up the sensor, by sending 000000 to 0x6B (= PWR_MGMT_1);
  Wire.endTransmission(MPU_adress);

}

void loop() {
  Wire.requestFrom(MPU_adress, 6); // request 6 bytes of data from MPU_adress
  while (Wire.available()) { // Peripheral may send less than requested
    Wire.beginTransmission(MPU_adress);
    Wire.write(0x44); // GYRO_XOUT_L
    Wire.endTransmission(MPU_adress);
    int GYRO_XOUT_L = Wire.read(); // Receive GYRO_XOUT_L data

    Wire.beginTransmission(MPU_adress);
    Wire.write(0x43); // GYRO_XOUT_H
    Wire.endTransmission(MPU_adress);
    int GYRO_XOUT_H = Wire.read(); // Receive GYRO_XOUT_H data

    int gyro_xout = GYRO_XOUT_L + GYRO_XOUT_H - 255;

    Serial.print(gyro_xout);
  }
  delay(10); 
}
