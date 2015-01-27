#include <assert.h>
#include <sstream>
#include "Poly.h"

//See Poly.h for more comments.

//Zero polynomial.
Poly::Poly() : mCoefficients(1)
{ 
	mLength = 1;
	mCoefficients[0] = 0;
}

//One polynomial/
Poly::Poly(bool aPlaceHolder) : mCoefficients(1)
{
	mLength = 1;
	mCoefficients[0] = 1;
}

//Creates a polynomial that has a maximum degree of that specified (inclusive).
Poly::Poly(int aMaxDegree) :  mCoefficients((aMaxDegree + aMaxDegree%2)/2 + 1)
{
	mLength = (aMaxDegree + aMaxDegree%2)/2 + 1;
	std::fill(&(mCoefficients[0]), &(mCoefficients[0]) + mLength, 0);
}

//Copy constructor.
Poly::Poly(const Poly& aOther)
{
	//int i;
	//for(i = aOther.mLength - 1; i >= 1; i--)
	//	if(aOther.mCoefficients[i] != 0)
	//		break;
	//int tLength = i + 1;

	mLength = aOther.mLength;
	mCoefficients.resize(mLength);
	std::copy(&(aOther.mCoefficients[0]), &(aOther.mCoefficients[0]) + mLength, &(mCoefficients[0]));
}

//Copy constructor that also lets you specify the max degree.
Poly::Poly(const Poly& aOther, int aSize) : mCoefficients(aSize / 2)
{
	mLength = aSize / 2;
	std::copy(&(aOther.mCoefficients[0]), &(aOther.mCoefficients[0]) + aOther.mLength, &(mCoefficients[0]));
	std::fill(&(mCoefficients[aOther.mLength]), &(mCoefficients[aOther.mLength]) + (mLength - aOther.mLength), 0);
}

//Recomputes what the max degree should be. Note doesn't actually modify polynomial.
void Poly::trim()
{
	int i;
	for(i = mLength - 1; i >= 1; i--)
		if(mCoefficients[i] != 0)
			break;
	mLength = i + 1;
}

//Gets the coefficient of the maximum degree term. Returns zero if odd.
Poly::poly_type Poly::getCoefficient(int aDegree) const 
{
	if(aDegree % 2 == 1 || aDegree / 2 >= mLength)
		return 0;
	return mCoefficients[aDegree / 2];
}

//Adds two polynomials. Note that this polynomial has to be already "big" enough to hold new terms
//which means that the following must be true: this->mLength >= aOther.mLength + aOffset/2.
void Poly::add(int aCoefficient, int aOffset, const Poly& aOther) 
{
	int aHalfOffset = aOffset/2;
	assert(aOther.mLength + aHalfOffset <= mLength && aOffset % 2 == 0);
	for(int i = 0; i < aOther.mLength; i++)
	{
		int tDegree = i + aHalfOffset;
		mCoefficients[tDegree] += aCoefficient*aOther.mCoefficients[i];
	}
}

//void Poly::multiplyByPower(int aPower)
//{
//	aPower = aPower/2;
//	for(int i = mLength - 1; i >= aPower; i--)
//		mCoefficients[i] = mCoefficients[i - aPower];
//	std::fill(&(mCoefficients[0]), &(mCoefficients[0])+aPower, 0);
//}

//Converts poly to string.
std::string Poly::toString() const 
{
	std::stringstream tResult;
	for(int i = mLength - 1; i >= 0; i--)
	{
		poly_type tCoefficient = mCoefficients[i];
		if (tCoefficient > 0)
			tResult << "+ " << tCoefficient << "t^" << (2*i) << " ";
		else if (tCoefficient < 0)
			tResult << "- " << -tCoefficient << "t^" << (2*i) << " ";
	}
	return tResult.str();
}

//Assignment operator.
Poly& Poly::operator=(const Poly& aOther)
{
	if(this == &aOther)
		return *this;

	mLength = aOther.mLength;
	mCoefficients.resize(aOther.mLength);
	std::copy(&(aOther.mCoefficients[0]), &(aOther.mCoefficients[0]) + aOther.mLength, &(mCoefficients[0]));

	return *this;
}

//Tests to see if two polynomials are equal. Note that they don't need to have same lengths (i.e. one could have zeros at end.)
bool Poly::operator==(const Poly& aOther) const
{
	if(aOther.mLength > mLength)
	{
		if(!std::equal(&(mCoefficients[0]), &(mCoefficients[0]) + mLength, &(aOther.mCoefficients[0])))
			return false;
		for(int i = mLength; i < aOther.mLength; i++)
			if(aOther.mCoefficients[i] != 0)
				return false;
	}
	else
	{
		if(!std::equal(&(mCoefficients[0]), &(mCoefficients[0]) + aOther.mLength, &(aOther.mCoefficients[0])))
			return false;
		for(int i = aOther.mLength; i < mLength; i++)
			if(mCoefficients[i] != 0)
				return false;
	}

	return true;
}

//Gets the terms that are greater than a specified degree.
Poly Poly::greaterThan(int aDegree) const
{
	int aHalfDegree = aDegree/2;
	if(mLength - aHalfDegree > 1)
	{
		Poly tReturn(2*(mLength - aHalfDegree - 1));
		std::copy( &(mCoefficients[0]) + aHalfDegree + 1, &(mCoefficients[0]) + mLength, &(tReturn.mCoefficients[0]));
		return tReturn;
	}
	return Poly();
}

//Zeros out the polynomial.
void Poly::clear()
{
	std::fill(&(mCoefficients[0]), &(mCoefficients[0]) + mLength, 0);
}


//Checks to see if all coefficients are positive.
bool Poly::allPositive() const
{
	for(int i = 0; i < mLength; i++)
		if(mCoefficients[i] < 0)
			return false;
	return true;
}

//A rudimentary hash function.
unsigned int hash_value(Poly const& aPoly)
{
    int tHash = 0;
    int tMult = 1;
	for(int i = 0; i < aPoly.mLength; i++)
	{
		tHash += aPoly.mCoefficients[i]*tMult;
        tMult *= 10;
	}
    return tHash;
}
