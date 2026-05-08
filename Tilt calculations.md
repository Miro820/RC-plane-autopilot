
calculate alpha (=roll) with x & y coordinates:
``int alpha = atan2(mpu_full_read[4], mpu_full_read[6])* RAD_TO_DEG;``

Calculate xy-vector:
``int xz = sqrt(pow(mpu_full_read[4], 2) + pow(mpu_full_read[6], 2));``
calculate beta (=tilt) with xy & z coordinates:
``int beta = atan2(mpu_full_read[5], xz)*RAD_TO_DEG;``
