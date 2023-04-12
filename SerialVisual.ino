
//========================================================================================================================//
//    THIS IS FOR FUNCTIONS LIKE PRINTING SERIAL DATA AND SERIAL PLOTTING, NOT CRITICAL COMPONENTS OF FLYING THE CRAFT.   //                           
//========================================================================================================================//


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


void plotData(){
  Serial.print("gyro_X:");
  Serial.print(rotX);
  Serial.print(", ");

  Serial.print("gyro_Y:");
  Serial.print(rotY);
  Serial.print(", ");

  Serial.print("gyro_Z:");
  Serial.print(rotZ);
  Serial.print(", | ");

  Serial.print("Accel_X:");
  Serial.print(gForceX);
  Serial.print(", ");

  Serial.print("Accel_Y:");
  Serial.print(gForceY);
  Serial.print(", ");

  Serial.print("Accel_Z:");
  Serial.print(gForceZ);
  Serial.print(", ");

  Serial.println();
}


void plotAngles() {
  Serial.print("gyro_X:");
  Serial.print(gyroAngleX);
  Serial.print(", ");

  Serial.print("gyro_Y:");
  Serial.print(gyroAngleY);
  Serial.print(", ");

  Serial.print("gyro_Z:");
  Serial.print(gyroAngleZ);
  Serial.print(", | ");

  Serial.print("accel_X:");
  Serial.print(accelAngleX);
  Serial.print(", ");

  Serial.print("accel_Y:");
  Serial.print(accelAngleY);
  Serial.print(", ");

  Serial.println();
}
