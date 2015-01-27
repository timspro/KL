#include <assert.h>
#include <sstream>
#include "Poly.h"

//See Poly.h for more comments.

Poly::Poly() : mCoefficients(0)
{ 
	mLength = 0; 
}

Poly::Poly(bool aPlaceHolder) : mCoefficients(1)
{
	mLength = 1;
	mCoefficients[0] = 1;
}

Poly::Poly(int aMaxDegree) :  mCoefficients((aMaxDegree + aMaxDegree%2)/2 + 1)
{
	mLength = (aMaxDegree + aMaxDegree%2)/2 + 1;
	std::fill(&(mCoefficients[0]), &(mCoefficients[0]) + mLength, 0);
}

Poly::Poly(const Poly& aOther) : mCoefficients(aOther.mLength)
{
	mLength = aOther.mLength;
	std::copy(&(aOther.mCoefficients[0]), &(aOther.mCoefficients[0]) + aOther.mLength, &(mCoefficients[0]));
}

//Recomputes what the max degree should be.
void Poly::trim()
{
	int i;
	for(i = mLength - 1; i >= 0; i--)
		if(mCoefficients[i] != 0)
			break;
	mLength = i + 1;
}

Poly::poly_type Poly::getCoefficient(int aDegree) const //Gets the coefficient of the maximum degree term.
{
	if(aDegree % 2 == 1 || aDegree / 2 >= mLength)
		return 0;
	return mCoefficients[aDegree / 2];
}

//Adds two polynomials. Note that the following must be true: this->mLength >= aOther.mLength + aOffset/2.
void Poly::add(int aCoefficient, int aOffset, const Poly& aOther) 
{
	int aHalfOffset = aOffset/2;
	assert(aOther.mLength - aHalfOffset >= 0 && aOther.mLength <= mLength && aOffset % 2 == 0);
	for(int i = 0; i < aOther.mLength - aHalfOffset; i++)
	{
		int tDegree = i + aHalfOffset;
		mCoefficients[tDegree] += aCoefficient*aOther.mCoefficients[i];
	}
}

void Poly::multiplyByPower(int aPower)
{
	aPower = aPower/2;
	for(int i = mLength - 1; i >= aPower; i--)
		mCoefficients[i] = mCoefficients[i - aPower];
	std::fill(&(mCoefficients[0]), &(mCoefficients[0])+aPower, 0);
}

std::string Poly::toString() const //Converts poly to string.
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

Poly& Poly::operator=(const Poly& aOther)
{
	if(this == &aOther)
		return *this;

	mLength = aOther.mLength;
	mCoefficients.resize(aOther.mLength);
	std::copy(&(aOther.mCoefficients[0]), &(aOther.mCoefficients[0]) + aOther.mLength, &(mCoefficients[0]));

	return *this;
}

bool Poly::operator==(const Poly& aOther) const
{
	if(mLength != aOther.mLength)
		return false;
	return std::equal(&(mCoefficients[0]), &(mCoefficients[0]) + mLength, &(aOther.mCoefficients[0]));
}

Poly Poly::greaterThan(int aDegree) const
{
	int aHalfDegree = aDegree/2;
	assert(2*(mLength - 1) - aDegree - 2 >= 0);
	Poly tReturn(2*(mLength - 1) - aDegree - 2);
	std::copy( &(mCoefficients[0]) + aHalfDegree + 1, &(mCoefficients[0]) + mLength, &(tReturn.mCoefficients[0]));
	return tReturn;
}

void Poly::clear()
{
	std::fill(&(mCoefficients[0]), &(mCoefficients[0]) + mLength, 0);
}

//std::size_t hash_value(Poly const& aPoly) //Maybe create a better hash?
//{
//    int tHash = 0;
//    int tMult = 1;
//	for(int i = 0; i < aPoly.mLength; i++)
//	{
//		tHash += aPoly.mCoefficients[i]*tMult;
//        tMult *= 10;
//	}
//    return tHash;
//}

bool Poly::allPositive() const
{
	for(int i = 0; i < mLength; i++)
		if(mCoefficients[i] < 0)
			return false;
	return true;
}
