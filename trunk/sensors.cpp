
/*
 * tempName.cpp
 *
 *  Created on: Feb 27, 2009
 *      Author: starswifter
 */

#include "constants.h"

void fillSensorData(short array[8]) {
	char i = 0;
	for(i = 0; i < 8; i++) {
		digitalWrite(A, Aarray[i]);
		digitalWrite(B, Barray[i]);
		digitalWrite(C, Carray[i]);
		array[i] = analogRead(getSensorData);
	}
}

bool gateSensor() {
	return false;
}
