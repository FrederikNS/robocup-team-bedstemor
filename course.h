/*
 *  course.h
 *  Arduino
 *
 *  Created by Frederik Sabroe on 21/03/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#define north 0
#define south 1
#define east 2
#define west 3
#define northwest 4

const char paths[17][4] = {
//{from cross,to cross,side of "from cross",side of "to cross"}
/*1*/{0,1,east,south},
/*2*/{1,0,west,east},
/*3*/{1,2,north,south},
/*4*/{2,3,northwest,south},
/*5*/{2,3,north,east},
/*6*/{3,4,west,north},
/*7*/{4,0,east,west},
/*8*/{4,5,south,north},
/*9*/{5,0,east,west},
/*10*/{5,0,south,east},
/*11*/{3,6,north,south},
/*12*/{6,0,east,northwest},
/*13*/{6,7,east,east},
/*14*/{6,7,north,south},
/*15*/{7,8,west,west},
/*16*/{7,8,north,south},
/*17*/{8,8,east,north}};

const char crosses[8][4] = {
//{north,south,east,west}
/*1*/{3,1,0,2},
/*2*/{5,3,0,4},
/*3*/{11,4,5,6},
/*4*/{6,8,7,0},
/*5*/{8,10,9,0},
/*6*/{14,11,13,12},
/*7*/{16,14,13,15},
/*8*/{17,16,17,15}};