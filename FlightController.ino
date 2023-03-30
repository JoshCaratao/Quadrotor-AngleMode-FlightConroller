#include <Wire.h>
#include "I2Cdev.h"

//========================================================================================================================//
//                                  Joshua Caratao's Quadcopter Flight Controller Code                                    // 
//           All methods have either been written completely by myself or learned and adapted from other resources.       //
//========================================================================================================================//

//Declare Radio Reciever Pins
// const int Chn_1_Pin = ;  //Throttle
// const int Chn_2_Pin = ;  //Elevator
// const int Chn_3_Pin = ;  //Aileron
// const int Chn_4_Pin = ;  //Rudder
// const int Chn_5_Pin = ;  //Gear (Throttle Cut)
// const int Chn_6_Pin = ;  // Aux channel

//Declare ESC Output Pins
// const int Motor_1_Pin  =;
// const int Motor_2_Pin  =;
// const int Motor_3_Pin  =;
// const int Motor_4_Pin  =;


//Declare IMU Input Pin
//const int MPU = 0x68;


//Declare IMU Raw Data Variables
long AccX, AccY, AccZ;
long GyroX, GyroY, GyroZ;

//Declare the actual calculated IMU Values
float gForceX, gForceY, gForceZ;
float rotX, rotY, rotZ;

//These adjust the full scale Range of the gyro and accel in the IMU. 
//Change these from a value from 1-4, which 4 being highest range, but least sensitivity
const int accFullScaleRange = 1;
const int gyroFullScaleRange = 1;

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

}





//========================================================================================================================//
//                                                       MAIN LOOP                                                        //                           
//========================================================================================================================//
                                                  
void loop() {
  // put your main code here, to run repeatedly:
  readIMU(); //This calls both the readIMU function and the ProcessIMU function
  printData(); //Print out IMU Data
  delay(100);
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

//Next, we need to change the range of our measurements and the sensitivity ofr Gyroscope and Accelerometer
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
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (3B - 40). here we are requestion 6 register from 3B to 40, which gives the Accel outputs
  while(Wire.available() < 6);
  AccX = Wire.read()<<8|Wire.read(); //Store first two bytes into a 
  AccY = Wire.read()<<8|Wire.read(); //Store middle two bytes into a 
  AccZ = Wire.read()<<8|Wire.read(); //Store last two two bytes into a 

  //Read Gyroscope 
  Wire.beginTransmission(0b1101000); //The I2C address of the MPU
  Wire.write(0x43); //Starting register for Gyro Readings
  Wire.endTransmission();
  Wire.requestFrom(0b1101000, 6); //Request Accel Registers (3B - 40). here we are requestion 6 register from 3B to 40, which gives the Accel outputs
  while(Wire.available() < 6);
  GyroX = Wire.read()<<8|Wire.read(); //Store first two bytes into a 
  GyroY = Wire.read()<<8|Wire.read(); //Store middle two bytes into a 
  GyroZ = Wire.read()<<8|Wire.read(); //Store last two two bytes into a 

  processIMU();
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
  gForceZ = (AccZ / aLSB) - accOffsetZ + 1; //Add 1 here to calibrate the z-acceleration to 1G (gravity)

  //Compute Gyro
  rotX = (GyroX / gLSB) - gyroOffsetX;
  rotY = (GyroY / gLSB) - gyroOffsetY;
  rotZ = (GyroZ / gLSB) - gyroOffsetZ;

}

//This function calculated the acceleration and gyro offsets .
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

  int samples = 0;
  while(((millis() - time1)/ (1000)) < 5){
    readIMU();
    
    gForceTotalX += gForceX;
    gForceTotalY += gForceY;
    gForceTotalZ += gForceZ;

    rotTotalX += rotX;
    rotTotalY += rotY;
    rotTotalZ += rotZ;  

    //increase the sample size by 1 each each loop
    samples += 1;
  }

  //Compute the averages of each IMU value over a 5 second time period (or whatever time period works best) and set the offsets.
  accOffsetX = (gForceTotalX / samples);
  accOffsetY = (gForceTotalY / samples);
  accOffsetZ = (gForceTotalZ / samples);

  gyroOffsetX = (rotTotalX / samples);
  gyroOffsetY = (rotTotalY / samples);
  gyroOffsetZ = (rotTotalZ / samples);
  
}



//Prints out the IMU Data 
void printData(){
  Serial.print("Gyro(deg)");
  Serial.print(" X=");
  Serial.print(rotX);
  Serial.print(" Y=");
  Serial.print(rotY);
  Serial.print(" Z=");
  Serial.print(rotZ);

  Serial.print(" | ");

  Serial.print(" Accel(g)");
  Serial.print(" X=");
  Serial.print(gForceX);
  Serial.print(" Y=");
  Serial.print(gForceY);
  Serial.print(" Z=");
  Serial.print(gForceZ);

  Serial.println(" ");

}
































                                                  
