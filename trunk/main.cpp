#include "WProgram.h"
#include "SoftwareSerial.h"
#include "Print.h"

#define c  478 //2093
#define cs 451 //2217.46
#define d  426 //2349.32
#define ds 402 //2489.02
#define e  379 //2637.02
#define f  358 //2793.83
#define fs 338 //2959.96
#define g  319 //3135.96
#define gs 301 //3322.44
#define a  284 //3520
#define as 268 //3729.31
#define b  253 //3951.07

#define notePitch(note,octave) ((note)<<(6-octave))

// variables
int ledPin = 13;
int speakerPin = 4;
int analogValue = 0;


void setup(){
	Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps
	
	pinMode(speakerPin, OUTPUT); // sets the digital pin as output
}

void blink(long int duration) {
	while(1){
		digitalWrite(ledPin, HIGH);
		delay(duration);
		digitalWrite(ledPin, LOW);
		delay(duration);
	}
	
}

void playNote(int tone, int octave, long int duration){
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

void playMelody() {
	unsigned int i;
	unsigned int notes[] =     {e,ds,e,ds,e,b,d,c,a,  c,e,a,b,  e,gs,b,c,  e,e,ds,e,ds,e,b,d,c,a,  c,e,a,b,  e,c,b,a};
	unsigned int octave[] =    {4, 4,4, 4,4,3,4,4,3,  4,4,4,4,  4, 4,4,5,  4,5, 5,5, 5,5,4,5,5,4,  4,4,4,4,  5,5,4,4};
	unsigned int durations[] = {4, 4,4, 4,4,4,4,4,1,  4,4,5,2,  4, 4,4,1,  4,4, 4,4, 4,4,4,4,4,1,  4,4,4,1,  4,4,4,1};
	
	for(i=0;i<sizeof(notes)/sizeof(unsigned int);i++) {
		if(notes[i]) {
			playNote(notes[i], octave[i], 1000000/durations[i]);
		} else {
			delay(1000/durations[i]);
		}
	}
	delay(5000);
}

void loop(){
	//blink(1000);
	//playNote(a, 8);
	playMelody();
	//analogValue = analogRead(0);
	
	//Print::print(analogValue);
	//Print::println();
}

int main(void){
	init();
	setup();
	for(;;)
		loop();
	
	//Return should never be reached
	return 0;
}
