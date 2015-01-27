#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "TPoly.h"

const Graph *TPoly::mGraph;
std::ofstream *TPoly::mStream;
//Expects
TPoly::TPoly(const std::vector<unsigned char> &tWeylString)
{ 
	int tLength = tWeylString.size();
	int tNumberOfWeights = mGraph->getUpperBoundEx(tLength).mIndex;

	mPolys = new std::vector<Poly*>(tNumberOfWeights, (Poly*) NULL);
	mBack = new std::vector<Poly*>(tNumberOfWeights, (Poly*) NULL);

	mPolys->at(0) = new Poly(2*(mGraph->getLongestLength())); 
	mPolys->at(0)->add(1, 0, Poly(true)); //Sets T_0 = 1;
	mIndex.push_back(&(mGraph->getZero())); //Tells us that the poly at index 0 is nonzero.
	mGlobalOffset = 0;

	int tEnd = tLength;
	for(int i = 0; i < tEnd; i++)
		multiplyBy(tWeylString[i]);

	delete mBack;
}

TPoly::~TPoly()
{
	for(unsigned int i = 0; i < mIndex.size(); i++)
		delete mPolys->at(mIndex[i]->mIndex);
	delete mPolys;
}

void TPoly::multiplyBy(int aS)
{
	//We need to queue instead of modifying it in the loop.

	unsigned int tStartSize = mIndex.size();
	for(unsigned int i = 0; i < tStartSize; i++)
	{
		Poly& tCurrent = *(mPolys->at(mIndex[i]->mIndex));
		const Weight &tNu = *mIndex[i];
		const Weight &tNuS = *(tNu.mEdges[aS]);

		if(tNuS.mLength > tNu.mLength)
		{
			if(mBack->at(tNu.mIndex) == NULL)
				mBack->at(tNu.mIndex) = new Poly(tCurrent);
			else
				mBack->at(tNu.mIndex)->add(1, 0, tCurrent);

			if(mBack->at(tNuS.mIndex) == NULL)
			{
				mBack->at(tNuS.mIndex) = new Poly(tCurrent);
				if(mPolys->at(tNuS.mIndex) == NULL)
					mIndex.push_back(&tNuS);
			}
			else
				mBack->at(tNuS.mIndex)->add(1, 0, tCurrent);
		}
		else if(tNuS.mLength < tNu.mLength)
		{
			if(mBack->at(tNu.mIndex) == NULL)
			{
				mBack->at(tNu.mIndex) = new Poly(tCurrent);
				mBack->at(tNu.mIndex)->multiplyByPower(2);
			}
			else
				mBack->at(tNu.mIndex)->add(1, 2, tCurrent);

			if(mBack->at(tNuS.mIndex) == NULL)
			{
				mBack->at(tNuS.mIndex) = new Poly(tCurrent);
				mBack->at(tNuS.mIndex)->multiplyByPower(2);
				if(mPolys->at(tNuS.mIndex) == NULL)
					mIndex.push_back(&tNuS);
			}
			else
				mBack->at(tNuS.mIndex)->add(1, 2, tCurrent);
		}
		else
		{
			if(mBack->at(tNu.mIndex) == NULL)
				mBack->at(tNu.mIndex) = new Poly(tCurrent);
			else
				mBack->at(tNu.mIndex)->add(1, 0, tCurrent);

			mBack->at(tNu.mIndex)->add(1, 2, tCurrent);
		}
	}

	std::vector<Poly*> *tSwap = mPolys;
	mPolys = mBack;
	mBack = tSwap;
	mGlobalOffset++;

	for(int i = 0; i < tStartSize; i++)
		if(mBack->at(mIndex[i]->mIndex) != NULL)
		{
			delete mBack->at(mIndex[i]->mIndex);
			mBack->at(mIndex[i]->mIndex) = NULL;
		}
}

std::string printWeight(const unsigned char* tWeight)
{
	std::stringstream tStream;
	tStream << "( " << (int) tWeight[0];
	for(int i = 1; i < RANK - 1; i++)
		tStream << " , " << (int) tWeight[i];
	tStream << " )";
	return tStream.str();
}

