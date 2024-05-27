# **Flight Controller Code/Package** 

(All code has either been written by Joshua Caratao or learned and adapted from other helpful resources)

*NOTE*: This code runs on a physical Flight Controller consisting of an Arduino Nano and a popular Inertial Measurement Unit called MPU-6050.  

*NOTE*: Flight Controller V1 is no longer worked on and I have moved onto V2 which includes major upgrades in its structure.

This should still work for other arduinos/microcontrollers, however, pins may differ. Additionally, if you are using a microcontroller with its own internal sensors (gyroscope and accelerometer) or a different IMU, those will require different drivers or libraries than the ones I have written in my own code. 
  

My overall goals are to successfully, read IMU Data from the MPU-6050 to calculate craft orientation and implement an Angle Mode PID Controller to drive Brushless DC Motors for self-leveling of the quadrotor craft. 



# **FLIGHT CONTROLLER V2.0**


The change from V1 to V2 includes major restructuring towards developing and using my own custom libraries and more object oriented programming to simplify and better the readability of the main flight controller sketch file. 


## HOW IT WORKS:

### State Estimation

  Using an MPU-6050 Library/Driver I wrote with the help of online resouces and the MPU-6050 Data Sheet, my flight controller is able to retrieve gyroscope and accelerometer readings from the Inertial Measurement Unit (IMU). For each axis (roll, pitch, and yaw), the flight controller then uses my "StateEstimator" Library to estimate the drone's roll, pitch, and yaw angles using a complementary filter that fuses the gyroscope and accelerometer data for a more accurate state estimation. 
  

## PROGRESS NOTES:

11/18/23 - Restructured Flight Controller V1 and im now able to reliably read, process, and output angular velocity values and acceleration values from the external IMU using my custom library
            
TO DO NEXT:  Figure out Serial Plotting for testing, and implment attitude estimation through gyro and accelerometer separately first.

11/19/23 - Made changes to my calibration function. Rather than a set time for calibration, it takes a specified amount of samples for averaging and calculating offset values. Also working on a matlab script to allow for live visualization of sensor data. Also added in a new struct class for storing state attitude data and a new class (system) for performing attitude estimation.


11/29/23  - Made slight changes to IMU file. Class is now called MPU6050 to specifiy the IMU that the code works with. Also switching to a data collector matlab script that plots the data after collection for testing rather than simultaneous data collecting and plotting.


12/15/23 - Restructured flight controller once more. Deleted unecessary Structs and decided to just refer to the class variables directly in the the main sketch. I also renamed the classes to be more accurate. This made the flight controller code much more readable and easy to follow in my opinion. I also fixed my axes for my gyro and accelerometer values according to an online resource. When flat, the Z acceleration reads 1 rather than -1 now. I also decided to align the axes with the MPU 6050 Axes to help mitigate confusion between axes and pitch/roll/yaw axes. The roll now refers to the Y axis, etc.


12/17/23 - Updated my physical Flight controller with Indicator LEDS. Programmed those into my main flight controller program. Now, during the setup a RED LED lights up, and during calibration, the LED blinks, alternating every half second. Once calibration and set up are complete, the red LED turns off and the Green LED turns on, letting the user know that the flight controller has entered the main loop and is ready.

5/27/14 - Added Complementary Filter to fuse Gyro and Accelerometer Sensor data for more accurate Attitude Estimation. Also added in note to test/implement a low pass filter on the accelerometer measurements to filter out high frequency noise.
