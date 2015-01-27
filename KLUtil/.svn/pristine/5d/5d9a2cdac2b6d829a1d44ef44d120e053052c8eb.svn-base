#include "weight.h"

#ifndef WEYLSTR_H
#define WEYLSTR_H

class WeylStr {
public:
	WeylStr(); //initialized to zero
	WeylStr(const int String[MAX_WEYL_S_STR]);
	void SetWeylStr(const int S[MAX_WEYL_S_STR]);
	void Append_S(int si);
	void GetWeylStr(int S[MAX_WEYL_S_STR]) const;
	int GetElement(int i) const;
	//here, from left to right starting at 1
	//ie the "first" s5 is in spot 11!
	Weight ActOn(const Weight &W) const;
	Weight DotOn(const Weight &W) const;
	int GetLength() const;
	//(s1s2s3s4s5).ChopOff(2) = s1s2s3
	WeylStr ChopOffLastiElements(int i) const;
	WeylStr(const WeylStr &W);
	WeylStr& operator=(const WeylStr &W);
private:
	int Str[MAX_WEYL_S_STR];
};

WeylStr operator*(const WeylStr &W1, const WeylStr &W2); //just cancatenates
ostream &operator<<(ostream &out, const WeylStr& W);
istream &operator>>(istream &in, WeylStr &W);
//this == will not work with strings containing s(RANK), only si's for i= 1,...,RANK-1
int operator==(const WeylStr &W1, const WeylStr &W2);


#endif
