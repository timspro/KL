#include <string>
#include <vector>

#ifndef POLY_H
#define POLY_H

//Poly class to represent polynomials in t^2. Note that
//functions can (and should) accept arguments that are odd.
//In most cases, the value will be rounded up to the nearest
//even number.

class Poly
{
public:
	typedef int poly_type; //The underlying data type for coefficients.

	Poly(); //Zero polynomial.
	Poly(bool aPlaceHolder);//Constructor for unity (1t^0).
	Poly(int aMaxDegree); //Constructs a zero polynomial that will can hold terms of up to t^aMaxDegree.
	Poly(const Poly& aOther); //Literal copy constructor. May want to trim aOther first.
	Poly(const Poly& aOther, int aGrowth);

	//Adds two polynomials. Note that the following must be true: this->mLength >= aOther.mLength + aOffset/2.
	void add(int aCoefficient, int aOffset, const Poly& aOther); 
	poly_type getCoefficient(int aDegree) const; //Gets polynomial of specified degree.
	void multiplyByPower(int aPower);
	void clear();
	bool isZero() const { return mLength == 1 && mCoefficients[0] == 0; }
	bool allPositive() const;
	Poly greaterThan(int aDegree) const;
	const int getLength() const { return mLength; }

	void trim(); //Recalculates size of polynomial, eliminating leading degrees with zero coefficient.

	std::string toString() const; //Converts Poly to string.

	Poly& operator=(const Poly& aOther); //Literal assignment. May want to trim aOther first.
	bool operator==(const Poly& aOther) const; //Note must have same length. May want to trim aOther first.
private:
	std::vector<poly_type> mCoefficients; //Array of coefficients.
	int mLength; //Represents the max degree of polynomial.

	friend unsigned int hash_value(Poly const& aPoly);
};

unsigned int hash_value(Poly const& aPoly);

#endif
