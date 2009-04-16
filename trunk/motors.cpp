/*
 * motors.cpp
 *
 *  Created on: Mar 17, 2009
 *      Author: starswifter
 */

#include "constants.h"

/*
 * Function for setting the speed and direction of the left motor.
 */
void setLeftMotor(unsigned char speed, char direction) {
	digitalWrite(leftMotorDirectionPin,!direction);
	//The left motor is sitting at opposite direction of the right
	//Therefore, when we want both to drive forward, one of them will
	//drive backward compared to the direction of the robot.
	//That will simply make the robot spin.
	//To counter that, left motor will have to interpret the direction
	//opposite of the right motor
	if(!direction) {
		speed = 255-speed;
	}
	analogWrite(leftMotorSpeedPin, speed);
}

/*
 * Function for setting the speed and direction of the right motor.
 */
void setRightMotor(unsigned char speed, char direction) {
	digitalWrite(rightMotorDirectionPin,direction);
	if(direction) {
		speed = 255-speed;
	}
	analogWrite(rightMotorSpeedPin, speed);
}

/*
 * Function for setting the speed and direction of both motors.
 * It just calls the existing specialised functions to do the job.
 */
void setBothMotors(unsigned char speed, char direction) {
	setRightMotor(speed, direction);
	setLeftMotor(speed, direction);
}
