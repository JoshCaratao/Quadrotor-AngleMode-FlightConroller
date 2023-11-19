#include <Wire.h>
//#include "I2Cdev.h"

//========================================================================================================================//
//                                  Joshua Caratao's Quadcopter Flight Controller Code                                    // 
//      All methods have either been written completely by myself or learned and adapted from other helpful resources.    //
//========================================================================================================================//

//Declare Radio Reciever Pins
// const int Chn_1_Pin;  //Throttle
// const int Chn_2_Pin;  //Elevator
// const int Chn_3_Pin;  //Aileron
// const int Chn_4_Pin;  //Rudder
// const int Chn_5_Pin;  //Gear (Throttle Cut)
// const int Chn_6_Pin;  // Aux channel

//Declare ESC Output Pins
// const int Motor_1_Pin;
// const int Motor_2_Pin;
// const int Motor_3_Pin;
// const int Motor_4_Pin;


//Declare IMU Input Pin
//const int MPU = 0x68;


//Declare IMU Raw Data Variables
long AccX, AccY, AccZ;
long GyroX, GyroY, GyroZ;

//Declare the actual calculated IMU Values
float gForceX, gForceY, gForceZ;
float rotX, rotY, rotZ;
//float oldRotX, oldRotY, oldRotZ;

//Declare the Estimated Attitude Angles according to Gyro
float gyroAngleX = 0;   
float gyroAngleY = 0;
float gyroAngleZ = 0;

//Declare Estimated AttitudeAngles According to Accelerometer
float accelAngleX = 0;
float accelAngleY = 0;

//These adjust the full scale Range of the gyro and accel in the IMU. 
//Change these from a value from 1-4, which 4 being highest range, but least sensitivity
const int accFullScaleRange = 1;
const int gyroFullScaleRange = 4;

//Declare Accelerometer and Gyro LSB values for IMU
float aLSB; 
float gLSB;

//Declare Accelerometer and Gyro Offset Values 
float accOffsetX = 0;
float accOffsetY = 0;
float accOffsetZ = 0;

float gyroOffsetX = 0;
float gyroOffsetY = 0;
float gyroOffsetZ = 0;

//Declare time variables
unsigned long gyroLastUpdate = micros();
double deltaT;

//Define Complementary Filter Constants
//#define gyroComplement 0.995
//#define accelComplement 0.005

//Define Radiants to Degree Constant
//#define RAD_TO_DEG (180/PI)


//========================================================================================================================//
//                                                      VOID SETUP                                                        //                           
//========================================================================================================================//

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(); //This initializes I2C communication. It is absolutely essential
  setLSB(); //This just sets the LSB values according to the FullScaleRange chosen above
  setupMPU(); //sets up the MPU for reading
  calibrateIMU(); //After the program starts, set the craft on a flat surface to calibrate IMU for 5 seconds
  readIMU();

}





//========================================================================================================================//
//                                                       MAIN LOOP                                                        //                           
//========================================================================================================================//
                                                  
void loop() {
  // put your main code here, to run repeatedly:
  //readIMU();
  gyroLastUpdate = micros();
  computeGyroAngles();
  computeAccelAngles();
  //printData(); //Print out IMU Data
  //plotData();
  plotAngles();
  delay(50);
}


//========================================================================================================================//
//                                               SEPARATE FUNCTIONS SECTION                                               //                           
//========================================================================================================================//


//========================================================================================================================//
//                                               IMU FUNCTIONS                                                            //                           
//========================================================================================================================//

