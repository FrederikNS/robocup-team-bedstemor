#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "wiring.h"

#define REVERSE 1
#define FORWARD 0

#define centerRobot 12

#define leftMotorInterrupt 3
#define rightMotorInterrupt 2

#define startButton 7 //??

#define speakerPin -1 //placeholder

#define enableMotor 9
#define leftMotorSpeedPin 6
#define leftMotorDirectionPin 7
#define rightMotorSpeedPin 5
#define rightMotorDirectionPin 4

#define enableLight 13

#define A 8
#define B 10
#define C 12
#define getSensorData 0
#define getDistanceData 1

#define north 0
#define south 1
#define east 2
#define west 3
#define northwest 4

const int Aarray[] = {LOW,HIGH,LOW,HIGH,LOW,LOW,HIGH,HIGH};
const int Barray[] = {LOW,HIGH,HIGH,LOW,HIGH,LOW,LOW,HIGH};
const int Carray[] = {LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH};

#define bitShift 2
#define toFixedPoint(x) (x << bitShift)
#define fromFixedPoint(x) (x >> bitShift)
#define multiplyFixed(x,y) ((x*y) >> bitShift)
#define divideFixed(x,y) ((x << bitShift) / y)

#define INTERRUPT_R 2
#define INTERRUPT_L 3

#endif