std::string TPoly::toString()
{
	int tLength = mGraph->getLongestLength();
	std::stringstream tStream;
	unsigned char tFinal[] = MAX_WEIGHT; 
	std::string sFinal = printWeight(tFinal);
	for(unsigned int i = 0; i < mIndex.size(); i++)
		tStream << printWeight(mIndex[i]->mValue) << '\t' << sFinal << '\t' << (int) (mIndex[i]->mLength) << '\t' << mPolys->at(mIndex[i]->mIndex)->toString() << std::endl;

	return tStream.str();
}

void TPoly::fix()
{
	sort(mIndex.begin(), mIndex.end()); 

	for(int i = (int) mIndex.size() - 2; i >= 0; i--)
	{
		if(i % 100 == 0)
			std::cout << i << std::endl; 

		if(mIndex[i]->mIndex == 5264)
			i = i;

		Poly& tPoly = *(mPolys->at(mIndex[i]->mIndex));

		int tLength = mIndex[i]->mLength;
		int tOurZeroDegree = mGlobalOffset - tLength;
		//tOurZeroDegree = tOurZeroDegree - tOurZeroDegree % 2; //Not sure if necessary.
		Poly tAdd = tPoly.greaterThan(tOurZeroDegree);
		int tZeroth = tPoly.getCoefficient(tOurZeroDegree);
		tAdd.trim();
		if(tAdd.isZero() && tZeroth == 0)
			continue;

		//(*mStream) << "WEIGHT:" << printWeight(mGraph->getWeight(mIndex[i]).mValue) << std::endl;
		//(*mStream) << toString() << std::endl;

		TPoly tNu(mGraph->getWeylString(*(mIndex[i])));

 		addSymmetric(tOurZeroDegree, tAdd, tZeroth, tNu);

		//*mStream << toString() << std::endl;
	}


	//(*mStream) << "Final:\n" << toString() << std::endl;
}

void TPoly::addSymmetric(int aOurZeroDegree, const Poly& aPoly, int aZeroCoefficient, const TPoly& aNuTPoly)
{
	int tEnd = aNuTPoly.mIndex.size();
	if(aOurZeroDegree % 2 == 0)
		for(int i = 0; i < tEnd; i++)
		{
			const Weight &tWeight = *(aNuTPoly.mIndex[i]);

			Poly& tNuPoly = *(aNuTPoly.mPolys->at(tWeight.mIndex));
			if(mPolys->at(tWeight.mIndex) == NULL)
			{
				assert(false);
				(mPolys->at(tWeight.mIndex)) = new Poly(2*(mGraph->getLongestLength())); 
			}
			Poly& tGammaPoly = *(mPolys->at(tWeight.mIndex)); //OK?
			//assert(&tNuPoly != NULL && &tGammaPoly != NULL);
			int tPolyLength = aPoly.getLength();
			for(int j = 0; j < tPolyLength; j++)
			{
				tGammaPoly.add(-aPoly.getCoefficient(2*j), aOurZeroDegree + 2*(j + 1), tNuPoly);
				tGammaPoly.add(-aPoly.getCoefficient(2*j), aOurZeroDegree - 2*(j + 1), tNuPoly);
			}
			tGammaPoly.add(-aZeroCoefficient, aOurZeroDegree, tNuPoly);

			//assert(tGammaPoly.allPositive());
		}
	else
		for(int i = 0; i < tEnd; i++)
		{
			const Weight &tWeight = *(aNuTPoly.mIndex[i]);

			Poly& tNuPoly = *(aNuTPoly.mPolys->at(tWeight.mIndex));
			if(mPolys->at(tWeight.mIndex) == NULL)
			{
				assert(false);
				(mPolys->at(tWeight.mIndex)) = new Poly(2*(mGraph->getLongestLength())); 
			}
			Poly& tGammaPoly = *(mPolys->at(tWeight.mIndex)); //OK?

			int tPolyLength = aPoly.getLength();
			for(int j = 0; j < tPolyLength; j++)
			{
				tGammaPoly.add(-aPoly.getCoefficient(2*j), aOurZeroDegree + 2*(j + 1) - 1, tNuPoly);
				tGammaPoly.add(-aPoly.getCoefficient(2*j), aOurZeroDegree - 2*(j + 1) + 1, tNuPoly);
			}

			//assert(tGammaPoly.allPositive());
		}
}

void TPoly::setGraph(const Graph& aGraph, std::ofstream& aStream)
{
	mGraph = &aGraph;
	mStream = new std::ofstream(DEBUG_FILE);
}
