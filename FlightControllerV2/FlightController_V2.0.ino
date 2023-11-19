#include <Wire.h>
//#include "I2Cdev.h"
#include "IMU.h"
#include "IMUStruct.h"

//========================================================================================================================//
//                                  Joshua Caratao's Quadcopter Flight Controller Code V2.0                               // 
//      All methods have either been written completely by myself or learned and adapted from other helpful resources.    //
//========================================================================================================================//


//These adjust the full scale Range of the gyro and accel in the IMU. 
//Change these from a value from 1-4, which 4 being highest range, but least sensitivity
const int accFullScaleRange = 1;
const int gyroFullScaleRange = 4;

//Inititialize IMU Object Instance
IMU droneIMU;

//Declare the Estimated Attitude Angles according to Gyro
float gyroAngleX = 0;   
float gyroAngleY = 0;
float gyroAngleZ = 0;

//Declare Estimated Attitude Angles According to Accelerometer
float accelAngleX = 0;
float accelAngleY = 0;

//Declare Angular Velocities and Accelerations from IMU
// float AccX, AccY, AccZ;
// float GyroX, GyroY, GyroZ;
IMUStruct IMUData;



void setup() {
  Serial.begin(9600);
  Wire.begin(); //This initializes I2C communication. It is absolutely essential

  //IMU Setup and calibration functions
  droneIMU.setupIMU(accFullScaleRange, gyroFullScaleRange);
  droneIMU.setLSB(accFullScaleRange, gyroFullScaleRange);
  droneIMU.calibrateIMU();
  
}

void loop() {

  IMUData = droneIMU.readIMU();

  printData();
  plotData();
  
  
}



//Prints out the IMU Data 
void printData(){
  Serial.print("Gyro(deg)");
  Serial.print(" X=");
  Serial.print(IMUData.GyroX);
  Serial.print(" Y=");
  Serial.print(IMUData.GyroX);
  Serial.print(" Z=");
  Serial.print(IMUData.GyroZ);

  Serial.print(" | ");

  Serial.print(" Accel(g)");
  Serial.print(" X=");
  Serial.print(IMUData.AccX);
  Serial.print(" Y=");
  Serial.print(IMUData.AccY);
  Serial.print(" Z=");
  Serial.print(IMUData.AccZ);

  Serial.println(" ");

}


void plotData(){
  Serial.print("gyro_X:");
  Serial.print(IMUData.GyroX);
  Serial.print(", ");

  Serial.print("gyro_Y:");
  Serial.print(IMUData.GyroY);
  Serial.print(", ");

  Serial.print("gyro_Z:");
  Serial.print(IMUData.GyroZ);
  Serial.print(", | ");

  Serial.print("Accel_X:");
  Serial.print(IMUData.AccX);
  Serial.print(", ");

  Serial.print("Accel_Y:");
  Serial.print(IMUData.AccY);
  Serial.print(", ");

  Serial.print("Accel_Z:");
  Serial.print(IMUData.AccZ);
  Serial.print(", ");

  Serial.println();
}


// void plotAngles() {
//   Serial.print("gyro_X:");
//   Serial.print(gyroAngleX);
//   Serial.print(", ");

//   Serial.print("gyro_Y:");
//   Serial.print(gyroAngleY);
//   Serial.print(", ");

//   Serial.print("gyro_Z:");
//   Serial.print(gyroAngleZ);
//   Serial.print(", | ");

//   Serial.print("accel_X:");
//   Serial.print(accelAngleX);
//   Serial.print(", ");

//   Serial.print("accel_Y:");
//   Serial.print(accelAngleY);
//   Serial.print(", ");

//   Serial.println();
// }

