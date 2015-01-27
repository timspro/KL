#include "weylstr.h"

WeylStr::WeylStr() {
	int i;
	for (i = 0; i < MAX_WEYL_S_STR; i++)
		Str[i] = 0;
}

WeylStr::WeylStr(const int String[MAX_WEYL_S_STR]) {
	int i;
	for (i = 0; i< MAX_WEYL_S_STR; i++) {
		Str[i] = String[i];
		assert((0 <= Str[i]) && (Str[i] <= RANK));
	}
	return;
}

void WeylStr::SetWeylStr(const int S[MAX_WEYL_S_STR]) {
	int i;
	for (i = 0; i< MAX_WEYL_S_STR; i++) {
		Str[i] = S[i];
		assert((0 <= Str[i]) && (Str[i] <= RANK));
	}
	return;
}

void WeylStr::Append_S(int si) {
	assert((0 < si) && (si <= RANK));
	int i;
	for (i = 0; (i<MAX_WEYL_S_STR)&&(Str[i] != 0); i++);
	if (i == MAX_WEYL_S_STR)
		cerr << "Strange Error!! WeylStr too long to append" << endl;
	else
		Str[i] = si;
	return;
}

void WeylStr::GetWeylStr(int S[MAX_WEYL_S_STR]) const {
	int i;
	for (i = 0; i < MAX_WEYL_S_STR; i++)
		S[i] = Str[i];
	return;
}

int WeylStr::GetElement(int i) const {
	assert((0 < i)&&(i<=MAX_WEYL_S_STR));
	return Str[i-1];
}

Weight WeylStr::ActOn(const Weight &W) const {
	int i;
	Weight T = W;
	for (i = MAX_WEYL_S_STR-1; i >= 0; i--)
		if ((1 <= Str[i]) && (Str[i] <= RANK))
			T = Si(T,Str[i]);
	return T;
}


Weight WeylStr::DotOn(const Weight &W) const {
	return ( ActOn( W+Weight(alpharootarray[0]) )- Weight(alpharootarray[0]) );
}

int WeylStr::GetLength() const {
	int i;
	int c = 0;
	for (i = 0; i < MAX_WEYL_S_STR; i++)
		if (Str[i] != 0)
			c++;
	return c;
}

WeylStr WeylStr::ChopOffLastiElements(int i) const {
	int S[MAX_WEYL_S_STR];
	GetWeylStr(S);
	int j;
	for (j = GetLength(); (j > 0)&&(j > GetLength() - i); j--)
		S[j-1] = 0;
	return WeylStr(S);
}

WeylStr::WeylStr(const WeylStr &W) {
	int A[MAX_WEYL_S_STR];
	W.GetWeylStr(A);
	SetWeylStr(A);
}

WeylStr& WeylStr::operator=(const WeylStr &W) {
	if (this == &W)
		return *this;
	int A[MAX_WEYL_S_STR];
	W.GetWeylStr(A);
	SetWeylStr(A);
	return *this;
}

ostream &operator<<(ostream &out, const WeylStr &W) {
	int i;
	for (i = 1; i <= MAX_WEYL_S_STR; i++)
		if (W.GetElement(i) != 0)
			out << "s" << W.GetElement(i);
	return out;
}

WeylStr operator*(const WeylStr &W1, const WeylStr &W2) {
	int S[MAX_WEYL_S_STR];
	int i,j;
	for (i = 0; i < W1.GetLength(); i++)
		S[i] = W1.GetElement(i+1);
	for (j = 0; j < MAX_WEYL_S_STR - i; j++)
		S[i+j] = W2.GetElement(j+1);
	return WeylStr(S);
}

istream &operator>>(istream &in, WeylStr &W) {
	int array[MAX_WEYL_S_STR];
	int i;
	int stop=0;
	in.ignore(5,(int) 's');
	in >> array[0];
	for (i =1; (!stop)&&(i <MAX_WEYL_S_STR) ; i++) {
		if (in.peek() == 's') {
			in.get();
			in >> array[i];
		}
		else
			stop = i;
	}
	for ( i = stop; i < MAX_WEYL_S_STR; i++)
		array[i] = 0;
	W = WeylStr(array);
	return in;
}

int operator==(const WeylStr &W1, const WeylStr &W2) {
	int r = 1;
	int i;
	for (i = 1; i < RANK; i++)
		if (!( W1.ActOn(alpharootarray[i]) == W2.ActOn(alpharootarray[i]) ))
			r = 0;
	return r;
}         
