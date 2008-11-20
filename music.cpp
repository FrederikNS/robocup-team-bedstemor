/*
 *  Music.cpp
 *  Robocup
 *
 *  Created by Frederik Sabroe on 20/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include "Music.h"

void playNote(int tone, int octave, long int duration, int pin){
	long elapsedTime = 0;
	while(elapsedTime <= duration) {
		digitalWrite(speakerPin, HIGH);
		delayMicroseconds(notePitch(tone,octave));
		digitalWrite(speakerPin, LOW);
		delayMicroseconds(notePitch(tone,octave));
		elapsedTime+=notePitch(tone,octave)*2;
	}
}

//{e,ds,e,ds,e,b,d,c,a,  c,e,a,b,  e,gs,b,c,  e,e,ds,e,ds,e,b,d,c,a,  c,e,a,b,  e,c,b,a};
void playMelody(int pin) {
	unsigned int i;
	unsigned int notes[] =     {e,ds,e,ds,e,b,d,c,a,  c,e,a,b,  e,gs,b,c,  e,e,ds,e,ds,e,b,d,c,a,  c,e,a,b,  e,c,b,a};
	unsigned int octave[] =    {4, 4,4, 4,4,3,4,4,3,  4,4,4,4,  4, 4,4,5,  4,5, 5,5, 5,5,4,5,5,4,  4,4,4,4,  5,5,4,4};
	unsigned int durations[] = {4, 4,4, 4,4,4,4,4,1,  4,4,5,2,  4, 4,4,1,  4,4, 4,4, 4,4,4,4,4,1,  4,4,4,1,  4,4,4,1};
	
	for(i=0;i<sizeof(notes)/sizeof(unsigned int);i++) {
		if(notes[i]) {
			playNote(notes[i], octave[i], 1000000/durations[i], pin);
		} else {
			delay(1000/durations[i]);
		}
	}
	delay(5000);
}