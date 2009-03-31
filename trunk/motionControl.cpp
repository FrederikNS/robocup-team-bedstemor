/*
 * motionControl.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#include "sensors.h"
#include "constants.h"
#include "motors.h"

void findLine(float result[3], int senso[8]) {
	int sensor[8];
	fillSensorData(sensor);
	int high = 900;
	int i = 0;
	int something = 0;
	int start1 = -1;
	int end1 = -1;
	int start2 = -1;
	int end2 = -1;
	int line = 0;
	int sum = 0;
	for(i = 0; i < 8; i++) {
		if(sensor[i] > high) {
			if(start1 == -1) {
				start1 = i;
			} else if(start2 == -1) {
				start2 = i;
			}
		} else {
			if(start1 == -1) {
				start1 = i;
			} else if(start2 == -1) {
				start2 = i;
			}
		}
	}/*
	if(start1 != -1 && end1 == -1) {
		end1 = 7;
	}
	if(start2 != -1 && end2 == -1) {
		end2 = 7;
	}
	if(start1 <= 1 && end1 >= 7) {
		result[0] = -3;
	} else if(start1 == 0 && end1 >= 4) {
		result[0] = -1;
	} else if(start1 <= 3 && end1 == 7) {
		result[0] = -2;
	} else if(start2 == -1) {
		result[0] = 1;*/
		for(i = 0; i < 8; i++) {
			line += sensor[i] * (i+1);
			sum += i;
		}
		result[1] = line/sum;/*
	} else if(end2 != -1) {
		result[0] = 2;
		int split = start2 - end1;
		for(i = 0; i < split; i++) {
			line += sensor[i] * (i+1);
			sum += i;
		}
		result[1] = line/sum;
		line = 0;
		sum = 0;
		for(i = split; i < 8; i++) {
			line += sensor[i] * (i+1);
			sum += i;
		}
		result[2] = line/sum;
	} else {
		result[0] = 0;
	}*/
	senso = sensor;
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
