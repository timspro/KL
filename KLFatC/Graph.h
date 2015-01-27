#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include "Globals.h"

//The graph has an underlying array of weights.

class Graph
{
public:
	Graph(); //Creates graph from graph file (which is set in Globals.h).
	const Weight& getZero() const;
	const Weight& getMaximum() const;
	const Weight& getUpperBoundEx(const Weight& aGamma) const; //Returns the beginning ending weight with the length one more than aGamma.
	const Weight& getUpperBoundEx(int aLength) const;
	const Weight& getLesserBoundIn(const Weight& aGamma) const; //Returns the beginning weight (inclusive) with the same length as aGamma.
	const Weight& getLesserBoundIn(int aLength) const;
	int getNumberOfWeights() const; //Returns the number of valid weights in the graph.
	int getLongestLength() const; //Gets the longest length.
	std::vector<unsigned char> getWeylString(const Weight& aGamma) const; //Returns a weyl string for the weight.

	//const Weight& getWeight(int aIndex) const;
	void outputUseCount() const;

	static bool isNotDominant(const Weight& aGamma); //Checks to see if weight is not dominant.
	static int getSThatReduces(const Weight& aGamma); //Gets gamma x such that x*aS = aGamma.
private:
	int getNumberOfWeights(std::ifstream& aGraph) const;
	int getLongestLength(std::ifstream& aGraph);
	void readFIL(std::ifstream& aGraph);
	//void readFIL2(std::ifstream& aGraph);

	static const Weight getNotDominant();

	std::vector<Weight*> mIndexBounds; //Record of largest possible index for a given length.
	std::vector<Weight> mWeights;
	int mSize;
	static const Weight mNotDominant;
	static const bool mNumbers = true;

	mutable std::vector<int> mUseCount;
};

#endif
