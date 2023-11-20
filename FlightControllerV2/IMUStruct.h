//This is used to create a struct object for use in both the main flight controller and in the IMU Library to store IMU Data

#ifndef IMUSTRUCT_H
#define IMUSTRUCT_H

struct IMUStruct {
  float AccX;
  float AccY;
  float AccZ;
  float GyroX;
  float GyroY;
  float GyroZ;
};

#endif