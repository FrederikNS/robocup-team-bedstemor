/*
 * motors.cpp
 *
 *  Created on: Mar 17, 2009
 *      Author: starswifter
 */

#include "constants.h"

void setLeftMotor(unsigned char speed, char direction) {
	digitalWrite(leftMotorDirectionPin,direction);
	if(!direction) {
		speed = 255-speed;
	}
	analogWrite(leftMotorSpeedPin, speed);
}

void setRightMotor(unsigned char speed, char direction) {
	digitalWrite(rightMotorDirectionPin,direction);
	if(direction) {
		speed = 255-speed;
	}
	analogWrite(rightMotorSpeedPin, speed);
}

void setBothMotors(unsigned char speed, char direction) {
	setRightMotor(speed, direction);
	setLeftMotor(speed, direction);
}

void goDistance(int distance, char direction) {

}
