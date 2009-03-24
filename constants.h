#ifndef CONSTANTS_H_
#define CONSTANTS_H_

#include "wiring.h"

#define REVERSE 1
#define FORWARD 0

#define speakerPin -1 //placeholder

#define enableMotor 9
#define leftMotorSpeed 5
#define leftMotorDirection 4
#define rightMotorSpeed 3
#define rightMotorDirection 2

#define A 8
#define B 10
#define C 12
#define getSensorData 0

const int Aarray[] = {LOW,HIGH,LOW,HIGH,LOW,LOW,HIGH,HIGH};
const int Barray[] = {LOW,HIGH,HIGH,LOW,HIGH,LOW,LOW,HIGH};
const int Carray[] = {LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH};

#endif
