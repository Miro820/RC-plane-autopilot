// trying out the angle calculations for Alpha and Beta



//-----------------------------------------------------------------

#include <Wire.h>
#include <math.h>

int MPU_adress = 0x68;
int MPU_Wake = 0x6B; //Register for waking up the sensor
int MPU_Config = 0x1A; //Register for configurating sensor

int16_t mpu_full_read[7]; // create a global array, that will be used for storing all the sensor data

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
  read_MPU();

  int alpha = atan2(mpu_full_read[4], mpu_full_read[5])* RAD_TO_DEG; // calculate alpha (=roll) with x & y coordinates
  
  int xy = sqrt(pow(mpu_full_read[4], 2) + pow(mpu_full_read[5], 2));
  int beta = atan2(mpu_full_read[6], xy)* RAD_TO_DEG; // calculate beta (=tilt) with xy & z coordinates
  //Serial.println(alpha);
  //Serial.print(",");
  Serial.println(beta);
}


int16_t read_MPU() {

  //______________________________________
  //______________________________________
  // ------ GYRO -------------------------
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x43); // select the GYRO_XOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t GYRO_XOUT_H = Wire.read(); // Receive GYRO_XOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t GYRO_XOUT_L = Wire.read(); // Receive GYRO_XOUT_L data
  int16_t gyro_xout = (GYRO_XOUT_H << 8) | GYRO_XOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte
  
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x45); // select the GYRO_YOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t GYRO_YOUT_H = Wire.read(); // Receive GYRO_YOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t GYRO_YOUT_L = Wire.read(); // Receive GYRO_YOUT_L data
  int16_t gyro_yout = (GYRO_YOUT_H << 8) | GYRO_YOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte
   
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x47); // select the GYRO_ZOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t GYRO_ZOUT_H = Wire.read(); // Receive GYRO_ZOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t GYRO_ZOUT_L = Wire.read(); // Receive GYRO_ZOUT_L data
  int16_t gyro_zout = (GYRO_ZOUT_H << 8) | GYRO_ZOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte

  //______________________________________
  //______________________________________
  // ------ Accelerometer ----------------
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x3B); // select the ACCEL_XOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t ACCEL_XOUT_H = Wire.read(); // Receive ACCEL_XOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t ACCEL_XOUT_L = Wire.read(); // Receive ACCEL_XOUT_L data
  int16_t accel_xout = (ACCEL_XOUT_H << 8) | ACCEL_XOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte
  
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x3F); // select the ACCEL_YOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t ACCEL_YOUT_H = Wire.read(); // Receive ACCEL_YOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t ACCEL_YOUT_L = Wire.read(); // Receive ACCEL_YOUT_L data
  int16_t accel_yout = (ACCEL_YOUT_H << 8) | ACCEL_YOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte
   
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x47); // select the ACCEL_ZOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t ACCEL_ZOUT_H = Wire.read(); // Receive ACCEL_ZOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t ACCEL_ZOUT_L = Wire.read(); // Receive ACCEL_ZOUT_L data
  int16_t accel_zout = (ACCEL_ZOUT_H << 8) | ACCEL_ZOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte

  //______________________________________
  //______________________________________
  // ------ Thermometer ------------------

  Wire.beginTransmission(MPU_adress);
  Wire.write(0x41); // select the TEMP_OUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t TEMP_OUT_H = Wire.read(); // Receive TEMP_OUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t TEMP_OUT_L = Wire.read(); // Receive TEMP_OUT_L data
  int16_t temp_out = (TEMP_OUT_H << 8) | TEMP_OUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte



  // save all data to the global array:
  mpu_full_read[0] = temp_out;
  mpu_full_read[1] = gyro_xout;
  mpu_full_read[2] = gyro_yout;
  mpu_full_read[3] = gyro_zout;
  mpu_full_read[4] = accel_xout;
  mpu_full_read[5] = accel_yout;
  mpu_full_read[6] = accel_zout;
}
