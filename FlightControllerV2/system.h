//This class is responsible for representing the system itself and storing and computing the system state (attitude)

#ifndef SYSTEM_h
#define SYSTEM_h

#include "Arduino.h"
#include "attitudeData.h"
#include "IMUStruct.h"

class System{

  public:
    System(float alpha); //Constructor

    //Declare Struct to store system states (roll, pitch, yaw) 
    attitudeData systemState;

    //Declare function for computing the system attitude/state. This returns a struct with attitude data and takes in a struct filled with IMUData
    attitudeData computeState(IMUStruct IMUData);

  

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