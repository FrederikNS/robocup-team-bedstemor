/*
 * main2.cpp
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#include "WProgram.h"
#include "motionControl.h"
#include "wiring.h"
#include "constants.h"
#include "motors.h"

//http://www.ikalogic.com/tut_line_sens_algo.php

void setup() {
	Serial.begin(9600);
	pinMode(leftMotorSpeed, OUTPUT);
	pinMode(leftMotorDirection, OUTPUT);
	pinMode(rightMotorSpeed, OUTPUT);
	pinMode(rightMotorDirection, OUTPUT);
	pinMode(enableMotor, OUTPUT);
	pinMode(A, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(C, OUTPUT);
}

void loop() {
	float temp[3] = {0,0,0};
	int sensors[8];
	int i;
	findLine(temp, sensors);
	for(i = 0; i < 8; i++) {
		Serial.println(sensors[i] + " ");
	}
	Serial.println("\n");
	int left = 0;
	int right = 0;
	float factor = 1024/4.5;
	int max = 1023;
	left = (temp[1]*factor)+0.5;
	right = ((1024-temp[1])*factor)+0.5;
	if(left > max) {
		left = max;
	}
	if(right > max) {
		right = max;
	}

	setLeftMotor(left,FORWARD);
	setRightMotor(right,FORWARD);


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
}

int main() {
	init();
	setup();
	for(;;) {
		loop();
	}
	return 0;
}
