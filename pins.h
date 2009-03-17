#ifndef PINS_H_
#define PINS_H_

#include "wiring.h"

const int enableMotor = 9;
const int leftMotorSpeed = 5;
const int leftMotorDirection = 4;
const int rightMotorSpeed = 3;
const int rightMotorDirection = 2;

const int A = 8;
const int B = 10;
const int C = 12;
const int getSensorData = 0;

const int Aarray[] = {LOW,HIGH,LOW,HIGH,LOW,LOW,HIGH,HIGH};
const int Barray[] = {LOW,HIGH,HIGH,LOW,HIGH,LOW,LOW,HIGH};
const int Carray[] = {LOW,LOW,LOW,LOW,HIGH,HIGH,HIGH,HIGH};

#endif
