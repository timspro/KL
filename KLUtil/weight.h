#include "globals.h"

#ifndef WEIGHT_H
#define WEIGHT_H

class Weight {
public:
	Weight(); //initializes to zero
	Weight(const int weightarray[RANK-1]);
	int Get(int i) const;
	int ConvertToInt() const;
	Weight(const Weight &W);
	Weight& operator=(const Weight &W);
private:
	int Warray[RANK-1];
};

Weight Si(const Weight &W, int i); //returns Si(W)
Weight ConvertToRootNotation(const Weight &W); //crashes if W is not a sum of integer roots
int IsDominantAndRestricted(const Weight &W);

int operator==(const Weight &W1, const Weight &W2);
//to have <, W2 - W1 must have at least 1 pos tuple and no negatives
int operator<(const Weight &W1, const Weight &W2);
int operator<=(const Weight &W1, const Weight &W2);
Weight operator*(int j, const Weight &W);
Weight operator+(const Weight &W1, const Weight &W2);
Weight operator-(const Weight &W1, const Weight &W2);
ostream &operator<<(ostream &out, const Weight &W);
istream &operator>>(istream &in, Weight &W);

int AmIaSumOfPositiveRoots(const Weight &Root); //for root notation
#endif
