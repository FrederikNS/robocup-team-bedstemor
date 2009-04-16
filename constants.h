#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "wiring.h"

//Constants for setting the direction of the robot (for the motors)
#define REVERSE 1
#define FORWARD 0

//Constant for the distance from the sensors to the axle of the wheels
#define centerRobot 6

//Constants for the digital ports of the interrupts
#define INTERRUPT_R 2
#define INTERRUPT_L 3

//Constant for the start button
#define startButton 7 //Not implemented?

//Constant for the speaker
#define speakerPin -1 //placeholder

//Constants for which pins the motors is connected to
#define enableMotor 9
#define leftMotorSpeedPin 6
#define leftMotorDirectionPin 7
#define rightMotorSpeedPin 5
#define rightMotorDirectionPin 4

//Constant for the light diodes used at the sensors
#define enableLight 13

//Constants used when retrieving data from the sensors
#define A 8
#define B 10
#define C 12
#define getSensorData 0
#define getDistanceData 1
const int Aarray[] = {LOW,HIGH,LOW,HIGH,LOW,LOW,HIGH,HIGH};
const int Barray[] = {LOW,HIGH,HIGH,LOW,HIGH,LOW,LOW,HIGH};
const int Carray[] = {LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH};

//Constants used for directions at cross roads
#define north 0
#define south 1
#define east 2
#define west 3
#define northwest 4

//Macros needed for transforming integers to and from fixed point,
//and multiplying and dividing with fixed point
#define bitShift 2
#define toFixedPoint(x) (x << bitShift)
#define fromFixedPoint(x) (x >> bitShift)
#define multiplyFixed(x,y) ((x*y) >> bitShift)
#define divideFixed(x,y) ((x << bitShift) / y)

#endif
