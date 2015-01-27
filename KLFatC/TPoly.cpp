#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "TPoly.h"

//See TPoly.h for more information about underlying data structures.

//The graph that is used sparsely for information such as the number of weights in the graph.
//Most information is contained already in TPoly, its weyl string, or the structure of the weights.
const Graph *TPoly::mGraph; 

//The output stream for printing debugging information.
std::ofstream *TPoly::mStream;

//The polynomial dictionary that is used to help minimize redundant information by storing the most
//common polynomials in terms of t^2 generated throughout the calculation.
PolyStore TPoly::mStore(100);

//Constructs a TPoly.
TPoly::TPoly(const std::vector<unsigned char> &tWeylString)
{ 
	//Allocates underlying datastructures and sets TPoly equal to 1*T0.
	int tLength = tWeylString.size();
	int tNumberOfWeights = mGraph->getUpperBoundEx(tLength).mIndex;

	mPolySize = 2*mGraph->getLongestLength();
	Poly tZero;
	Poly tOne(true);
	
	mZero = mStore.store(tZero);
	const Poly* pOne = mStore.store(tOne);

	//Note that each TPoly takes up a lot of space. For n=9, ~3.4 million pointers.
	mPolys = new std::vector<const Poly*>(tNumberOfWeights, mZero);
	mBack = new std::vector<const Poly*>(tNumberOfWeights, mZero);

	mPolys->at(0) = pOne;
	mIndex.push_back(&(mGraph->getZero())); //Tells us that the poly at index 0 is nonzero.
	mGlobalOffset = 0;

	//Begins to construct TPoly for specified weyl string.
	int tEnd = tLength;
	for(int i = 0; i < tEnd; i++)
		multiplyBy(tWeylString[i]);

	//Gets rid of the backing array used during the construction of the TPoly.
	//Note that with the dictionary, polynomials can't be simply deleted as they may be referenced elsewhere.
	for(int i = 0; i < mBack->size(); i++)
		if(!(mStore.contains(*(mBack->at(i)))))
			delete mBack->at(i);
	delete mBack;
}

//A destructor
TPoly::~TPoly()
{
	unsigned int tStartSize = mIndex.size();
	for(unsigned int i = 0; i < tStartSize; i++)
		mStore.dispose(mPolys->at(mIndex[i]->mIndex));
	delete mPolys;
}

void TPoly::multiplyBy(int aS)
{
	//Deletes previous polynomials and zeros them out.
	for(int i = 0; i < mBack->size(); i++)
		mStore.dispose(mBack->at(i));
	for(int i = 0; i < mBack->size(); i++)
		mBack->at(i) = mZero;

	//Given how large the TPoly's are we need to iterate only through non-zero elements.
	//Thus, we look at the vector of non-zero indices: mIndex.
	unsigned int tStartSize = mIndex.size();
	for(unsigned int i = 0; i < tStartSize; i++)
	{
		const Poly& tCurrent = *(mPolys->at(mIndex[i]->mIndex));
		const Weight &tNu = *mIndex[i];
		const Weight &tNuS = *(tNu.mEdges[aS]);

		//Performs multiplication rules writing to mBack and then at the end swapping mPoly and mBack.
		if(tNuS.mLength > tNu.mLength)
		{
			Poly tNuPoly(*(mBack->at(tNu.mIndex)),mPolySize); //Get any polynomial computed earlier in for loop.
			Poly tNuSPoly(*(mBack->at(tNuS.mIndex)),mPolySize); //Get any polynomial computed earlier in for loop.

			tNuPoly.add(1, 0, tCurrent); //Add the current polynomial to it.
			mStore.dispose(mBack->at(tNu.mIndex)); //Get rid of old polynomial.
			mBack->at(tNu.mIndex) = mStore.store(tNuPoly); //Store polynomial in dictionary and write it back to mBack.

			//We may need to add the tNuS into mIndex if we have never generated a polynomial with that index before.
			if(mBack->at(tNuS.mIndex)->isZero() && mPolys->at(tNuS.mIndex)->isZero())
				mIndex.push_back(&tNuS);

			//Similar to before.
			tNuSPoly.add(1, 0, tCurrent);
			mStore.dispose(mBack->at(tNuS.mIndex));
			mBack->at(tNuS.mIndex) = mStore.store(tNuSPoly);
		}
		else if(tNuS.mLength < tNu.mLength)
		{
			//See previous if statement.
			Poly tNuPoly(*(mBack->at(tNu.mIndex)),mPolySize);
			Poly tNuSPoly(*(mBack->at(tNuS.mIndex)),mPolySize);

			tNuPoly.add(1, 2, tCurrent);
			mStore.dispose(mBack->at(tNu.mIndex));
			mBack->at(tNu.mIndex) = mStore.store(tNuPoly);

			if(mBack->at(tNuS.mIndex)->isZero() && mPolys->at(tNuS.mIndex)->isZero())
				mIndex.push_back(&tNuS);
			tNuSPoly.add(1, 2, tCurrent);
			mStore.dispose(mBack->at(tNuS.mIndex));
			mBack->at(tNuS.mIndex) = mStore.store(tNuSPoly);
		}
		else
		{
			//See previous if statement.
			Poly tNuPoly(*(mBack->at(tNu.mIndex)),mPolySize);

			tNuPoly.add(1, 0, tCurrent);
			tNuPoly.add(1, 2, tCurrent);

			mStore.dispose(mBack->at(tNu.mIndex));
			mBack->at(tNu.mIndex) = mStore.store(tNuPoly);
		}
	}
	//Swap the new results with old TPoly.
	std::vector<const Poly*> *tSwap = mPolys;
	mPolys = mBack;
	mBack = tSwap;
	//Need to increase offset so that we can have polynomials in terms of positive powers of t and thus easier to store.
	mGlobalOffset++; 
}

