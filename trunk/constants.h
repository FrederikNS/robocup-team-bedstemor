#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "wiring.h"

#define REVERSE 1
#define FORWARD 0

#define cross 128

#define startButton 7

#define speakerPin -1 //placeholder

#define enableMotor 9
#define leftMotorSpeedPin 5
#define leftMotorDirectionPin 4
#define rightMotorSpeedPin 3
#define rightMotorDirectionPin 2

#define enableLight 13

#define A 8
#define B 10
#define C 12
#define getSensorData 0

#define north 0
#define south 1
#define east 2
#define west 3
#define northwest 4

const char Aarray[] = {LOW,HIGH,LOW,HIGH,LOW,LOW,HIGH,HIGH};
const char Barray[] = {LOW,HIGH,HIGH,LOW,HIGH,LOW,LOW,HIGH};
const char Carray[] = {LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH};

#define bitShift 2
#define toFixedPoint(x) (x << bitShift)
#define fromFixedPoint(x) (x >> bitShift)
#define multiplyFixed(x,y) ((x*y) >> bitShift)
#define divideFixed(x,y) ((x << bitShift) / y)

#endif
