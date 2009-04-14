/*
 * motionControl.h
 *
 *  Created on: Feb 19, 2009
 *      Author: starswifter
 */

#ifndef MOTIONCONTROL_H_
#define MOTIONCONTROL_H_

void findLine(float array[3], short sensor[8]);
void calculateMotorSpeedFromLine(float line);
void turnDegrees(short degree);
void stop(char direction);

#endif
