
#include <Wire.h>

int MPU_adress = 0x68;
int MPU_Wake = 0x6B; //Register for waking up the sensor
int MPU_Config = 0x1A; //Register for configurating sensor

void setup() {
  Wire.begin(); //start I²C communication
  Serial.begin(115200);

  Wire.beginTransmission(MPU_adress); // Sart transmitting to the MPU6050
  Wire.write(MPU_Wake); // Select the PWR_MGMT_1 register
  Wire.write(0x00); // Wake up the sensor, by sending 0
  Wire.endTransmission(); // Since MPU_wake is 0x6B, the selcected register is now 0x6C... ==> start a new transmission
  
  Wire.beginTransmission(MPU_adress);
  Wire.write(MPU_Config); // Select the CONFIG register
  Wire.write(0x04); // Set Digital Low Pass Filter (DLPF) to 3
  Wire.endTransmission();

}

void loop() {
    Serial.println(read_MPU());
  delay(10); 
}


int16_t read_MPU() {
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x43); // select the GYRO_XOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t GYRO_XOUT_H = Wire.read(); // Receive GYRO_XOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t GYRO_XOUT_L = Wire.read(); // Receive GYRO_XOUT_L data

  int16_t gyro_xout = (GYRO_XOUT_H << 8) | GYRO_XOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte
  return gyro_xout;
}