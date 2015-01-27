#include "weight.h"

Weight::Weight() {
	int i;
	for (i = 0; i < RANK-1; i++)
		Warray[i] = 0;
}

Weight::Weight(const int weightarray[RANK-1]) {
	int i;
	for (i = 0; i < RANK-1; i++)
		Warray[i] = weightarray[i];
}

int Weight::Get(int i) const {
	assert( (i > 0) && (i < RANK) );
	return Warray[i-1];
}

Weight::Weight(const Weight &W) {
	int i;
	for (i = 1; i < RANK; i++)
		Warray[i-1] = W.Get(i);
}

Weight& Weight::operator=(const Weight &W) {
	if (this == &W)
		return *this;
	int i;
	for (i = 1; i < RANK; i++)
		Warray[i-1] = W.Get(i);
	return *this;
}

int operator<(const Weight &W1, const Weight &W2) {
	int r = 1;
	int i;
	for ( i = 1; i < RANK; i++)
		if (W1.Get(i) > W2.Get(i))
			r = 0;
	if (W1 == W2)
		r = 0;
	return r;
}

int operator<=(const Weight &W1, const Weight &W2) {
	int r = 1;
	int i;
	for ( i = 1; i < RANK; i++)
		if (W1.Get(i) > W2.Get(i))
			r = 0;
	return r;
}

int operator==(const Weight &W1, const Weight &W2) {
	int r = 1;
	int i;
	for ( i = 1; i < RANK; i++)
		if (W1.Get(i) != W2.Get(i))
			r = 0;
	return r;
}

Weight operator*(int j, const Weight &W) {
	int N[RANK-1];
	int i;
	for (i = 0; i < RANK-1; i++)
		N[i] = j*W.Get(i+1);
	return Weight(N);
}

Weight operator+(const Weight &W1, const Weight &W2) {
	int N[RANK-1];
	int i;
	for (i = 1; i < RANK; i++)
		N[i-1] = W1.Get(i)+W2.Get(i);
	return Weight(N);
}

Weight operator-(const Weight &W1, const Weight &W2) {
	return (W1+((-1)*W2));
}

ostream &operator<<(ostream &out, const Weight &W) {
	out << "( " << W.Get(1);
	int i;
	for ( i = 2; i < RANK; i++)
		out << " , " << W.Get(i);
	out << " )";
	return out;
}

istream &operator>>(istream &in, Weight &W) {
	int w[RANK-1];
	in.ignore(7, (int) '(');
	int i;
	for (i = 0; i < RANK-2; i++) {
		in >> w[i];
		in.ignore(3, (int) ',');
	}
	in >> w[i];
	in.ignore(3, (int) ')');
	W = Weight(w);
	return in;
}

Weight Si(const Weight &W, int i) {
	assert( (0 < i) && (i <= RANK) );
	if (i < RANK)
		return (W - W.Get(i)*Weight(alpharootarray[i]));

	int j;
	int m = 0;
	Weight alphaT;
	for ( j = 1; j < RANK; j++) {
		alphaT = alphaT+Weight(alpharootarray[j]);
		m += W.Get(j);
	}
	return ( W - (m+PRIME)*alphaT );
}

int CanConvertToIntegerRoot(const Weight &W) {
	Weight a;
	int i;
	for (i = 1; i < RANK; i++)
		a=a+(W.Get(i)*Weight(InverseMatrixForWeightToRootConversion[i-1]));
	int r = 1;
	for ( i = 0; i < RANK-1; i++)
		if (a.Get(i+1)%RANK)
			r = 0;
	return r;
}

Weight ConvertToRootNotation(const Weight &W) {
	Weight a;
	int i;
	for (i = 1; i < RANK; i++)
		a=a+(W.Get(i)*Weight(InverseMatrixForWeightToRootConversion[i-1]));
	int N[RANK-1];
	for ( i = 0; i < RANK-1; i++) {
		N[i] = a.Get(i+1) / RANK;
		assert(!(a.Get(i+1)%RANK));
	}
	return Weight(N);
}


//CHECK THIS, CAN the Weight be EQUAL TO PRIME*RHO???
int IsDominantAndRestricted(const Weight &W) {
	const Weight top = ConvertToRootNotation( (PRIME-1)*Weight(alpharootarray[0]));
	Weight w = ConvertToRootNotation(W);
	Weight zero;
	return ((zero <= w)&&(w < top));
}


int AmIaSumOfPositiveRoots(const Weight &Root) {
	int r = 1;
	int i;
	for (i = 1; i < RANK; i++)
		if (Root.Get(i) < 0)
			r = 0;
	return r;
}         
