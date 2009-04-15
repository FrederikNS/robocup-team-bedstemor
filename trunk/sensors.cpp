
/*
 * tempName.cpp
 *
 *  Created on: Feb 27, 2009
 *      Author: starswifter
 */

#include "WProgram.h"
#include "constants.h"

void fillSensorData(int array[8]) {
	int i;
	for(i = 7; i >= 0; i--) {
		digitalWrite(A, Aarray[i]);
		digitalWrite(B, Barray[i]);
		digitalWrite(C, Carray[i]);
		array[i] = (i==0?array[1]:analogRead(getSensorData));
	}
	/*for(int j=0;j<=7;j++){
		if(array[j]<1000) {
			Serial.print(0);
			if(array[j]<100) {
				Serial.print(0);
				if(array[j]<10) {
					Serial.print(0);
				}
			}
		}
		Serial.print(array[j]);
		Serial.print(" ");
	}
	Serial.println();*/
}

bool gateSensor() {
	return false;
}
