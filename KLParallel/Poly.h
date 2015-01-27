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
	typedef unsigned long long poly_type; //The underlying data type for coefficients.

	Poly(); //Zero polynomial.
	Poly(bool placeHolder);//Constructor for unity (1t^0).
	Poly(int maxDegree); //Constructs a zero polynomial that will can hold terms of up to t^aMaxDegree.
	Poly(const Poly& other); //Literal copy constructor. May want to trim aOther first.

	//Adds two polynomials. Note that the following must be true: this->mLength >= aOther.mLength + aOffset/2.
	void add(poly_type coefficient, int offset, const Poly& other); 
	poly_type getCoefficient(int degree) const; //Gets polynomial of specified degree.
	void multiplyByPower(int power);
	void clear();
	bool isZero() const { return length == 0; }
	bool allPositive() const;
	Poly greaterThan(int degree) const;
	const int getLength() const { return length; }

	void trim(); //Recalculates size of polynomial, eliminating leading degrees with zero coefficient.

	std::string toString() const; //Converts Poly to string.

	Poly& operator=(const Poly& other); //Literal assignment. May want to trim aOther first.
	bool operator==(const Poly& other) const; //Note must have same length. May want to trim aOther first.
private:
	std::vector<poly_type> coefficients; //Array of coefficients.
	int length; //Represents the max degree of polynomial.

	//friend std::size_t hash_value(Poly const& aPoly);
};

//std::size_t hash_value(Poly const& aPoly);

#endif
