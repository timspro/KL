#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include "Graph.h"
#include "Globals.h"
#include "TPoly.h"

//A convenience function for checking to see if a weight equals another weight when they are
//in different formats.
bool operator==(const Weight &tFirst,const char tWeight[RANK])
{
	for(int i = 0; i < RANK - 1; i++)
		if(!(tFirst.mValue[i] == tWeight[i]))
			return false;
	return true;
}

int main()
{
	std::ofstream tStream(OUTPUT_FILE);
	const Graph tGraph;

	TPoly::setGraph(tGraph);

	std::cout << "Making weyl string." << std::endl;

	//Find maximal weight we want.
	char tMaximalWeight[RANK] = MAX_WEIGHT;
	const Weight* pWeight = &(tGraph.getMaximum());
	const Weight* const pZero = &(tGraph.getZero());
	for( ; pWeight != pZero; pWeight--)
		if(*pWeight == tMaximalWeight)
			break;
	if(pWeight == pZero)
		assert(false);

	//Create maximal weyl string.
	std::vector<unsigned char> tWeylString = tGraph.getWeylString(*pWeight);
	
	std::cout << "Making Fat C Gamma." << std::endl;
	TPoly tFatCGamma(tWeylString);
	//tStream << tFatCGamma.toString() << std::endl << std::endl;

	std::cout << "Beginning adding." << std::endl;
	tFatCGamma.fix();
	tStream << tFatCGamma.toString() << std::endl;

	tGraph.outputUseCount();
}
