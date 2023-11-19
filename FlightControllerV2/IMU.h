#ifndef IMU_h
#define IMU_h

#include "Arduino.h"
#include <Wire.h>
#include "IMUStruct.h"

class IMU {

  public:
    IMU(); //Constructor
    void setupIMU(int accFullScaleRange, int gyroFullScaleRange);
    void setLSB(int accFullScaleRange, int gyroFullScaleRange);
    void calibrateIMU();
    IMUStruct readIMU();
    


  private:
  //Declare IMU Raw Data Variables
  long AccX_RAW, AccY_RAW, AccZ_RAW;
  long GyroX_RAW, GyroY_RAW, GyroZ_RAW;

  //Declare Accelerometer and Gyro LSB values for IMU
  float aLSB; 
  float gLSB;

  //Declare Accelerometer and Gyro Offset Values 
  float accOffsetX;
  float accOffsetY;
  float accOffsetZ;

  float gyroOffsetX;
  float gyroOffsetY;
  float gyroOffsetZ;

  //Declare Struct to store IMUData 
  IMUStruct IMUData;


};

#endif