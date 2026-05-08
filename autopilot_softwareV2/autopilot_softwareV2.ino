// the whole thing

// ch1: <R>
// ch2: ^Rv
// ch3: ^Lv
// ch4: <L>
// ch5: VRA

// elevator = ch2
// rudder = ch1 ==> direct connect
// left aileron = ch4
// right aileron = ch4
// throttle = ch3 ==> direct connect

//-----------------------------------------------------------------


// #TODO
// alle comments nalezen en correcten...



#include <Wire.h>
#include <math.h>
#include <Servo.h>


// USER SETTINGS
int levelling_range = 5;        // int from 0 to 40 that controls the range where the plane auto-levels. (bigger = less control & more auto levelling)
float gain1 = 1;                // int between 0.5 and 1.2 to control the how much the servo moves, for every degree of MPU rotation.
bool rev1 = true;               // change to true/false, to reverse the levelling direction.

// #TODO // int levelling_range = 5;        // int from 0 to 40 that controls the range where the plane auto-levels. (bigger = less control & more auto levelling)
float gain2 = 1;                // int between 0.5 and 1.2 to control the how much the servo moves, for every degree of MPU rotation.
bool rev2 = true;               // change to true/false, to reverse the levelling direction.
// END USER SETTINGS


const int ch2_in = 2;
const int ch4_in = 3;

int MPU_adress = 0x68;
int MPU_Wake = 0x6B;            //Register for waking up the sensor
int MPU_Config = 0x1A;          //Register for configurating sensor

int16_t mpu_full_read[7];       // create a global array, that will be used for storing all the sensor data

Servo servo_elevator;           // create Servo object to control a servo
Servo servo_right_aileron;
Servo servo_left_aileron;


// ----------------PWM reading logic----------------

// ISR (Interrupt Service Routine) variables for ch2 and ch4
// 'volatile' tells the compiler these can change at any time (inside an interrupt), so it should never cache them
volatile unsigned long ch2_start = 0, ch4_start = 0;   // stores the timestamp of the rising edge
volatile int ch2_pulse = 1500, ch4_pulse = 1500;        // stores the measured pulse width in microseconds (default 1500 = center)

void ch2_ISR() {
  if (digitalRead(ch2_in)) ch2_start = micros();        // rising edge: record the start time
  else ch2_pulse = micros() - ch2_start;                // falling edge: calculate pulse width by subtracting start time from current time
}

void ch4_ISR() {
  if (digitalRead(ch4_in)) ch4_start = micros();        // rising edge: record the start time
  else ch4_pulse = micros() - ch4_start;                // falling edge: calculate pulse width by subtracting start time from current time
}


void setup() {
  Wire.begin();                 //start I²C communication
  Serial.begin(115200);

  Wire.beginTransmission(MPU_adress); // Sart transmitting to the MPU6050
  Wire.write(MPU_Wake);         // Select the PWR_MGMT_1 register
  Wire.write(0x00);             // Wake up the sensor, by sending 0
  Wire.endTransmission();       // Since MPU_wake is 0x6B, the selcected register is now 0x6C... ==> start a new transmission
  
  Wire.beginTransmission(MPU_adress);
  Wire.write(MPU_Config);       // Select the CONFIG register
  Wire.write(0x04);             // Set Digital Low Pass Filter (DLPF) to 3
  Wire.endTransmission();

  pinMode(ch2_in, INPUT);
  pinMode(ch4_in, INPUT);
  servo_elevator.attach(5);     // attach the servo's on pin x to the Servo object
  servo_right_aileron.attach(6);
  servo_left_aileron.attach(9);

  attachInterrupt(digitalPinToInterrupt(ch2_in), ch2_ISR, CHANGE);  // whenever pin ch2_in changes (high→low or low→high), immediately call ch2_ISR()
  attachInterrupt(digitalPinToInterrupt(ch4_in), ch4_ISR, CHANGE);
  
}

void loop() {
  
  // Read RC-inputs first
  // Read the PWM signal of ch2 + convert to degree's
  int ch2 = pwm_to_degree(ch2_pulse);
  int ch4 = pwm_to_degree(ch4_pulse);
  
  // Read MPU
  read_MPU();
  int alpha = atan2(mpu_full_read[4], mpu_full_read[6])* RAD_TO_DEG;  // calculate alpha (=roll) with x & y coordinates
  alpha = constrain(alpha + 90, 0, 180);  // Convert alpha from [-180°, 180°] to [0°, 180°] by clipping (this way the same sesitivity is kept, as beta)
  int xz = sqrt(pow(mpu_full_read[4], 2) + pow(mpu_full_read[6], 2));
  int beta = atan2(mpu_full_read[5], xz)* RAD_TO_DEG;  // calculate beta (=tilt) with xy & z coordinates
  beta = beta + 90;                  // Convert beta from [-90°, 90°] to [0°, 180°]

  // Create servo-position variables
  int pos1;
  int pos2;



  //------------------------------------------------
  // ----------------Elevator logic-----------------

  if (ch2 >= (90 - levelling_range) && ch2 <= (90 + levelling_range)) {
    pos1 = beta*gain1;
    if (rev1 == true) pos1 = 180 - pos1;          // Reverse levelling direction if rev1 is set to true
  }

  else {
    pos1 = ch2;
  }

  

  //-------------------------------------------------
  //------------------Aileron logic------------------

  if (ch4 >= (90 - levelling_range) && ch4 <= (90 + levelling_range)) {
    pos2 = alpha*gain2;
    if (rev2 == true) pos2 = 180 - pos2;          // Reverse levelling direction if rev2 is set to true
  }

  else {
    pos2 = ch4;
  }



  // make sure that pos1 & pos2 are valid
  pos1  = constrain(pos1, 0, 180);
  pos2  = constrain(pos2, 0, 180);

  // Write to the servos
  servo_elevator.write(pos1);     // tell servo to go to position pos1 (in degree)
  servo_right_aileron.write(pos2);     // tell servo to go to position pos2 (in degree)
  servo_left_aileron.write(180 - pos2);       // do the oposite of right aileron (pos2)


  delay(20);                      // waits 20 ms for the servo's to reach their position

}



int pwm_to_degree(int pwm_signal) {   // convert PWM-signal to degree's (0°-180°)
  if (pwm_signal == 0) return 90;  // default to center on timeout
  return map(pwm_signal, 1000, 2000, 0, 180);
}



void read_MPU() {

  //______________________________________
  //______________________________________
  // ------ GYRO -------------------------
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x43); // select the GYRO_XOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t GYRO_XOUT_H = Wire.read(); // Receive GYRO_XOUT_H data (use 16 bits, so the data can be shifted by 8 bits to the left, without data loss 0000 1010 => 1010 0000)
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
  Wire.write(0x3D); // select the ACCEL_YOUT_H register
  Wire.endTransmission(); 
  Wire.requestFrom(MPU_adress, 2); // request 2 bytes of data from MPU_adress
  int16_t ACCEL_YOUT_H = Wire.read(); // Receive ACCEL_YOUT_H data (use 16 bits, so the data can be shifted by 6 bits to the left, without data loss 0000 1010 => 1010 0000)
  uint8_t ACCEL_YOUT_L = Wire.read(); // Receive ACCEL_YOUT_L data
  int16_t accel_yout = (ACCEL_YOUT_H << 8) | ACCEL_YOUT_L; // Shift high byte with 8 bits to the left... Then use "or" to combine with the lower byte
   
  Wire.beginTransmission(MPU_adress);
  Wire.write(0x3F); // select the ACCEL_ZOUT_H register
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
