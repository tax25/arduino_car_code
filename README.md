# Arduino car project

This is the code for an Arduino project that I started.
The project is still in development, so the code will be modified over and over again over the next
few days or months.

## define val (row 66)
The value "val" is the sound speed at a temperature of 20° Celsius in centimeters per microsecond.
We use it to calculate the distance between the sensor and an obstacle.
To calculate the distance between the sensor and the obstacle we have to
multiply the speed of sound (that for now is "val") for the time returned by
the pulseIn func divided by 2, like this: val * returnedTime/2.

I plan to replace "val" with the real speed of sound for the temperature in the
room the robot is in, to do it I think I'll use the DHT22 temperature & humidity
sensor to calculate the speed of sound.
We can calculate the speed of sound by following this formula:
sound_speed = 0.03314 + 0.000062 * temperature (the values are in cm/microsecond).

## Translating joystick values from 0/1023 to -100/100 and to 45/135
The joystick returns 0 to 1023 values from the potentiometers it has, but
We want to use the joystick to control the speed of the motor, and the motor driver
only accepts values from 0 to 255. To be able to use the joystick to control the
speed we have to "translate" them to min = 0 and max = 255, to do this we
use the function "map".

We do the same thing for the steering too.
45° is the maximum left steering, 90° the car is going straight and 135° is the
maximum right steering.

ATTENTION: sometimes to have the maximum value you want, after the "translation"
you have to add a number in the map function. 
Eg: map(_speed, 0, 1023, 100, -100); could give you 99 and -99 as max values.
to avoid this you can just write 101. map(_speed, 0, 1023, 101, -101);
