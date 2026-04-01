
#include <Wire.h>

int MPU_adress = 0x68;
int MPU_Wake = 0x6B;

void setup() {
  Wire.begin(); //start I²C communication
  Serial.begin(115200);

  Wire.beginTransmission(MPU_adress);
  Wire.write(0x6B); // Select the PWR_MGMT_1 register
  Wire.write(0x00); // Wake up the sensor, by sending 0
  Wire.endTransmission();

}

void loop() {
    //Serial.println(read_MPU());
    Serial.println(read_MPU(), DEC);
  delay(10); 
}


int16_t read_MPU() {
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x43); // GYRO_XOUT_H
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t GYRO_XOUT_H = Wire.read(); // Receive GYRO_XOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t GYRO_XOUT_L = Wire.read(); // Receive GYRO_XOUT_L data

  int16_t gyro_xout = (GYRO_XOUT_H << 8) | GYRO_XOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte
  return gyro_xout;
}