//Prints out the weight. Probably could be moved to graph.cpp.
std::string printWeight(const unsigned char* tWeight)
{
	std::stringstream tStream;
	tStream << "( " << (int) tWeight[0];
	for(int i = 1; i < RANK - 1; i++)
		tStream << " , " << (int) tWeight[i];
	tStream << " )";
	return tStream.str();
}

//Prints out the TPoly in a somewhat easy to understand way (given that it is a polynomial of polynomials).
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
	//Gets indices in sorted order.
	sort(mIndex.begin(), mIndex.end()); 

	//Starting at nearly the end (lowest degree term) of the TPoly.
	for(int i = (int) mIndex.size() - 2; i >= 0; i--)
	{
		//Lets you know status of computation.
		if(i % 100 == 0)
			std::cout << i << std::endl; 

		//Get current polynomial.
		const Poly& tPoly = *(mPolys->at(mIndex[i]->mIndex));

		int tLength = mIndex[i]->mLength;
		//Figure out what the real zero degree is.
		int tOurZeroDegree = mGlobalOffset - tLength;
		//Get g(t).
		Poly tAdd = tPoly.greaterThan(tOurZeroDegree);
		//Get C.
		int tZeroth = tPoly.getCoefficient(tOurZeroDegree);
		//Polynomial may be zero, so eliminate any leading zero terms.
		tAdd.trim();
		if(tAdd.isZero() && tZeroth == 0)
			continue;

		//(*mStream) << "WEIGHT:" << printWeight(mGraph->getWeight(mIndex[i]).mValue) << std::endl;
		//(*mStream) << toString() << std::endl;

		TPoly tNu(mGraph->getWeylString(*(mIndex[i])));
		//Do TPoly addition.
 		addSymmetric(tOurZeroDegree, tAdd, tZeroth, tNu);

		//*mStream << toString() <<std::endl;
	}


	//(*mStream) << "Final:\n" << toString() << std::endl;
}

//Adds two TPolys.
void TPoly::addSymmetric(int aOurZeroDegree, const Poly& aPoly, int aZeroCoefficient, const TPoly& aNuTPoly)
{
	int tEnd = aNuTPoly.mIndex.size();
	//Could be that aOurZeroDegree is odd. In this case, we have to modify it so that we aren't adding together odd degrees in our t^2 polynomials.
	int tParityOffset = aOurZeroDegree % 2;
	for(int i = 0; i < tEnd; i++)
	{
		const Weight &tWeight = *(aNuTPoly.mIndex[i]);

		//Get the two polys associated with the same weight from NuTPoly and this TPoly.
		const Poly& tNuPoly = *(aNuTPoly.mPolys->at(tWeight.mIndex));
		assert(mPolys->at(tWeight.mIndex) != NULL);
		Poly tGammaPoly(*(mPolys->at(tWeight.mIndex)), mPolySize); //OK?

		//Does g*(t) = g(t) + f(t)h(t) where g(t) is tGammaPoly, f(t) is (aPoly + aZeroCoefficient + aPoly with inverted degrees), h(t) is aNuPoly. 
		int tPolyLength = aPoly.getLength();
		for(int j = 0; j < tPolyLength; j++)
		{
			tGammaPoly.add(-aPoly.getCoefficient(2*j), aOurZeroDegree + 2*(j + 1) - tParityOffset, tNuPoly);
			tGammaPoly.add(-aPoly.getCoefficient(2*j), aOurZeroDegree - 2*(j + 1) + tParityOffset, tNuPoly);
		}
		//Add zero coefficient. 
		if(tParityOffset == 0)
			tGammaPoly.add(-aZeroCoefficient, aOurZeroDegree, tNuPoly);

		//Delete old polynomial and store new result.
		mStore.dispose(mPolys->at(tWeight.mIndex));
		mPolys->at(tWeight.mIndex) = mStore.store(tGammaPoly);
	}
}

void TPoly::setGraph(const Graph& aGraph)
{
	mGraph = &aGraph;
	mStream = new std::ofstream(DEBUG_FILE);
}
