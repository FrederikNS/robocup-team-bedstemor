/*
 * motionControl.h
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#ifndef MOTIONCONTROL_H_
#define MOTIONCONTROL_H_

void findLine(char* situation, short lineLocations[2], short sensor[8]);
void calculateMotorSpeedFromLine(short line, short speedLimit);
void turnDegrees(short degree);
void stop(char direction);

#endif
