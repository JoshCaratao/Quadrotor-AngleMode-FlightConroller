#include "Arduino.h"
#include "attitudeData.h"
#include "system.h"
#include "IMUStruct.h"

#define RAD_TO_DEG (180/3.1415926535897932384626433832795) //Define constant for converting from radians to degrees

System::System(float alpha){ //Constructor inititialization
  //Initialize Accelerometer and Gyro States to 0 
  systemState.accRoll = 0;
  systemState.accPitch = 0;
  systemState.gyroRoll = 0;
  systemState.gyroPitch = 0;
  systemState.gyroYaw = 0;

  //Initialize fused system state to 0
  systemState.Roll = 0;
  systemState.Pitch = 0;

  //initialize prevTime as 0
  prevTime = 0;

  //initialize values for complementary filter weights
  Kg = alpha;
  Ka = 1 - alpha; 
}


attitudeData System::computeState(IMUStruct IMUData){
  //First compute system state according to invividual sensor
  computeGyroState(IMUData.GyroX, IMUData.GyroY, IMUData.GyroZ);
  computeAccState(IMUData.AccX, IMUData.AccY, IMUData.AccZ);

  //Implement complementary filter as a sensor fusion algorithm for more accurate state estimation

  return systemState;
}


void System::computeGyroState(float gyroX, float gyroY, float gyroZ){

  unsigned long currentTime = micros();
  
  double deltaT = ((currentTime - prevTime) / 1000000); //Calculate change in times since last change in GyroState IN SECONDS

  //Calculate new state according to gyro by integrating angular velocities.
  systemState.gyroRoll = systemState.gyroRoll + (gyroX * deltaT);
  systemState.gyroPitch = systemState.gyroPitch + (gyroY * deltaT);
  systemState.gyroYaw = systemState.gyroYaw + (gyroZ * deltaT);

  prevTime = currentTime; //update prevTime for next iteration
}


void System::computeAccState(float accX, float accY, float accZ){

  systemState.accPitch = atan2(accY,accZ) * RAD_TO_DEG;
  systemState.accRoll = atan2(-1 * accX, sqrt(accY*accY + accZ*accZ)) * RAD_TO_DEG;

}

