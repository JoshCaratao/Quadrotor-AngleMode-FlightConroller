Flight Controller Code (All code has either been written by Joshua Caratao or learned and adapted from other helpful resources)

*NOTE: This code runs on a physical Flight Controller consisting of an Arduino Nano and a separate Intertial Measurement Unit (IMU)(MPU 6050). 
*NOTE: Flight Controller V1 is no longer worked on and I have moved onto V2 which includes major upgrades in its structure.

This should still work for other arduinos/microcontrollers, however, pins may differ. Additionally, if you are using a microcontroller with its own internal sensors (gyroscope and accelerometer), functions to get those sensor readings will differ.
  

My overall goals are to successfully, read IMU Data from the MPU-6050 to calculate craft orientation, take in commands from a radio controller, and implement a PID Controller for quadcopter dynamics and self-leveling. 
