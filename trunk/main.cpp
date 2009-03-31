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

//http://www.ikalogic.com/tut_line_sens_algo.php

void setup() {
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
	//findLine();
}

int main() {
	init();
	setup();
	for(;;) {
		loop();
	}
	return 0;
}
