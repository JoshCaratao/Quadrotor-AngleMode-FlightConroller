%This script is used for testing purposes. This takes in live serial data
%from my arduino and then plots it in real time

%Check for free ports
%freeports = serialportlist('available');

% Print the list of available ports
%fprintf('Available Ports:\n');
%fprintf('%s\n', freeports);

%Create Serial Object
s = serialport('COM5', 9600); 
configureTerminator(s, "LF"); %allows Matlab to know when a full messages has been recieved (end of a data packet)

%Create figure for Accelerometer
accFigure = figure;
accPlots = plot(zeros(1,3)); %Initial plot, assuming six data points

%Set plot properties for accelerometer
accTitle = "Live Accelerometer Data Plot";
title(accTitle);
xlabel('Time');
ylabel('Linear Acceleration');
legend(accPlots, 'X Acceleration', 'Y Acceleration', 'Z Acceleration');

%Create figure for Gyroscope
gyroFigure = figure;
gyroPlots = plot(zeros(1,3)); %Initial plot, assuming six data points

%Set plot properties for Gyroscope
gyroTitle = "Live Gyroscope Data Plot";
title(gyroTitle);
xlabel('Time');
ylabel('Angular Velocity');
legend(gyroPlots, 'X Gyro', 'Y Gyro', 'Z Gyro');


%Initialize Variables
bufferSize = 100; %Adjust based on data rate. This is the amount of data points stored before updating plots
time = zeros(bufferSize, 1);
accData = zeros(bufferSize, 3);
gyroData = zeros(bufferSize, 3);

%Main loop for continuously updating the plots
while(ishandle(accFigure) && ishandle(gyroFigure)) %ishandle() checks if the graphics handle for the plots are still valid

    try
        %Read data from serial port
        newData = readline(s); %Takes in serial data (6 IMU values in this case)
        newData = str2double(split(newData, ',')); %Turns them into doubles and parses them by commas

        disp(newData);
    
        %Update time and data buffers
        time = [time(2:end); time(end) + 1];
        accData = [accData(2:end, :); newData(1:3)];
        gyroData = [gyroData(2:end, :); newData(4:6)];
        
        %Update the live plots
        for i = 1:3
            set(accPlots(i), 'XData', time, 'YData', accData(:, i));
            set(gyroPlots(i), 'XData', time, 'YData', gyroData(:, i));
        end
    
        drawnow; %Updates the display. Forces the immediate drawing of graphics figures and updates to the display. 
    
        %Pause to control data rate
        pause(0.1);

    catch
        %Handle errors
        disp('Error reading or processing data.');
        break;
    end
   
end

%Close the serial connection when done;
close(s);
delete(s);
clear s;

   