%This script is used for testing purposes. This takes in live serial data
%from my arduino and then plots it after a certain amount of time

%Create Serial Object
s = serialport('COM5', 9600); 
configureTerminator(s, "LF"); %allows Matlab to know when a full messages has been recieved (end of a data packet)


%Number of Data Points to plot
numPoints = 1000;
data = zeros(numPoints,6);

%Loop to collect Data

%Create figure for Plotting
figure;







%Close the serial connection when done;
close(s);
delete(s);
clear s;

   