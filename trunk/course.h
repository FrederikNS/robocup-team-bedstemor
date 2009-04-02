/*
 *  course.h
 *  Arduino
 *
 *  Created by Frederik Sabroe on 21/03/09.
 *
 */

#include "constants.h" 

const char paths[18][5] = {
//{from cross,to cross,side of "from cross",side of "to cross",# of gates}
/*1*/{0,1,east,south,3},
/*2*/{1,0,west,east,1},
/*3*/{1,2,north,south,0},
/*4*/{2,3,northwest,south,2},
/*5*/{2,3,north,east,1},
/*6*/{3,4,west,north,0},
/*7*/{4,0,east,west,0},
/*8*/{4,5,south,north,1},
/*9*/{4,5,west,west,2},
/*10*/{5,0,east,west,0},
/*11*/{5,0,south,east,2},
/*12*/{3,6,north,south,0},
/*13*/{6,0,east,northwest,2},
/*14*/{6,7,east,east,3},
/*15*/{6,7,north,south,0},
/*16*/{7,8,west,west,1},
/*17*/{7,8,north,south,0},
/*18*/{8,8,east,north,1}};

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

char weights[8][4] = {
//{north,south,east,west}
/*1*/{1,0,0,2},
/*2*/{0,0,0,1},
/*3*/{2,0,3,1},
/*4*/{0,0,0,1},
/*5*/{0,1,0,0},
/*6*/{0,1,2,3},
/*7*/{0,1,0,2},
/*8*/{0,1,2,0}};

bool visits[8][4];