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
//#define factor multiplyFixed(speedLimit,14)
//#define degreeToTickFactor 8.1*1000

//Variables for the interrupt functions
long leftTick = 0;
long rightTick = 0;

/*
 * Function for calculating the location of a line, based on the data from the sensors.
 */
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
	//Detects the two lowest values and positions in the sensor data
	for(i = 0; i < 8; i++) {
		temp = sensor[i];
		if(temp < value1) {
			lowest1 = i;
			value1 = temp;
		} else if(temp < value2) {
			//The second lowest value can not be right next to the first one
			//since both of them is probably detecting the same line
			if(i != lowest1+1) {
				lowest2 = i;
				value2 = temp;
			}
		}
		//Detects when it enters and leaves a row of sensors which is below the threshold
		//It can only detect up to two lines
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
	//If a streak of low sensors were started but not ended before all the sensors had been read
	//it will end them with the last sensor.
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

	//If (almost) all the sensors are within the first low sensor data range
	//it have detected a cross road
	if(start1 <= 0 && end1 >= 7) {
		*situation = -3;
	} else if(start1 == 0 && end1 >= 4) {
		//If all the sensors from left to the middle is within the first streak
		//it have detected a left turn
		*situation = -1;
	} else if(start1 <= 3 && end1 == 7) {
		//If all the sensors from the middle to the right is within the first streak
		//it have detected a right turn
		*situation = -2;
	} else if(start2 == -1) {
		//If none of the above situations have occurred, and the second streak is not started
		//It have simply detected a single line
		*situation = 1;
		//For calculating the location of the line it will use weighed mean, but
		//only from right before to right next the lowest sensor (with the correct weight according to their position)
		i = lowest1 - 1;
		temp = lowest1 + 1;/*
		Serial.print("Calculating from sensor: ");
		Serial.print(i);
		Serial.print(" to ");
		Serial.println(temp);*/

		//If the lowest value were detected at sensor 0 or 7, it will use a sensor which is outside range
		//So if the lowest is sensor 0, it will not try to get data from sensor -1
		//And if the lowest is sensor 7, it will not try to get data from sensor 8
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
		//Prevents divide by 0
		if(!sum) {
			sum = 1;
		}
		lineLocations[0] = divideFixed(toFixedPoint(line),toFixedPoint(sum));
	} else if(end2 != -1) { //two roads
		//If both streaks of low sensors is set, two lines have been detected
		*situation = 2;
		//Finds the lowest sensor in the first streak
		for(i = start1; i <= end1; i++) {
			temp = sensor[i];
			if(temp < value1) {
				lowest1 = i;
				value1 = temp;
			}
		}
		//Finds the lowest sensor in the second streak
		for(i = start2; i <= end2; i++) {
			temp = sensor[i];
			if(temp < value2) {
				lowest2 = i;
				value2 = temp;
			}
		}
		//Weighed mean line with a single line
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

/*
 * Function for calculating the speed of each motor depending on how far the line is from centrum
 */
void calculateMotorSpeedFromLine(int line, int speedLimit) {
	setLeftMotor(constrain(speedLimit+(fromFixedPoint(line)-4)*60,0,255),FORWARD);
	setRightMotor(constrain(speedLimit-(fromFixedPoint(line)-4)*60,0,255),FORWARD);
}

/*
 * Interrupt function for left motor
 */
void leftInterrupt() {
	leftTick--;
}

/*
 * Interrupt function for right motor
 */
void rightInterrupt() {
	rightTick--;
}

/*
 * Function for turning the robot a certain amount of degrees
 */
void turnDegrees(int degree) {
	Serial.println("turning");
	//Correcting the amount of degrees, so that it will be 0 or above, and below 360
	degree %= 360;

	long ticksRemaining = 0;

	if(degree > 180 || (degree > -180 && degree <= 0)) {
		//If the angle is between 180 and 360 degrees, or between 0 and -180, it will make a right turn
		if(degree < 0) {
			degree *= -1;
		}
		leftTick = rightTick = ((degree*100000)/20000);
		setLeftMotor(255,FORWARD);
		setRightMotor(255,REVERSE);
	} else {
		//If the anle is between 0 and 180, or it is between -180 and -360, it will make a left turn
		if(degree < 0) {
			degree = 360-degree;
		}
		rightTick = leftTick = ((degree*100000)/20000);
		setLeftMotor(255,REVERSE);
		setRightMotor(255,FORWARD);
	}
	ticksRemaining = leftTick + rightTick;
	//Waiting until the amount of ticks is below 0 (it is hard to hit exactly 0)
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

/*
 * Function of stopping the robots movement
 */
void stop(char direction) {
	/*if(direction) {
		setBothMotors(255,REVERSE);
	} else {
		setBothMotors(255,FORWARD);
	}*/
	setBothMotors(0,FORWARD);
}

/*
 * Function for driving a specific distance in some direction
 */
void goDistance(int distance, char direction) {
	Serial.println("goDistance");

	//Calculate the amounts of ticks needed to drive the specified distance
	leftTick = rightTick = 30*distance;
	Serial.println(leftTick);
	long ticksRemaining = leftTick + rightTick;
	setBothMotors(255, direction);
	//Wait until the correct amount of ticks have passed
	while(ticksRemaining > 0) {
		ticksRemaining = leftTick + rightTick;
		Serial.println(leftTick+rightTick);
	}
	stop(direction);
	ticksRemaining = 0;
	leftTick = 0;
	rightTick = 0;
}

/*
 * Function for going straight again, the distance from the row of sensors to the axle of the wheels
 */
void goStraight() {
	Serial.println("Going Straight");
	goDistance(centerRobot, FORWARD);
}

/*
 * Function for turning left
 * Mostly used at sharp corners (like at a cross road)
 */
void goLeft() {
	Serial.println("Going Left");
	goDistance(centerRobot, FORWARD);
	turnDegrees(90);
}

/*
 * Function for turning right
 * Mostly used at sharp corners (like at a cross road)
 */
void goRight() {
	Serial.println("Going Right");
	goDistance(centerRobot, FORWARD);
	turnDegrees(-90);
}
