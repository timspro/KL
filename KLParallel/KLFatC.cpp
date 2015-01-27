#include <vector>
#include <iostream>
#include <fstream>
#include <cassert>
#include "Graph.h"
#include "Globals.h"
#include "TPoly.h"
#include <omp.h>

bool operator==(const Weight &first,const char weight[RANK])
{
	for(int i = 0; i < RANK - 1; i++)
		if(!(first._value[i] == weight[i]))
			return false;
	return true;
}

int main()
{
	#pragma omp parallel
	{
		printf("Hello from %d!\n", omp_get_thread_num());
	}


	std::ofstream stream(OUTPUT_FILE);
	const Graph graph;

	TPoly::setGraph(graph, stream);

	std::cout << "Making weyl string." << std::endl;

	//Find maximal weight we want.
	char maximalWeight[RANK] = MAX_WEIGHT;
	const Weight* weight = &(graph.getMaximum());
	const Weight* const zero = &(graph.getZero());
	for( ; weight != zero; weight--)
		if(*weight == maximalWeight)
			break;
	if(weight == zero)
		assert(false);

	//Create maximal weyl string.
	std::vector<unsigned char> weylString = graph.getWeylString(*weight);
	
	std::cout << "Making Fat C Gamma." << std::endl;
	TPoly fatCGamma(weylString);

	std::cout << "Beginning adding." << std::endl;
	fatCGamma.fix();
	stream << fatCGamma.toString() << std::endl;
}
