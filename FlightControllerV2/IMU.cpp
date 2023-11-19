#include "Arduino.h"
#include "IMU.h"
#include <Wire.h>
#include "IMUStruct.h"

IMU::IMU(){ //Constructor inititialization

  //Initialize Accelerometer and Gyro Offset Values 
  accOffsetX = 0;
  accOffsetY = 0;
  accOffsetZ = 0;
  gyroOffsetX = 0;
  gyroOffsetY = 0;
  gyroOffsetZ = 0;
 
}



//===========================================================================================================================================//
//                                                             IMU SETUP                                                                     //
//This Function establishes communication with an MPU6050 and sets up all the registers which we will be using to read the data from the IMU //                                                      //                           
//===========================================================================================================================================//
void IMU::setupIMU(int gyroFullScaleRange, int accFullScaleRange){
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




//========================================================================================================================================================================//
//                                                             IMU READING                                                                                                //
//This function  reads the data from the IMU and calls the processIMU function to compute the values of acceleration and rotational velocity, and then returns the values //                                                      //                           
//========================================================================================================================================================================//
IMUStruct IMU::readIMU(){
  //NOTE: According to the datasheet. The actual gyro and accel values we want must be calculated from the raw readings by 
  //dividing by the LSB/g value, which depends on the FullScaleRange used.

  //Read Accelerometer RAW Data
  Wire.beginTransmission(0b1101000); //The I2C address of the MPU
  Wire.write(0x3B); //Starting register for Accel Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (3B - 40). here we are requesting 6 registers from 3B to 40, which gives the Accel outputs
  while(Wire.available() < 6);
  AccY_RAW = Wire.read()<<8|Wire.read(); //Store first two bytes into a 
  AccX_RAW = Wire.read()<<8|Wire.read(); //Store middle two bytes into a 
  AccZ_RAW = Wire.read()<<8|Wire.read(); //Store last two two bytes into a 

  //Read Gyroscope RAW Data
  Wire.beginTransmission(0b1101000); //The I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (43-48). here we are requestion 6 register from 43 to 48, which gives the Gyro raw data output
  while(Wire.available() < 6);
  GyroY_RAW = Wire.read()<<8|Wire.read(); //Store first two bytes into a 
  GyroX_RAW = Wire.read()<<8|Wire.read(); //Store middle two bytes into a 
  GyroZ_RAW = Wire.read()<<8|Wire.read(); //Store last two two bytes into a 

  //Compute acceleration and rotational Rates *NOTE* Calibration is already handled in this function and further calibration is NOT NEEDED in main sketch
  //Compute Accel
  IMUData.AccX = (AccX_RAW / aLSB) - accOffsetX ;
  IMUData.AccY = (AccY_RAW / aLSB) - accOffsetY;
  IMUData.AccZ = (AccZ_RAW / aLSB) - accOffsetZ; 

  //Compute Gyro
  IMUData.GyroX = ((GyroX_RAW / gLSB) - gyroOffsetX);
  IMUData.GyroY = (GyroY_RAW / gLSB) - gyroOffsetY;
  IMUData.GyroZ = (GyroZ_RAW / gLSB) - gyroOffsetZ;

  return IMUData;

}



//===========================================================================================================================================//
//                                                             IMU SET LSB                                                                   //
//         This sets the LSB/g and LSB/deg/s values used to calculate data from IMU. This should be run before any other IMU Function        //                           
//===========================================================================================================================================//
void IMU::setLSB(int accFullScaleRange, int gyroFullScaleRange){
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



//===========================================================================================================================================//
//                                                             IMU Calibration                                                               //
//                                          This function calculates the acceleration and gyro offsets.                                      //                           
//===========================================================================================================================================//
void IMU::calibrateIMU(){
  const int numSamples = 1000; //Can adjust number of samples as needed
  
  //Sets the total of the IMU values recorded to be averaged
  float AccTotalX = 0;
  float AccTotalY = 0;
  float AccTotalZ = 0;
  float GyroTotalX = 0;
  float GyroTotalY = 0;
  float GyroTotalZ = 0;

  Serial.print("Calibration in Progress...");

  for(int i = 0; i<numSamples; i++){
    //Read data from IMU
    readIMU(); //Not storing returned struct here as I have a global struct that this function changes already

    AccTotalX += abs(IMUData.AccX);
    AccTotalY += abs(IMUData.AccY);
    AccTotalZ += abs(IMUData.AccZ);

    GyroTotalX += abs(IMUData.GyroX);
    GyroTotalY += abs(IMUData.GyroY);
    GyroTotalZ += abs(IMUData.GyroZ);  

    delay(5);
  }

  //Compute the averages of each IMU value over a 5 second time period (or whatever time period works best) and set the offsets.
  accOffsetX = (AccTotalX / numSamples);
  accOffsetY = (AccTotalY / numSamples);
  accOffsetZ = (AccTotalZ / numSamples) + 1; //Add 1 here to calibrate the z-acceleration to 1G (gravity)

  gyroOffsetX = (GyroTotalX / numSamples);
  gyroOffsetY = (GyroTotalY / numSamples);
  gyroOffsetZ = (GyroTotalZ / numSamples); 

  Serial.println();
  Serial.println("...Calibration Complete...");

   // Display or log the calibrated biases
  Serial.print("Gyro Bias: X=");
  Serial.print(gyroOffsetX);
  Serial.print(" Y=");
  Serial.print(gyroOffsetY);
  Serial.print(" Z=");
  Serial.println(gyroOffsetZ);
  Serial.print("Accel Bias: X=");
  Serial.print(accOffsetX);
  Serial.print(" Y=");
  Serial.print(accOffsetY);
  Serial.print(" Z=");
  Serial.println(accOffsetZ);

  delay(2000);
}

