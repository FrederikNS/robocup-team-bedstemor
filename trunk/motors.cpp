/*
 * motors.cpp
 *
 *  Created on: Mar 17, 2009
 *      Author: starswifter
 */

#include "constants.h"

void setLeftMotor(int speed, int direction) {
	digitalWrite(leftMotorDirection,direction);
	if(direction) {
		speed = 256-speed;
	}
	analogWrite(leftMotorSpeed, speed);
}

void setRightMotor(int speed, int direction) {
	digitalWrite(rightMotorDirection,direction);
	if(direction) {
		speed = 256-speed;
	}
	analogWrite(rightMotorSpeed, speed);
}

void setBothMotors(int speed, int direction) {
	setRightMotor(speed, direction);
	setLeftMotor(speed, direction);
}
