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
#include "motionControl.h"

#define maxSpeed 255
#define factor multiplyFixed(speedLimit,14)
#define degreeToTickFactor 8.1*1000


long leftTick = 0;
long rightTick = 0;

void findLine(char* situation, int lineLocations[2], int sensor[8]) {
	fillSensorData(sensor);
	int i = 0;
	unsigned char lowest1 = 10;
	int value1 = 1023;
	unsigned char lowest2 = 10;
	int value2 = 400;
	int low = 400;
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
	}
	/*Serial.print("Lines: ");
	Serial.print(lineLocations[0]);
	Serial.print("\t");
	Serial.println(lineLocations[1]);*/
}

void calculateMotorSpeedFromLine(int line, int speedLimit) {
		setLeftMotor(constrain(speedLimit+(fromFixedPoint(line)-4)*60,0,255),FORWARD);
		setRightMotor(constrain(speedLimit-(fromFixedPoint(line)-4)*60,0,255),FORWARD);
}

void leftInterrupt() {
	leftTick--;
}

void rightInterrupt() {
	rightTick--;
}

void turnDegrees(int degree) {
	Serial.println("turning");
		degree %= 360;
	
	long ticksRemaining = 0;
	
	if(degree > 180 || (degree > -180 && degree <= 0)) {
		//turn right
		if(degree < 0) {
			degree *= -1;
		}
		leftTick = rightTick = ((degree*100000)/20000);
		setLeftMotor(255,FORWARD);
		setRightMotor(255,REVERSE);
	} else {
		//turn left
		if(degree < 0) {
			degree = 360-degree;
		}
		rightTick = leftTick = ((degree*100000)/20000);
		setLeftMotor(255,REVERSE);
		setRightMotor(255,FORWARD);
	}
	ticksRemaining = leftTick + rightTick;
	while(ticksRemaining > 0) {
		ticksRemaining = leftTick + rightTick;
		Serial.println(ticksRemaining);
	}
	stop(FORWARD);
	Serial.println("Stop");
	ticksRemaining = 0;
	leftTick = 0;
	rightTick = 0;
}

void stop(char direction) {
	/*if(direction) {
		setBothMotors(255,REVERSE);
	} else {
		setBothMotors(255,FORWARD);
	}*/
	setBothMotors(0,FORWARD);
}

void goDistance(int distance, char direction) {
	Serial.println("goDistance");
	
	leftTick = rightTick = 30*distance;
	Serial.println(leftTick);
	long ticksRemaining = leftTick + rightTick;
	setBothMotors(255, direction);
	while(ticksRemaining > 0) {
		ticksRemaining = leftTick + rightTick;
		Serial.println(leftTick+rightTick);
	}
	stop(direction);
	ticksRemaining = 0;
	leftTick = 0;
	rightTick = 0;
}

void goStraight() {
	Serial.println("Going Straight");
	goDistance(centerRobot, FORWARD);
}

void goLeft() {
	Serial.println("Going Left");
	goDistance(centerRobot, FORWARD);
	turnDegrees(90);
}

void goRight() {
	Serial.println("Going Right");
	goDistance(centerRobot, FORWARD);
	turnDegrees(-90);
}