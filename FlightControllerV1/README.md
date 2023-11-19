FLIGHT CONTROLLER V1

This was my first main iteration of the flight controller which achieved IMU Data retrieval and separate attitude estimation values from a gyroscope and accelerometer.

While the overall logic of this setup is similar to my newer Flight Controller V2, almost all functions are written in one big main sketch file. Although this works, it looks messy, can be hard to read, and is not modular if I ever want to apply certain parts to other projects.
