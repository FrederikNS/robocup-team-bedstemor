/*
 * motionControl.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#include "sensors.h"
#include "constants.h"
#include "motors.h"

#define maxSpeed 255
#define factor multiplyFixed(speedLimit,14)
#define degreePerSecond toFixedPoint(0) //placeholder

void findLine(char* situation, short lineLocations[2], short sensor[8]) {
	fillSensorData(sensor);
	char i = 0;
	char lowest1 = 0;
	char value1 = 10;
	char lowest2 = 0;
	char value2 = 10;
	short low = 200;
	char start1 = -1;
	char end1 = -1;
	char start2 = -1;
	char end2 = -1;
	char line = 0;
	short sum = 0;
	short temp = 0;
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
			} else if(start2 == -1) {
				start2 = i;
			}
		} else {
			if(end1 == -1) {
				end1 = i;
			} else if(end2 == -1) {
				end2 = i;
			}
		}
	}
	if(start1 != -1 && end1 == -1) {
		end1 = 7;
	}
	if(start2 != -1 && end2 == -1) {
		end2 = 7;
	}
	if(start1 <= 0 && end1 >= 7) { //crossroad
		*situation = -3;
	} else if(start1 == 0 && end1 >= 4) { //turn left
		*situation = -1;
	} else if(start1 <= 3 && end1 == 7) { //turn right
		*situation = -2;
	} else if(start2 == -1) { //forward
		*situation = 1;
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
		lineLocations[1] = divideFixed(toFixedPoint(line),toFixedPoint(sum));
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
		lineLocations[2] = divideFixed(toFixedPoint(line),toFixedPoint(sum));
	} else {
		situation = 0;
	}
}

void calculateMotorSpeedFromLine(short line, short speedLimit) {
	int rightMotorSpeed = 0;
	int leftMotorSpeed = 0;
	rightMotorSpeed = fromFixedPoint((multiplyFixed((toFixedPoint(8)-line),factor))+2);
	leftMotorSpeed = fromFixedPoint(multiplyFixed(line,factor)+2);
	if(rightMotorSpeed > speedLimit) {
		rightMotorSpeed = speedLimit;
	}
	if(leftMotorSpeed > speedLimit) {
		leftMotorSpeed = speedLimit;
	}
	setLeftMotor(leftMotorSpeed, FORWARD);
	setRightMotor(rightMotorSpeed, FORWARD);
}

void turnDegrees(short degree) {
	while(degree > 360) {
		degree -= 360;
	}
	while(degree < -360) {
		degree += 360;
	}
	int time = 0;
	if(degree > 180 || (degree > -180 && degree < 0)) {
		//turn right
		if(degree < 0) {
			degree = -degree;
		}
		time = multiplyFixed(toFixedPoint(degree),degreePerSecond);
		setLeftMotor(255,FORWARD);
		setRightMotor(255,REVERSE);
	} else {
		//turn left
		if(degree < 0) {
			degree = -degree;
		}
		degree -= 180;
		time = multiplyFixed(toFixedPoint(degree),degreePerSecond);
		setLeftMotor(255,REVERSE);
		setRightMotor(255,FORWARD);
	}
	while(time != 0) {
		time--;
	}
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
