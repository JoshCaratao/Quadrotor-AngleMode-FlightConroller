//This is used to create a struct object for storing systems's attitude/state

#ifndef ALTITUDE_DATA_H
#define ALTITUDE_DATA_H

struct attitudeData {
  float gyroRoll, gyroPitch, gyroYaw; //system states according to gyroscope
  float accRoll, accPitch; //roll and pitch according to accelerometer
  float Roll, Pitch;  //Fused Sensor data (more accurate representation of system state)
};

#endif