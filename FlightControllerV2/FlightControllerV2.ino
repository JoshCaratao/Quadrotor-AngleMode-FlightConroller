#include <Wire.h>
//#include "I2Cdev.h"
//#include "IMU.h"
//#include "IMUStruct.h"
#include "MyMPU6050.h"
#include "StateEstimator.h"
//#include "attitudeData.h"
//#include "system.h"

//========================================================================================================================//
//                                  Joshua Caratao's Quadcopter Flight Controller Code V2.0                               // 
//      All methods have either been written completely by myself or learned and adapted from other helpful resources.    //
//========================================================================================================================//


//These adjust the full scale Range of the gyro and accel in the IMU. 
//Change these from a value from 1-4, which 4 being highest range, but least sensitivity
const int accFullScaleRange = 1;
const int gyroFullScaleRange = 1;

//Inititialize IMU Object Instance
MyMPU6050 systemIMU;

//Declare the Estimated Attitude Angles according to Gyro
float gyroAngleX = 0;   
float gyroAngleY = 0;
float gyroAngleZ = 0;

//Declare Estimated Attitude Angles According to Accelerometer
float accelAngleX = 0;
float accelAngleY = 0;

//initialize struct for storing IMU Data
//IMUStruct IMUData;

//Initialize struct for storing drone attitude estimations
//attitudeData systemState;

//Initalize value of complemetary filter gains. Change as needed for tuning. This represents the weight of the Gyroscope. The weight of the accelerometer will just be (1-alpha)
float alpha = 0.98;

double prevTime = micros();

void setup() {
  Serial.begin(9600);
  Wire.begin(); //This initializes I2C communication. It is absolutely essential

  //IMU Setup and calibration functions
  systemIMU.setupMPU6050(accFullScaleRange, gyroFullScaleRange);
  Serial.println("IMU Initialized...");
  delay(1000);
  systemIMU.setLSB(accFullScaleRange, gyroFullScaleRange);
  Serial.println("LSB Set...");
  delay(1000);
  systemIMU.calibrateMPU6050();
  
  //initialize instance of System class for calculating system state/attitude and pass in filter weight. I do it in here so that the timer for gyro state estimation doesnt begin too early.
  StateEstimator state(alpha);

}


void loop() {
  systemIMU.readMPU6050();

  //Send Serial Data to Matlab at specific hz
  // if(((micros() - prevTime) / 1000000) >= 0.1){
  //   sendSerialIMUData();
  // }

  printData();
  plotData();
  
}



//Send Serial Data to Matlab
void sendSerialIMUData(){
  String serialString = String(systemIMU.AccX) + ',' + String(systemIMU.AccY) + ',' + String(systemIMU.AccZ) + ',' + String(systemIMU.GyroX) + ',' + String(systemIMU.GyroY) + ',' + String(systemIMU.GyroZ);

  Serial.println(serialString);
}



//Prints out the IMU Data 
void printData(){
  Serial.print("Gyro(deg/s)");
  Serial.print(" X=");
  Serial.print(systemIMU.GyroX);
  Serial.print(" Y=");
  Serial.print(systemIMU.GyroY);
  Serial.print(" Z=");
  Serial.print(systemIMU.GyroZ);

  Serial.print(" | ");

  Serial.print(" Accel(g)");
  Serial.print(" X=");
  Serial.print(systemIMU.AccX);
  Serial.print(" Y=");
  Serial.print(systemIMU.AccY);
  Serial.print(" Z=");
  Serial.print(systemIMU.AccZ);

  Serial.println(" ");

}


void plotData(){
  // Send data to Serial Plotter
  Serial.print("Accel_X:");
  Serial.print(systemIMU.AccX);
  Serial.print(", ");

   Serial.print("Accel_Y:");
  Serial.print(systemIMU.AccY);
  Serial.print(", ");

  Serial.print("Accel_Z:");
  Serial.print(systemIMU.AccZ);
  Serial.print(", ");

  Serial.print("gyro_X:");
  Serial.print(systemIMU.GyroX);
  Serial.print(", ");

  Serial.print("gyro_Y:");
  Serial.print(systemIMU.GyroY);
  Serial.print(", ");

  Serial.print("gyro_Z:");
  Serial.println(systemIMU.GyroZ);

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

