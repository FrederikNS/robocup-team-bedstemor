/*
 * motionControl.h
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#ifndef MOTIONCONTROL_H_
#define MOTIONCONTROL_H_

void findLine(char* situation, int lineLocations[2], int sensor[8]);
void calculateMotorSpeedFromLine(int line, int speedLimit);
void turnDegrees(int degree);
void stop(char direction);
void leftInterrupt();
void rightInterrupt();
void stop(char direction);
void goDistance(int distance, char direction);

#endif
