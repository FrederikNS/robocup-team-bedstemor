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
#include "sensors.h"
#include "course.h"

//#define DEBUG

//http://www.ikalogic.com/tut_line_sens_algo.php

void setup() {
	Serial.begin(9600); //Set serial baud rate

	//Assign pins
	pinMode(leftMotorSpeedPin, OUTPUT);
	pinMode(leftMotorDirectionPin, OUTPUT);
	pinMode(rightMotorSpeedPin, OUTPUT);
	pinMode(rightMotorDirectionPin, OUTPUT);
	pinMode(enableMotor, OUTPUT);
	pinMode(A, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(C, OUTPUT);
	pinMode(enableLight, OUTPUT);
	pinMode(startButton, INPUT);
	pinMode(getDistanceData, INPUT);
	pinMode(INTERRUPT_R,INPUT);
	pinMode(INTERRUPT_L,INPUT);
	attachInterrupt(1,leftInterrupt,RISING);
	attachInterrupt(0,rightInterrupt,RISING);
	noInterrupts();
}

void loop() {
	//Wait for start signal
	//while(digitalRead(startButton) == LOW){}
	/*while(1) {
		Serial.println(analogRead(getDistanceData));
	}*/

	while(analogRead(getDistanceData) <= 100){}
#ifdef DEBUG
	Serial.println("Start");
#endif

	digitalWrite(enableMotor, 1); //Enable motors
	digitalWrite(enableLight, 1); //Enable light

	//Whereabout details
	char currentPath = 1; //The path the robot is on currently
	char lastCross = 0; //The last cross the robot passed
	char nextCross = 1; //The next cross the robot will encounter
	char entryDirection = east; //The direction the start of the path meets the cross
	char exitDirection = south; //The direction the robot will enter the next cross

	//Cross Priority
	char preferredDirection = 5; //The way the robot will prefer going
	char preferredDirectionValue = 0; //The priority the preferredPath has

	int sensors[8]; //Array for the sensor values
	int lineLocations[2]; //Array for location of lines

	/*What situation the robot is currently in
	 -3: Robot sees a branch to both sides
	 -2: Robot sees a branch to the (left/right)
	 -1: Robot sees a branch to the (left/right)
	 0: Robot sees no lines
	 1: Robot sees one line
	 2: Robot sees two lines */
	char situation = 1;

	Serial.print("Situation: ");
	Serial.println((int)situation);

	char gatesSeen[18]; //Gate count array
	unsigned long gateDelay; //variable to keep track of time since last gate to avoid detecting a gate twice

	int speedLimit = 500;//Variable for limiting the speed of the robot


	bool run; //Variable to keep track of when to break loops


	char i; //Iteration variable

	while(1) {
		//Cross handling
		//Serial.print("Situation: ");
		//Serial.println((int)situation);
		/*for(int j=1;j<=7;j++){
			if(sensors[j]<1000) {
				Serial.print(0);
				if(sensors[j]<100) {
					Serial.print(0);
					if(sensors[j]<10) {
						Serial.print(0);
					}
				}
			}
			Serial.print(sensors[j]);
			Serial.print(" ");
		}
		Serial.println();*/
		if(0/*currentPath&cross==cross*/) {
#ifdef DEBUG
			Serial.println("Cross Found");
#endif
			//search for the path with the highest priority
			for(i=0;i<=4;i++) {
				if(exitDirection != i && preferredDirectionValue<weights[nextCross-1][i]) {
					preferredDirection = i;
					preferredDirectionValue = weights[nextCross-1][i];
				}
			}
#ifdef DEBUG
			Serial.println("Preferred path: ");
			Serial.print(i);
#endif

			//Turn the robot to the preferredPath
			switch(exitDirection) {
				case north:
					switch(preferredDirection) {
						case south:
							goDistance(5,FORWARD);
							break;
						case east:
							turnDegrees(90);
							break;
						case west:
							turnDegrees(270);
							break;
					}
					break;
				case south:
					switch(preferredDirection) {
						case north:
							goDistance(5, FORWARD);
							break;
						case east:
							turnDegrees(270);
							break;
						case west:
							turnDegrees(90);
							break;
					}
					break;
				case east:
					switch(preferredDirection) {
						case north:
							turnDegrees(270);
							break;
						case south:
							turnDegrees(90);
							break;
						case west:
							goDistance(5, FORWARD);
							break;
					}
					break;
				case west:
					switch(preferredDirection) {
						case north:
							turnDegrees(90);
							break;
						case south:
							turnDegrees(270);
							break;
						case east:
							goDistance(5, FORWARD);
							break;
					}
					break;
			}

			//Set whereabout variables
			lastCross = nextCross;
			entryDirection = preferredDirection;
			currentPath = crosses[nextCross-1][preferredDirection];
			exitDirection = (paths[currentPath-1][0]==lastCross?paths[currentPath-1][3]:paths[currentPath-1][2]);
			nextCross = (paths[currentPath-1][0]==lastCross?paths[currentPath-1][1]:paths[currentPath-1][0]);
			//End Cross Handling
		} else {
			//Behavioral State Machine

			switch(currentPath&(cross-1)){
				case 1:
				case 3:
				case 5:
				case 6:
				case 15:
				case 17:
					//Simple paths, just follow line
					Serial.print("Situation: ");
					Serial.println((int)situation);
					findLine(&situation, lineLocations, sensors);
					Serial.print("Lines: ");
					Serial.print(lineLocations[0]);
					Serial.print(" ");
					Serial.print(lineLocations[1]);
					Serial.println();
					run=true;
					while(run) {
						Serial.print("Situation: ");
						Serial.println((int)situation);
						switch(situation) {
							case 0:
							case 1:
								calculateMotorSpeedFromLine(lineLocations[0], speedLimit);
								if(gateSensor()&&millis()>gateDelay) {
									gatesSeen[currentPath-1]++;
									gateDelay = millis()+1000;
								}
								findLine(&situation, lineLocations, sensors);
								break;
							case 2:
								stop(FORWARD);
								findLine(&situation, lineLocations, sensors);
								break;
							case -1:
							case -2:
								stop(FORWARD);
								//currentPath |= cross;
								run=false;
								break;
						}
					}
					break;
				case 2:
					//Seesaw
					while(situation==1) {
						findLine(&situation, lineLocations, sensors);
						calculateMotorSpeedFromLine(lineLocations[0], speedLimit);
						if(gateSensor()&&millis()>gateDelay) {
							gatesSeen[currentPath-1]++;
							gateDelay = millis()+1000;
						}
					}

					goDistance(30,FORWARD);
					turnDegrees(90);
					goDistance(300, FORWARD);
					turnDegrees(90);

					while(situation<2) {

					}
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
			//End Behavioral State Machine
		}

	}
}

int main() {
	init();
	setup();
	for(;;) {
		loop();
	}
	return 0;
}
