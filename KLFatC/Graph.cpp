#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <cassert>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include "Globals.h"
#include "Graph.h"

const Weight Graph::mNotDominant = Graph::getNotDominant();

//Gets the number of weights in the graph so that the vector can be resized.
int Graph::getNumberOfWeights(std::ifstream& aGraph) const
{
	int i = 0;
	std::string tLine;
	while(true)
	{
		std::getline(aGraph,tLine);
		if(tLine[0] != '(')
			break;
		i++;
	}

	aGraph.seekg(0);

	return i;
}

//Read's in graph file.
void Graph::readFIL(std::ifstream& tGraph)
{
	mSize = getNumberOfWeights(tGraph);
	mWeights.resize(mSize + 1);
	mUseCount.resize(mSize + 1);
	for(int i = 0; i < mUseCount.size(); i++)
		mUseCount[i] = 0;

	//Fill weights and indices.
	for(unsigned int i = 0; i < mSize; i++)
	{
		mWeights[i].mIndex = i;
		std::string tInput;
		tGraph >> tInput;
		
		for(int j = 0; j < RANK - 1; j++)
		{
			tGraph >> tInput;
			mWeights[i].mValue[j] = (char) atoi(tInput.c_str());
			tGraph >> tInput;
		}
	}
	//Note there is a dummy weight at the end to make it easier to iterate through
	//the backing array.
	mWeights[mSize].mIndex = mSize;
	for(int i = 0; i < RANK; i++)
		mWeights[mSize].mEdges[i] = NULL;

	std::cout << "Filled in weights." << std::endl;

	//Fill index bounds array and length for gamma array.
	//First find zero length.
	std::string tLine;
	tGraph >> tLine;
	int tLastLength;
	if(mNumbers)
		tLastLength = atoi(tLine.c_str());
	else
		tLastLength = tLine.length()/2;
	int tZeroLength = tLastLength;

	//Now read in lengths. We keep track of bounds in the mIndexBounds array.
	mIndexBounds.push_back(&(mWeights[0]));
	mWeights[0].mLength = 0;
	for(unsigned int i = 1; i < mSize; i++)
	{
		tGraph >> tLine;
		int tLength = tLine.length()/2;
		if(mNumbers)
			tLength = atoi(tLine.c_str());
		else
			tLength = tLine.length()/2;

		if(tLastLength != tLength)
			mIndexBounds.push_back(&(mWeights[i]));
		tLastLength = tLength;
		mWeights[i].mLength = tLength - tZeroLength;
	}
	//Fill out dummy weight.
	mWeights[mSize].mLength = mIndexBounds.size();
	mIndexBounds.push_back(&(mWeights[mSize]));

	std::cout << "Filled in lengths." << std::endl;

	//Fill edges.
	for(unsigned int i = 0; i < mSize; i++)
		for(int j = 0; j < RANK; j++)
		{
			int t;
			tGraph >> t;
			if(t == 0)
				mWeights[i].mEdges[j] = &mNotDominant;
			else
			{
				t--;
				mWeights[i].mEdges[j] = &mWeights[t];
			}
		} 

	std::cout << "Filled in edges." << std::endl;

	tGraph.close();
}

Graph::Graph() //Creates graph from graph file (which is set in Globals.h).
{
	std::ifstream tGraph(GRAPH_FILE);
	std::cout << "Trying to read from " << GRAPH_FILE << std::endl;
	assert(tGraph);
	readFIL(tGraph);
}


const Weight& Graph::getZero() const
{
	return mWeights[0];
}

const Weight& Graph::getMaximum() const
{
	return mWeights[mSize - 1];
}

const Weight& Graph::getUpperBoundEx(const Weight& aGamma) const //Returns the largest possible index of an equivalent gamma to aGamma.
{
	return *(mIndexBounds[aGamma.mLength + 1]);
}

const Weight& Graph::getUpperBoundEx(int aLength) const
{
	return *(mIndexBounds[aLength + 1]);
}

const Weight& Graph::getLesserBoundIn(const Weight& aGamma) const //Returns the largest possible index of a gamma that is one less than aGamma.
{
	return *(mIndexBounds[aGamma.mLength]);
}

const Weight& Graph::getLesserBoundIn(int aLength) const
{
	return *(mIndexBounds[aLength]);
}

int Graph::getNumberOfWeights() const //Returns highest index in graph.
{
	return mSize - 1;
}

int Graph::getLongestLength() const
{
	return mWeights[mSize - 1].mLength;
}

//Checks to see if the weight is in the graph.
bool Graph::isNotDominant(const Weight& aWeight)
{
	return aWeight.mIndex == mNotDominant.mIndex;
}

//Used for creating a non-dominant weight statically upon initialization.
const Weight Graph::getNotDominant()
{
	Weight tNotDominant;
	tNotDominant.mIndex = -1;
	for(int i = 0; i < RANK; i++)
		tNotDominant.mEdges[i] = NULL;
	tNotDominant.mLength = std::numeric_limits<unsigned char>::max();
	return tNotDominant;
}

//Find an edge that reduces the length of the weight.
int Graph::getSThatReduces(const Weight& aGamma)
{
	for(int i = 0; i < RANK; i++)
		if(aGamma.mEdges[i]->mLength < aGamma.mLength)
			return i;
	assert(false);
	return -1;
}

//Create the weyl string for the weight by stringing together the various edges that reduce the length
//of aGamma to the length of the zero weight.
std::vector<unsigned char> Graph::getWeylString(const Weight& aGamma) const
{
	const Weight *tStart = &aGamma;
	std::vector<unsigned char> tWeylString(tStart->mLength);
	int tCount = tWeylString.size() - 1;
	while(tStart->mIndex != 0)
	{
		mUseCount[tStart->mIndex]++; //Modified
		int s = Graph::getSThatReduces(*tStart);
		tWeylString[tCount] = s;
		tStart = tStart->mEdges[s];
		tCount--;
	}

	return tWeylString;
}

//An old output function.
void Graph::outputUseCount() const
{
	std::ofstream tOut(LOG_FILE);

	for(int i = 0; i < mUseCount.size(); i++)
		tOut << i << " " << mUseCount[i] << std::endl;

	tOut.close();
}
