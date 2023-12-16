#ifndef MyMPU6050_h
#define MyMPU6050_h

#include "Arduino.h"
#include <Wire.h>

class MyMPU6050 {

  public:
    MyMPU6050(); //Constructor
    void setupMPU6050(int accFullScaleRange, int gyroFullScaleRange);
    void setLSB(int accFullScaleRange, int gyroFullScaleRange);
    void calibrateMPU6050();
    void readMPU6050();
    
    //Declare IMU Processed Data Variables publically
    float AccX, AccY, AccZ;
    float GyroX, GyroY, GyroZ;


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

};



#endif