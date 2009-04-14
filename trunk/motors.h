/*
 * motors.h
 *
 *  Created on: Mar 17, 2009
 *      Author: starswifter
 */

#ifndef MOTORS_H_
#define MOTORS_H_

void setLeftMotor(char speed, char direction);
void setRightMotor(char speed, char direction);
void setBothMotors(char speed, char direction);
void turnRight(int angle);
void turnLeft(int angle);
void goDistance(int distance, char direction);

#endif
