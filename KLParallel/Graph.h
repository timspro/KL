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
	const Weight& getUpperBoundEx(const Weight& gamma) const; //Returns the beginning ending weight with the length one more than aGamma.
	const Weight& getUpperBoundEx(int length) const;
	const Weight& getLesserBoundIn(const Weight& gamma) const; //Returns the beginning weight (inclusive) with the same length as aGamma.
	const Weight& getLesserBoundIn(int length) const;
	int getNumberOfWeights() const; //Returns the number of valid weights in the graph.
	int getLongestLength() const; //Gets the longest length.
	std::vector<unsigned char> getWeylString(const Weight& gamma) const; //Returns a weyl string for the weight.

	//const Weight& getWeight(int aIndex) const;

	static bool isNotDominant(const Weight& gamma); //Checks to see if weight is not dominant.
	static int getSThatReduces(const Weight& gamma); //Gets gamma x such that x*aS = aGamma.
private:
	int getNumberOfWeights(std::ifstream& graph) const;
	int getLongestLength(std::ifstream& graph);
	void readFIL(std::ifstream& graph);
	//void readFIL2(std::ifstream& aGraph);

	static const Weight getNotDominant();

	std::vector<Weight*> _indexBounds; //Record of largest possible index for a given length.
	std::vector<Weight> _weights;
	int _size;
	static const Weight notDominant;
	static const bool _numbers = true;
};

#endif
