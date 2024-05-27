#include "Arduino.h"
#include "StateEstimator.h"


#define RAD_TO_DEG (180/3.1415926535897932384626433832795) //Define constant for converting from radians to degrees

StateEstimator::StateEstimator(float alpha){ //Constructor inititialization
  //Initialize Accelerometer and Gyro States to 0 
  accRoll = 0;
  accPitch = 0;
  gyroRoll = 0;
  gyroPitch = 0;
  gyroYaw = 0;

  //Initialize fused system state to 0
  Roll = 0;
  Pitch = 0;
  Yaw = 0;

  //initialize prevTime as 0
  prevTime = 0;

  //initialize values for complementary filter weights
  Kg = alpha;
  Ka = 1 - alpha; 
}


void StateEstimator::computeState(float gyroX, float gyroY, float gyroZ, float accX, float accY, float accZ){
  //First compute system state according to invividual sensor
  computeGyroState(gyroX, gyroY, gyroZ);
  computeAccState(accX, accY, accZ);

  //Implement complementary filter to fuse the sensor data/estimation from the gyroscope with accelerometer. 
  Roll = (Kg * gyroRoll) + (Ka * accRoll);
  Pitch = (Kg * gyroPitch) + (Ka * accPitch);
  Yaw = gyroYaw; // Assuming no other sensor data for yaw 
}


void StateEstimator::computeGyroState(float gyroX, float gyroY, float gyroZ){

  unsigned long currentTime = micros();
  
  double deltaT = ((currentTime - prevTime) / 1000000); //Calculate change in times since last change in GyroState IN SECONDS

  //Calculate new state according to gyro by integrating angular velocities.
  gyroRoll = gyroRoll + (gyroY * deltaT);
  gyroPitch = gyroPitch + (gyroX * deltaT);
  gyroYaw = gyroYaw + (gyroZ * deltaT);

  prevTime = currentTime; //update prevTime for next iteration
}


void StateEstimator::computeAccState(float accX, float accY, float accZ){

  accPitch = atan2(accY,accZ) * RAD_TO_DEG;
  accRoll = atan2(-1 * accX, sqrt(accY*accY + accZ*accZ)) * RAD_TO_DEG;

}
