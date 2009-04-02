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
	pinMode(enableLight, OUTPUT);
	
	
}

void loop() {
	digitalWrite(enableMotor, HIGH);
	digitalWrite(enableLight, HIGH);
	
	char currentPath = 1;
	char lastCrossing = 0;
	
	char sensors[8];
	
	char situation;
	
	float lineLocations[2];
	
	while(1) {
		
		switch(currentPath){
			case 1:
			case 3:
			case 5:
			case 6:
			case 15:
			case 17:
				//Simple Paths, just follow line
				
				break;
			case 2:
				//Seesaw
				break;
			case 4:
				//Stairs
				break;
			case 7:
			case 8:
			case 10:
				//Stop immediately, forbidden path
				break;
			case 11:
				//Goal path
				break;
			case 12:
				//Discontinuate path
				break;
			case 13:
				//Terrain path
				break;
			case 14:
				//Forbidden port and race start
				break;
			case 16:
				//Mid Race
				break;
			case 18:
				//End race and Tunnel/box
				break;
		}
	}
	
	/*
	float temp[3] = {0,0,0};
	int sensors[8];
	int i;
	findLine(temp, sensors);
	for(i = 0; i < 8; i++) {
		if(sensors[i]<10)
			Serial.print(" ");
		if(sensors[i]<100)
			Serial.print(" ");
		if(sensors[i]<1000)
			Serial.print(" ");
		Serial.print(sensors[i]);
		Serial.print(" ");
	}
	Serial.println();
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
	*/

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
