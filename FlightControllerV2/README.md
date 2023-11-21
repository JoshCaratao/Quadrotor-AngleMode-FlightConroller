# **FLIGHT CONTROLLER V2.0**


The change from V1 to V2 includes major restructuring towards using custom libraries and more object oriented programming to simplify and better the readability of the main flight controller sketch file. 


## HOW IT WORKS:
  So far, the flight controller runs a main program called "FlightController_VX.X" which is the main logic and uses custom libraries to retrieve IMU Data and make necessary computations such as attitude estimation. 

  Using the I2C communication protocol to allow for proper communication between the Arduino and the IMU I wrote my own custom library for easily retrieving data from the MPU6050 (Further description in the Source file). 
  
  Using the angular velocities and linear accelerations retrieved from the IMU, separate roll, pitch, and yaw angles are calculated using numerical integration(gyroscope) and trigonemetric equations (accelerometer).

  However, because neither sensor is reliable enough alone for accurate attitude estimation alone, I implmented a sensor fusion algorithm through a simple complementary filter to combine both sensor measurements into one, more reliable measurement.



## PROGRESS NOTES:

11/18/23 - Restructured Flight Controller V1 and im now able to reliably read, process, and output angular velocity values and acceleration values from the external IMU using my custom library
            
TO DO NEXT:  Figure out Serial Plotting for testing, and implment attitude estimation through gyro and accelerometer separately first.

11/19/23 - Made changes to my calibration function. Rather than a set time for calibration, it takes a specified amount of samples for averaging and calculating offset values. Also working on a matlab script to allow for live visualization of sensor data. Also added in a new struct class for storing state attitude data and a new class (system) for performing attitude estimation.
