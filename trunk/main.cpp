#include "WProgram.h"
#include "SoftwareSerial.h"
#include "Print.h"
#include "music.h"

// variables
int ledPin = 13;
int speakerPin = 4;
int analogValue = 0;

void setup(){
	Serial.begin(9600);	// opens serial port, sets data rate to 9600 bps
	
	pinMode(speakerPin, OUTPUT); // sets the digital pin as output
}

void loop(){
	playMelody(speakerPin);
}

int main(void){
	init();
	setup();
	for(;;)
		loop();
	
	//Return should never be reached
	return 0;
}
