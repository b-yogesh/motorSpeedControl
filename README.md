# motorSpeedControl
Algorithm to control the speed of DC Motor

Based on the RPM of the motor(1200 RPM used), the range of +/-4 in the line no. 85 is used.

It is obtained by dividing the MaxRPM by MaxPWM value of Arduino (255)

If you are using motor with different RPM, try calculating the range and tweak the code as required.

Also, try to change the value of count and ecount in the while loops to 2 or 3 and check for better response time if the value of 1 doesn't work  out.
