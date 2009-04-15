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

#define DEBUG

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
	byte currentPath = 1; //The path the robot is on currently
	byte lastCross = 0; //The last cross the robot passed
	byte nextCross = 1; //The next cross the robot will encounter
	byte entryDirection = east; //The direction the start of the path meets the cross
	byte exitDirection = south; //The direction the robot will enter the next cross

	/*//Race
	byte currentPath = 14; //The path the robot is on currently
	byte lastCross = 6; //The last cross the robot passed
	byte nextCross = 7; //The next cross the robot will encounter
	byte entryDirection = east; //The direction the start of the path meets the cross
	byte exitDirection = east; //The direction the robot will enter the next cross*/

	bool cross = 0;
	//Cross Priority
	byte preferredDirection = 5; //The way the robot will prefer going
	byte preferredDirectionValue = 0; //The priority the preferredPath has

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

	byte gatesSeen[18]; //Gate count array
	unsigned long gatetimeout; //variable to keep track of time since last gate to avoid detecting a gate twice

	int speedLimit = 200;//Variable for limiting the speed of the robot

	bool run; //Variable to keep track of when to break loops

	byte i; //Iteration variable

	int startTime = 0;
	int targetTime = 0;
	int temp = 0;


	while(1) {
		//Cross handling
#ifdef DEBUG
		Serial.println(cross);
#endif
		if(!cross) {
			//Behavioral State Machine
#ifdef DEBUG
			Serial.println("Path Found");
#endif
			switch(currentPath){
			case 1:
			case 5:
			case 6:
			case 15:
			case 17:
#ifdef DEBUG
				Serial.print("Path: ");
				Serial.println((int)currentPath);
#endif
				//Simple paths, just follow line
				findLine(&situation, lineLocations, sensors);
#ifdef DEBUG
				Serial.println("Sensors Read");
#endif
				run=true;
				while(run) {
#ifdef DEBUG
					Serial.print("Situation: ");
					Serial.println((int)situation);
#endif
					switch(situation) {
					case 0:
						stop(FORWARD);
						break;
					case 1:
						calculateMotorSpeedFromLine(lineLocations[0], speedLimit);
						if(gateSensor()&&millis()>gatetimeout) {
							gatesSeen[currentPath-1]++;
							gatetimeout = millis()+1000;
						}
						findLine(&situation, lineLocations, sensors);
						break;
					case 2:
						stop(FORWARD);
						findLine(&situation, lineLocations, sensors);
						calculateMotorSpeedFromLine(lineLocations[0], speedLimit);
						break;
					case -1:
					case -2:
					case -3:
					stop(FORWARD);
					cross = true;
					run = false;
					break;
					}
				}
				break;
			case 2:
				//Seesaw
				goDistance(60, FORWARD);
				while(situation==1) {
					speedLimit = 100;
					findLine(&situation, lineLocations, sensors);
					calculateMotorSpeedFromLine(lineLocations[0], speedLimit);
					if(gateSensor()&&millis()>gatetimeout) {
						gatesSeen[currentPath-1]++;
						gatetimeout = millis()+1000;
					}
				}

				goDistance(30,FORWARD);
				turnDegrees(90);
				stop(FORWARD);
				goDistance(330, FORWARD);
				turnDegrees(90);
				stop(FORWARD);

				setBothMotors(150, FORWARD);

				while(situation==0) {findLine(&situation, lineLocations, sensors);}

				turnDegrees(30);

				lastCross=0;
				entryDirection=east;
				currentPath=1;
				exitDirection=south;
				nextCross=1;
				break;
			case 3:
				while(situation!=2) {
					findLine(&situation, lineLocations, sensors);
					calculateMotorSpeedFromLine(lineLocations[0], speedLimit);
				}
				while(situation==2) {
					findLine(&situation, lineLocations, sensors);
					calculateMotorSpeedFromLine(lineLocations[1], speedLimit);
				}

				break;
			case 4:
				//Stairs
				break;
			case 7:
				//need 180 degrees turn?
				goDistance(50, FORWARD);
				turnDegrees(-90);
				goDistance(10, FORWARD);
				turnDegrees(90);
				goDistance(20, FORWARD);
				turnDegrees(90);
				goDistance(50, FORWARD);
				goDistance(50, FORWARD);
				turnDegrees(90);
				goDistance(20, FORWARD);
				turnDegrees(90);
				goDistance(10, FORWARD);
				turnDegrees(-90);
				do {
					calculateMotorSpeedFromLine(14, speedLimit);
					findLine(&situation, lineLocations, sensors);
				} while(situation == 0);
				stop(FORWARD);

				cross = true;

				lastCross=4;
				entryDirection=west;
				currentPath=7;
				exitDirection=south;
				nextCross=5;
				break;
			case 8:
			case 10:
				//Stop immediately, forbidden path
				break;
			case 11:
				//Goal path
				break;
			case centerRobot:
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
				startTime = millis();
				while(!gateSensor()) {
					findLine(&situation, lineLocations, sensors);
					calculateMotorSpeedFromLine(lineLocations[0], speedLimit);
				}
				temp = millis();
				targetTime = temp - startTime;
				startTime = temp;

				while(startTime + targetTime < millis()) {
					findLine(&situation, lineLocations, sensors);
					calculateMotorSpeedFromLine(lineLocations[0], speedLimit);
				}
				turnDegrees(90);
				goDistance(20, FORWARD);
				turnDegrees(-90);
				goDistance(10, FORWARD);
				turnDegrees(90);
				goDistance(5, FORWARD);
				turnDegrees(90);
				goDistance(30, FORWARD);
				turnDegrees(90);
				do {
					calculateMotorSpeedFromLine(14, speedLimit);
					findLine(&situation, lineLocations, sensors);
				} while(situation == 0);
				while(situation==2) {
					findLine(&situation, lineLocations, sensors);
					calculateMotorSpeedFromLine(lineLocations[1], speedLimit);
				}
				break;
			case 18:
				//End race and Tunnel/box
				break;
			}
			//End Behavioral State Machine
		} else {
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
			Serial.print("Preferred path: ");
			Serial.println((int)preferredDirection);
#endif

			//Turn the robot to the preferredPath
			switch(exitDirection) {
			case north:
				switch(preferredDirection) {
				case south:
					goStraight();
					break;
				case east:
					goLeft();
					break;
				case west:
					goRight();
					break;
				}
				break;
				case south:
					switch(preferredDirection) {
					case north:
						goStraight();
						break;
					case east:
						goRight();
						break;
					case west:
						goLeft();
						break;
					}
					break;
					case east:
						switch(preferredDirection) {
						case north:
							goRight();
							break;
						case south:
							goLeft();
							break;
						case west:
							goStraight();
							break;
						}
						break;
						case west:
							switch(preferredDirection) {
							case north:
								goLeft();
								break;
							case south:
								goRight();
								break;
							case east:
								goStraight();
								break;
							}
							break;
			}
			stop(FORWARD);

			//Set whereabout variables
			lastCross = nextCross;
			entryDirection = preferredDirection;
			currentPath = crosses[nextCross-1][preferredDirection];
			exitDirection = (paths[currentPath-1][0]==lastCross?paths[currentPath-1][3]:paths[currentPath-1][2]);
			nextCross = (paths[currentPath-1][0]==lastCross?paths[currentPath-1][1]:paths[currentPath-1][0]);
			cross = false;
			//End Cross Handling
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