//This Function establishes communication with an MPU and sets up all the registers which we will be using to read the data from the IMU
void setupMPU(){
  Wire.beginTransmission(0b1101000); //This is the I2C address of the IMU
  Wire.write(0x6B); //Accessing the register 6B - Power management (Sec. 4.28) This register allows the user to configure the power mode.
  //From the datasheet, we know that the device starts up in sleep mode. Hence, we acccess this register to change that.
  Wire.write(0b00000000); //Now that we selected which register we wanted to write to, we can write what value we want to write into it. 
  //In this case, we write all 0s to make sure that we are not in the sleep mode (look at MPU 6050 datasheet)
  Wire.endTransmission();

//Next, we need to change the range of our measurements and the sensitivity of Gyroscope and Accelerometer
//Things to consider, are you going to have spontaneous movements? how much RPM do you expect to be going?
  Wire.beginTransmission(0b1101000);
  Wire.write(0x1B); //Accessing the register 1B - Gyroscope configuration
  if(gyroFullScaleRange == 1){
    Wire.write(0x00000000); //Setting the gyro to full scale +/- 250deg (This is the lowest, but gives the most sensitivity)
    //This changes the range by writing 0 to Bit 4 and 3 to the gyro register
  }
  else if(gyroFullScaleRange == 2){
    Wire.write(0x00001000); //Setting the gyro to full scale +/- 500deg 
    //This changes the range by writing 1 to Bit 4 and 3 to the gyro register
  }
  else if(gyroFullScaleRange == 3){
    Wire.write(0x00010000); //Setting the gyro to full scale +/- 1000deg 
    //This changes the range by writing 2 to Bit 4 and 3 to the gyro register
  }
  else if(gyroFullScaleRange == 4){
    Wire.write(0x00011000); //Setting the gyro to full scale +/- 2000deg
    //This changes the range by writing 3 to Bit 4 and 3 to the gyro register 
  }
  Wire.endTransmission();

  Wire.beginTransmission(0b1101000);
  Wire.write(0x1C); //Accessing the register 1C - Accelerometer configuration
  if(accFullScaleRange == 1){
    Wire.write(0x00000000); //Setting the accel to full scale +/- 2g (This is the lowest, but gives the most sensitivity)
    //This changes the range by writing 0 to Bit 4 and 3 to the accel register
  }
  else if(accFullScaleRange == 2){
    Wire.write(0x00001000); //Setting the accel to full scale +/- 4g 
    //This changes the range by writing 1 to Bit 4 and 3 to the accel register
  }
  else if(accFullScaleRange == 3){
    Wire.write(0x00010000); //Setting the accel to full scale +/- 8g 
    //This changes the range by writing 2 to Bit 4 and 3 to the accel register
  }
  else if(accFullScaleRange == 4){
    Wire.write(0x00011000); //Setting the accel to full scale +/- 16g
    //This changes the range by writing 3 to Bit 4 and 3 to the accel register 
  }
  Wire.endTransmission();

  //NOTE Look at data sheet to see how to configure to a different full scale range above.
  //NOTE: You can change the full scale range of the Accel and Gyro by changing the variable in teh variable declaration section

}


//This function  reads the data from the IMU and calls the processIMU function to compute the values of acceleration and rotational velocity. 
void readIMU(){
  //NOTE: According to the datasheet. The actual gyro and accel values we want must be calculated from the raw readings by 
  //dividing by the LSB/g value, which depends on the FullScaleRange used.


  //Read Accelerometer 
  Wire.beginTransmission(0b1101000); //The I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (3B - 40). here we are requesting 6 registers from 3B to 40, which gives the Accel outputs
  while(Wire.available() < 6);
  AccY = Wire.read()<<8|Wire.read(); //Store first two bytes into a 
  AccX = Wire.read()<<8|Wire.read(); //Store middle two bytes into a 
  AccZ = Wire.read()<<8|Wire.read(); //Store last two two bytes into a 

  //Read Gyroscope 
  Wire.beginTransmission(0b1101000); //The I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (43-48). here we are requestion 6 register from 43 to 48, which gives the Gyro raw data output
  while(Wire.available() < 6);
  GyroY = Wire.read()<<8|Wire.read(); //Store first two bytes into a 
  GyroX = Wire.read()<<8|Wire.read(); //Store middle two bytes into a 
  GyroZ = Wire.read()<<8|Wire.read(); //Store last two two bytes into a 

  processIMU(); //Take the raw values and process them to get rotational rates/acceleration
  
}


//This sets the LSB/g and LSB/deg/s values used to calculate data from IMU
void setLSB(){
  
  //Set Accel LSB
  if(accFullScaleRange == 1){
    aLSB = 16384.0;
  }
  else if (accFullScaleRange == 2){
    aLSB = 8192.0;
  }
  else if (accFullScaleRange == 3){
    aLSB = 4096.0;
  }
  else if (accFullScaleRange == 4){
    aLSB = 2048.0;
  }

  //Set Gyro LSB
  if(gyroFullScaleRange == 1){
    gLSB = 131.0;
  }
  else if (gyroFullScaleRange == 2){
    gLSB = 65.5;
  }
  else if (gyroFullScaleRange == 3){
    gLSB = 32.8;
  }
  else if (gyroFullScaleRange == 4){
    gLSB = 16.4;
  }

}


