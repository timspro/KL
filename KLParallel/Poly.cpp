#include <assert.h>
#include <sstream>
#include "Poly.h"

//See Poly.h for more comments.

Poly::Poly() : coefficients(0)
{ 
	length = 0; 
}

Poly::Poly(bool placeHolder) : coefficients(1)
{
	length = 1;
	coefficients[0] = (poly_type) 1;
}

Poly::Poly(int maxDegree) :  coefficients((maxDegree + maxDegree%2)/2 + 1)
{
	length = (maxDegree + maxDegree%2)/2 + 1;
	std::fill(&(coefficients[0]), &(coefficients[0]) + length, (poly_type) 0);
}

Poly::Poly(const Poly& other) : coefficients(other.length)
{
	length = other.length;
	if(length != 0)
		std::copy(&(other.coefficients[0]), &(other.coefficients[0]) + other.length, &(coefficients[0]));
}

//Recomputes what the max degree should be.
void Poly::trim()
{
	int i;
	for(i = length - 1; i >= 0; i--)
		if(coefficients[i] != (poly_type) 0)
			break;
	length = i + 1;
}

Poly::poly_type Poly::getCoefficient(int degree) const //Gets the coefficient of the maximum degree term.
{
	if(degree % 2 == 1 || degree / 2 >= length)
		return (poly_type) 0;
	return coefficients[degree / 2];
}

//Adds two polynomials. Note that the following must be true: this->mLength >= aOther.mLength + aOffset/2.
void Poly::add(poly_type coefficient, int offset, const Poly& other) 
{
	int halfOffset = offset/2;
	assert(other.length - halfOffset >= 0 && other.length <= length && offset % 2 == 0);
	for(int i = 0; i < other.length - halfOffset; i++)
	{
		int degree = i + halfOffset;
		coefficients[degree] += coefficient*other.coefficients[i];
	}
}

void Poly::multiplyByPower(int power)
{
	power = power/2;
	for(int i = length - 1; i >= power; i--)
		coefficients[i] = coefficients[i - power];
	std::fill(&(coefficients[0]), &(coefficients[0])+power, (poly_type) 0);
}

std::string Poly::toString() const //Converts poly to string.
{
	std::stringstream result;
	for(int i = length - 1; i >= 0; i--)
	{
		poly_type coefficient = coefficients[i];
		if (coefficient > (poly_type) 0)
			result << "+ " << coefficient << "t^" << (2*i) << " ";
		else if (coefficient < (poly_type) 0)
			result << "- " << -coefficient << "t^" << (2*i) << " ";
	}
	return result.str();
}

Poly& Poly::operator=(const Poly& other)
{
	if(this == &other)
		return *this;

	length = other.length;
	coefficients.resize(other.length);
	if(length != 0)
		std::copy(&(other.coefficients[0]), &(other.coefficients[0]) + other.length, &(coefficients[0]));

	return *this;
}

bool Poly::operator==(const Poly& other) const
{
	if(length != other.length)
		return false;
	return std::equal(&(coefficients[0]), &(coefficients[0]) + length, &(other.coefficients[0]));
}

Poly Poly::greaterThan(int degree) const
{
	int halfDegree = degree/2;
	assert(2*(length - 1) - degree - 2 >= 0);
	Poly result(2*(length - 1) - degree - 2);
	std::copy( &(coefficients[0]) + halfDegree + 1, &(coefficients[0]) + length, &(result.coefficients[0]));
	return result;
}

void Poly::clear()
{
	std::fill(&(coefficients[0]), &(coefficients[0]) + length, (poly_type) 0);
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
	for(int i = 0; i < length; i++)
		if(coefficients[i] < 0)
			return false;
	return true;
}
