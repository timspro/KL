#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>

using namespace std;

#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#define RANK 9
const int MAX_WEYL_S_STR = 200; // This is a guess.

const int w0Length = RANK*(RANK-1)/2;
const int w0array[MAX_WEYL_S_STR] =
{1,2,1,3,2,1,4,3,2,1,
5,4,3,2,1,6,5,4,3,2,
1,7,6,5,4,3,2,1,8,7,
6,5,4,3,2,1}; 

const int alpharootarray[RANK][RANK-1] = { //Straightforward to change.
{1,1,1,1,1,1,1,1}, //Used for rho (half sum of all roots).
{ 2,-1,0,0,0,0,0,0},
{-1,2,-1,0,0,0,0,0},
{0,-1,2,-1,0,0,0,0},
{0,0,-1,2,-1,0,0,0},
{0,0,0,-1,2,-1,0,0},
{0,0,0,0,-1,2,-1,0},
{0,0,0,0,0,-1,2,-1},
{0,0,0,0,0,0,-1, 2}};

const int InverseMatrixForWeightToRootConversion[RANK-1][RANK-1] = { //Straightforward to change.
{8,7,6,5,4,3,2,1},
{7,14,12,10,8,6,4,2},
{6,12,18,15,12,9,6,3},
{5,10,15,20,16,12,8,4},
{4,8,12,16,20,15,10,5},
{3,6,9,12,15,18,12,6},
{2,4,6,8,10,12,14,7},
{1,2,3,4,5,6,7,8}};

#endif
