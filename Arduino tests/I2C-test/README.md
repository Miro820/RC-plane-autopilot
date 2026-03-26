

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