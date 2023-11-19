//This is used to create a struct object for use in both the main flight controller and in the IMU Library to store IMU Data

#ifndef IMU_DATA_H
#define IMU_DATA_H

struct IMUStruct {
  float AccX;
  float AccY;
  float AccZ;
  float GyroX;
  float GyroY;
  float GyroZ;
};

#endif