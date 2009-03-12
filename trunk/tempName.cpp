/*
 * tempName.cpp
 *
 *  Created on: Feb 27, 2009
 *      Author: starswifter
 */

#include "pins.h"

void fillSensorData(int * array[]) {
	int i = 0;
	for(i = 0; i < 8; i++) {
		digitalWrite(A, Aarrat[i]);
		digitalWrite(B, Barrat[i]);
		digitalWrite(C, Carrat[i]);
		array[i] = analogRead(getSensorData);
	}
}
