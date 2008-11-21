/*
 *  Music.h
 *  Robocup
 *
 *  Created by Frederik Sabroe on 20/11/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

//Tone Frequencies for octave 7
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

//Macro to reach other octaves
#define notePitch(note,octave) ((note)<<(6-octave))

void playNote(int tone, int octave, long int duration, int pin);

void playMelody(int pin);
