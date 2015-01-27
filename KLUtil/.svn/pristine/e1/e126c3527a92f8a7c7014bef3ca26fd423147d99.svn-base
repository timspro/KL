#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>

using namespace std;

#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#define RANK 5
const int MAX_WEYL_S_STR = 60; //Guess

const int w0Length = RANK*(RANK-1)/2; //(RANK*(RANK-1))/2
const int w0array[MAX_WEYL_S_STR] =
{1,2,1,3,2,1,4,3,2,1}; //Leave for now.

const int alpharootarray[RANK][RANK-1] = { //Straightforward to change.
{1,1,1,1}, //Used for rho (half sum of all roots).
{2,-1,0,0},
{-1,2,-1,0},
{0,-1,2,-1},
{0,0,-1,2}};

const int InverseMatrixForWeightToRootConversion[RANK-1][RANK-1] = { //Straightforward to change.
{4,3,2,1},{3,6,4,2},{2,4,6,3},{1,2,3,4}};

#endif
