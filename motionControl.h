/*
 * motionControl.h
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#ifndef MOTIONCONTROL_H_
#define MOTIONCONTROL_H_

void findLine(float array[3], int sensor[8]);
void calculateMotorSpeedFromLine(float line);
void turnDegrees(int degree);
void stop(int direction);

#endif
