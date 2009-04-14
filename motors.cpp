/*
 * motors.cpp
 *
 *  Created on: Mar 17, 2009
 *      Author: starswifter
 */

#include "constants.h"

void setLeftMotor(char speed, char direction) {
	digitalWrite(leftMotorDirection,direction);
	if(direction) {
		speed = 256-speed;
	}
	analogWrite(leftMotorSpeed, speed);
}

void setRightMotor(char speed, char direction) {
	digitalWrite(rightMotorDirection,direction);
	if(direction) {
		speed = 256-speed;
	}
	analogWrite(rightMotorSpeed, speed);
}

void setBothMotors(char speed, char direction) {
	setRightMotor(speed, direction);
	setLeftMotor(speed, direction);
}
