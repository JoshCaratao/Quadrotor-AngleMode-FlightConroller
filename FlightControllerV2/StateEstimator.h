//This class is responsible for representing the system itself and storing and computing the system state (attitude)

#ifndef StateEstimator_h
#define StateEstimator_h

#include "Arduino.h"

class StateEstimator{

  public:
    StateEstimator(float alpha); //Constructor

    //Declare public function for computing the system attitude/state. 
    void computeState(float gyroX, float gyroY, float gyroZ, float accX, float accY, float accZ);

    //Declare public variables
    float gyroRoll, gyroPitch, gyroYaw;  //individual calculated gyro and and acc state values
    float accRoll, accPitch, accYaw;

    float Roll, Pitch, Yaw;             //system state angles according to complementary filter

  
  private:

    //Declare functions for computing system state accoring to the gyroscopes and accelerometers separately
    void computeGyroState(float gyroX, float gyroY, float gyroZ);
    void computeAccState(float accX, float accY, float accZ);

    //Declare necessary variables
    double prevTime;

    //Declare complementary filter weights. Kg is weight for gyro and Ka is weight for accelerometer
    float Kg;
    float Ka;
};

#endif