/*
 * motionControl.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#include "sensors.h"
#include "constants.h"
#include "motors.h"

void followLine() {
	int sensor[8];
	fillSensorData(sensor);
	int line = 0;
	int sum = 0;
	int i = 0;
	int maxSpeed = 255;
	int left = 0;
	int right = 0;
	int factor = maxSpeed / 4.5;

	for(i = 0; i < 8; i++) {
		line += sensor[i] * (i+1);
		sum += sensor[i];
	}
	line = line / sum;
	left = (line*factor)+0.5;
	right = ((1024-line)*factor)+0.5;
	if(left > maxSpeed)
		left = maxSpeed;
	if(right > maxSpeed)
		right = maxSpeed;

	setLeftMotor(left,FORWARD);
	setRightMotor(right,FORWARD);
}
