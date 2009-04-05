/*
 * motionControl.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#include "sensors.h"
#include "constants.h"
#include "motors.h"

#define int maxSpeed 255
#define float factor maxSpeed/3.5

void findLine(float result[3], int sensor[8]) {
	fillSensorData(sensor);
	int i = 0;
	int lowest1 = 0;
	int value1 = 10;
	int lowest2 = 0;
	int value2 = 10;
	int low = 200;
	int start1 = -1;
	int end1 = -1;
	int start2 = -1;
	int end2 = -1;
	int line = 0;
	int sum = 0;
	for(i = 0; i < 8; i++) {
		int temp = sensor[i];
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
		result[0] = -3;
	} else if(start1 == 0 && end1 >= 4) { //turn left
		result[0] = -1;
	} else if(start1 <= 3 && end1 == 7) { //turn right
		result[0] = -2;
	} else if(start2 == -1) { //forward
		result[0] = 1;
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
		result[1] = line/sum;
	} else if(end2 != -1) { //two roads
		result[0] = 2;
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
		result[1] = line/sum;
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
		result[2] = line/sum;
	} else {
		result[0] = 0;
	}
}

void calculateMotorSpeedFromLine(float line) {
	int rightMotorSpeed = 0;
	int leftMotorSpeed = 0;
	rightMotorSpeed = ((8-line)*factor)+0.5;
	leftMotorSpeed = (line*factor)+0.5;
	if(rightMotorSpeed > maxSpeed) {
		rightMotorSpeed = maxSpeed;
	}
	if(leftMotorSpeed > maxSpeed) {
		leftMotorSpeed = maxSpeed;
	}
	setLeftMotor(leftMotorSpeed, FORWARD);
	setRightMotor(rightMotorSpeed, FORWARD);
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
