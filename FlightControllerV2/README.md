# **FLIGHT CONTROLLER V2.0**


The change from V1 to V2 includes major restructuring towards developing and using my own custom libraries and more object oriented programming to simplify and better the readability of the main flight controller sketch file. 


## HOW IT WORKS:
  So far, the flight controller runs a main program called "FlightController_VX.X" which is the main logic and uses custom libraries to retrieve IMU Data and make necessary computations such as attitude estimation. 

  Using the I2C communication protocol to allow for proper communication between the Arduino and the IMU I wrote my own custom library for easily retrieving data from the MPU6050 (Further description in the Source file). 
  
  Using the angular velocities and linear accelerations retrieved from the IMU, separate roll, pitch, and yaw angles are calculated using numerical integration(gyroscope) and trigonemetric equations (accelerometer).

  However, because neither sensor is reliable enough alone for accurate attitude estimation alone, I implmented a sensor fusion algorithm through a simple complementary filter to combine both sensor measurements into one, more reliable measurement.



## PROGRESS NOTES:

11/18/23 - Restructured Flight Controller V1 and im now able to reliably read, process, and output angular velocity values and acceleration values from the external IMU using my custom library
            
TO DO NEXT:  Figure out Serial Plotting for testing, and implment attitude estimation through gyro and accelerometer separately first.

11/19/23 - Made changes to my calibration function. Rather than a set time for calibration, it takes a specified amount of samples for averaging and calculating offset values. Also working on a matlab script to allow for live visualization of sensor data. Also added in a new struct class for storing state attitude data and a new class (system) for performing attitude estimation.


11/29/23  - Made slight changes to IMU file. Class is now called MPU6050 to specifiy the IMU that the code works with. Also switching to a data collector matlab script that plots the data after collection for testing rather than simultaneous data collecting and plotting.


12/15/23 - Restructured flight controller once more. Deleted unecessary Structs and decided to just refer to the class variables directly in the the main sketch. I also renamed the classes to be more accurate. This made the flight controller code much more readable and easy to follow in my opinion. I also fixed my axes for my gyro and accelerometer values according to an online resource. When flat, the Z acceleration reads 1 rather than -1 now. I also decided to align the axes with the MPU 6050 Axes to help mitigate confusion between axes and pitch/roll/yaw axes. The roll now refers to the Y axis, etc.