//This function takes the raw IMU Data and processes them into useable values
void processIMU() {

  //Compute Accel
  gForceX = (AccX / aLSB) - accOffsetX ;
  gForceY = (AccY / aLSB) - accOffsetY;
  gForceZ = (AccZ / aLSB) - accOffsetZ; 

  //Compute Gyro
  rotX = ((GyroX / gLSB) - gyroOffsetX);
  rotY = (GyroY / gLSB) - gyroOffsetY;
  rotZ = (GyroZ / gLSB) - gyroOffsetZ;

}


//This function calculates the acceleration and gyro offsets .
void calibrateIMU(){
  //Set begin time
  float time1 = millis();

  //Sets the total of the IMU values recorded to be averaged
  float gForceTotalX = 0;
  float gForceTotalY = 0;
  float gForceTotalZ = 0;
  float rotTotalX = 0;
  float rotTotalY = 0;
  float rotTotalZ = 0;

  Serial.print("Calibration in Progress...");

  //Set begin time
  float time1 = millis();

  int samples = 0;
  while(((millis() - time1)/ (1000)) < 5){
    readIMU();
    
    gForceTotalX += abs(gForceX);
    gForceTotalY += abs(gForceY);
    gForceTotalZ += abs(gForceZ);

    rotTotalX += abs(rotX);
    rotTotalY += abs(rotY);
    rotTotalZ += abs(rotZ);  

    //increase the sample size by 1 each each loop
    samples += 1;
  }

  //Compute the averages of each IMU value over a 5 second time period (or whatever time period works best) and set the offsets.
  accOffsetX = (gForceTotalX / samples);
  accOffsetY = (gForceTotalY / samples);
  accOffsetZ = (gForceTotalZ / samples) + 1; //Add 1 here to calibrate the z-acceleration to 1G (gravity)

  gyroOffsetX = (rotTotalX / samples);
  gyroOffsetY = (rotTotalY / samples);
  gyroOffsetZ = (rotTotalZ / samples); 

  Serial.println();
  Serial.println("...Calibration Complete...");

  delay(2000);
}



//========================================================================================================================//
//                                               ATTITUDE ESTIMATION FUNCTIONS                                            //                           
//========================================================================================================================//


//Estimate Attitude using Gyro
void computeGyroAngles() {
  //Use trapezoidal numerical integration of angular Velocity to get angular position
  // float oldRotX = rotX;
  // float oldRotY = rotY;
  // float oldRotZ = rotZ;

  //Read IMU to get new angular velocity values
  readIMU();

  deltaT = ((micros() - gyroLastUpdate) / 1000000.0); //Calculate Change in time since last gyro update in seconds
  
  
  gyroAngleX += (rotX * deltaT);
  gyroAngleY += (rotY * deltaT);
  gyroAngleZ += (rotZ * deltaT);

  //This is the old way (trapezoidal method)
  // gyroAngleX += ((0.5) * ((micros() - gyroLastUpdate)/1000000) * (oldRotX + rotX));
  // gyroAngleY += ((0.5) * ((micros() - gyroLastUpdate)/1000000) * (oldRotY + rotY));
  // gyroAngleZ += ((0.5) * ((micros() - gyroLastUpdate)/1000000) * (oldRotZ + rotZ));

  //Update the last time the angles were computed
  gyroLastUpdate = micros();


}

//Compute Attitude (roll and pitch only) using Accelerometer
void computeAccelAngles(){
  //Note: should use a median filter, implement later, for now just use formulas

  //Read IMU to get new Accel Values
  readIMU();

  accelAngleX = atan2(gForceY,gForceZ) * RAD_TO_DEG;
  accelAngleY = atan2(-1 * gForceX, sqrt(gForceY*gForceY + gForceZ*gForceZ)) * RAD_TO_DEG;
  
}
































                                                  
