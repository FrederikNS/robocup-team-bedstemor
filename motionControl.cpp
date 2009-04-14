/*
 * motionControl.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#include "WProgram.h"
#include "sensors.h"
#include "constants.h"
#include "motors.h"

#define maxSpeed 255
#define factor multiplyFixed(speedLimit,14)
#define degreeToTickFactor 4.4*1000

volatile int leftTick = 0;
volatile int rightTick = 0;

void findLine(char* situation, int lineLocations[2], int sensor[8]) {
	fillSensorData(sensor);
	int i = 0;
	char lowest1 = 10;
	int value1 = 1023;
	char lowest2 = 10;
	int value2 = 500;
	int low = 500;
	char start1 = -1;
	char end1 = -1;
	char start2 = -1;
	char end2 = -1;
	long line = 0;
	int sum = 0;
	int temp = 0;
	for(i = 0; i < 8; i++) {
		temp = sensor[i];
		if(temp < value1) {
			lowest1 = i;
			value1 = temp;
		} else if(temp < value2) {
			if(i != lowest1+1) {
				lowest2 = i;
				value2 = temp;
			}
		}
		if(temp < low) {
			if(start1 == -1) {
				start1 = i;
			} else if(start2 == -1 && end1 != -1) {
				start2 = i;
			}
		} else {
			if(end1 == -1 && start1 != -1) {
				end1 = i-1;
			} else if(end2 == -1 && start2 != -1) {
				end2 = i-1;
			}
		}
	}
	if(start1 != -1 && end1 == -1) {
		end1 = 7;
	}
	if(start2 != -1 && end2 == -1) {
		end2 = 7;
	}/*
	Serial.print("start1: ");
	Serial.print((int)start1);
	Serial.print("\tend1: ");
	Serial.print((int)end1);
	Serial.print("\tstart2: ");
	Serial.print((int)start2);
	Serial.print("\tend2: ");
	Serial.println((int)end2);

	Serial.print("\tlowest1: ");
	Serial.print((int)lowest1);
	Serial.print("\tlowest2: ");
	Serial.println((int)lowest2);*/
	if(start1 <= 0 && end1 >= 7) { //crossroad
		*situation = -3;
	} else if(start1 == 0 && end1 >= 4) { //turn left
		*situation = -1;
	} else if(start1 <= 3 && end1 == 7) { //turn right
		*situation = -2;
	} else if(start2 == -1) { //forward
		*situation = 1;
		i = lowest1 - 1;
		temp = lowest1 + 1;/*
		Serial.print("Calculating from sensor: ");
		Serial.print(i);
		Serial.print(" to ");
		Serial.println(temp);*/
		if(i == -1) {
			i++;
		}
		if(temp == 8) {
			temp--;
		}
		for(i; i < temp; i++) {
			line += sensor[i] * (i+1);
			sum += sensor[i];
		}
		if(!sum) {
			sum = 1;
		}
		lineLocations[0] = divideFixed(toFixedPoint(line),toFixedPoint(sum));
	} else if(end2 != -1) { //two roads
		*situation = 2;
		for(i = start1; i <= end1; i++) {
			temp = sensor[i];
			if(temp < value1) {
				lowest1 = i;
				value1 = temp;
			}
		}
		for(i = start2; i <= end2; i++) {
			temp = sensor[i];
			if(temp < value2) {
				lowest2 = i;
				value2 = temp;
			}
		}
		i = lowest1 - 1;
		temp = lowest1 + 1;
		if(i == -1) {
			i++;
		}
		if(temp == 8) {
			temp--;
		}
		for(i; i < temp; i++) {
			line += sensor[i] * (i+1);
			sum += sensor[i];
		}
		if(!sum) {
			sum = 1;
		}
		lineLocations[0] = divideFixed(toFixedPoint(line),toFixedPoint(sum));
		line = 0;
		sum = 0;
		i = lowest2 - 1;
		temp = lowest2 + 1;
		if(i == -1) {
			i++;
		}
		if(temp == 8) {
			temp--;
		}
		for(i; i < temp; i++) {
			line += sensor[i] * (i+1);
			sum += sensor[i];
		}
		if(!sum) {
			sum = 1;
		}
		lineLocations[1] = divideFixed(toFixedPoint(line),toFixedPoint(sum));
	} else {
		situation = 0;
	}/*
	Serial.print("Lines: ");
	Serial.print(lineLocations[0]);
	Serial.print("\t");
	Serial.println(lineLocations[1]);*/
}

void calculateMotorSpeedFromLine(int line, int speedLimit) {
	int rightMotorSpeed = 0;
	int leftMotorSpeed = 0;
	if(line < 14) {
		setLeftMotor(fromFixedPoint(divideFixed(line,14))*speedLimit,FORWARD);
		setRightMotor(speedLimit,FORWARD);
	} else if(line > 14) {
		setRightMotor(fromFixedPoint(divideFixed((toFixedPoint(7)-line),14))*speedLimit,FORWARD);
		setLeftMotor(speedLimit,FORWARD);
	} else if(line == 14) {
		setBothMotors(speedLimit,FORWARD);
	}

	/*rightMotorSpeed = fromFixedPoint((multiplyFixed((toFixedPoint(8)-line),factor))+2);
	leftMotorSpeed = fromFixedPoint(multiplyFixed(line,factor)+2);
	if(rightMotorSpeed > speedLimit) {
		rightMotorSpeed = speedLimit;
	}
	if(leftMotorSpeed > speedLimit) {
		leftMotorSpeed = speedLimit;
	}
	setLeftMotor(leftMotorSpeed, FORWARD);
	setRightMotor(rightMotorSpeed, FORWARD);*/
}

void leftInterrupt() {
	leftTick--;
}

void rightInterrupt() {
	rightTick--;
}

void turnDegrees(int degree) {
	while(degree > 360) {
		degree -= 360;
	}
	while(degree < -360) {
		degree += 360;
	}
	int ticksLeft = 0;
	if(degree > 180 || (degree > -180 && degree < 0)) {
		//turn right
		if(degree < 0) {
			degree = -degree;
		}
		degreeToTickFactor;
		leftTick = rightTick = ((degree*degreeToTickFactor)/1000)/2;
		setLeftMotor(255,FORWARD);
		setRightMotor(255,REVERSE);
	} else {
		//turn left
		if(degree < 0) {
			degree = -degree;
		}
		degree -= 180;
		rightTick = leftTick = ((degree*degreeToTickFactor)/1000)/2;
		setLeftMotor(255,REVERSE);
		setRightMotor(255,FORWARD);
	}
	interrupts();
	while(!(ticksLeft < 0)) {
		ticksLeft = leftTick + rightTick;
	}
	noInterrupts();
	setLeftMotor(0,FORWARD);
	setRightMotor(0,FORWARD);
}

void stop(char direction) {
	if(direction) {
		setBothMotors(255,REVERSE);
	} else {
		setBothMotors(255,FORWARD);
	}
	setBothMotors(0,FORWARD);
}

/*
 *
	left = (line*factor)+0.5;
	right = ((1024-line)*factor)+0.5;
	if(left > maxSpeed)
		left = maxSpeed;
	if(right > maxSpeed)
		right = maxSpeed;

	setLeftMotor(left,FORWARD);
	setRightMotor(right,FORWARD);
 */
