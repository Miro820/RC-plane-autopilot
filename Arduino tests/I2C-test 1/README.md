## 3. I²C test

> [!WARNING]  
> Deze Arduino code werkt niet!!
> Deze code heeft tot inzichtten geleid, maar is niet functioneel... Voor de eerst volgende werkende testcode zie [I2C_working_MPU6050](..\I2C_working_MPU6050\README.md).



| MPU6050 Pin | Arduino Pin | Why |
|------|------|------|
| VCC | 3.3V or 5V | Power |
| GND | GND | Ground |
| SCL | A5 (Uno) / SCL | I2C clock |
| SDA | A4 (Uno) / SDA | I2C data |
| XDA | - | **Unused** (for daisy-chaining another I2C sensor) |
| XCL | - | **Unused** (for daisy-chaining another I2C sensor) |
| ADO | - | **Unused** (0V => address 0x68; 5V => address 0x69; — only needed with two MPU-6050s on the same I²C wire) |
| INT | - | **Unused** (Interupt-pin ???) |