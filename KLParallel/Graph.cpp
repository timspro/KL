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

const Weight Graph::notDominant = Graph::getNotDominant();

//Gets the number of weights in the graph so that the vector can be resized.
int Graph::getNumberOfWeights(std::ifstream& graph) const
{
	int i = 0;
	std::string line;
	while(true)
	{
		std::getline(graph,line);
		if(line[0] != '(')
			break;
		i++;
	}

	graph.seekg(0);

	return i;
}

//Read's in graph file.
void Graph::readFIL(std::ifstream& graph)
{
	_size = getNumberOfWeights(graph);
	_weights.resize(_size + 1);

	//Fill weights and indices.
	for(unsigned int i = 0; i < _size; i++)
	{
		_weights[i].index = i;
		std::string input;
		graph >> input;
		
		for(int j = 0; j < RANK - 1; j++)
		{
			graph >> input;
			_weights[i]._value[j] = (char) atoi(input.c_str());
			graph >> input;
		}
	}
	//Note there is a dummy weight at the end to make it easier to iterate through
	//the backing array.
	_weights[_size].index = _size;
	for(int i = 0; i < RANK; i++)
		_weights[_size]._edges[i] = NULL;

	std::cout << "Filled in weights." << std::endl;

	//Fill index bounds array and length for gamma array.
	//First find zero length.
	std::string line;
	graph >> line;
	int lastLength;
	if(_numbers)
		lastLength = atoi(line.c_str());
	else
		lastLength = line.length()/2;
	int zeroLength = lastLength;

	//Now read in lengths. We keep track of bounds in the mIndexBounds array.
	_indexBounds.push_back(&(_weights[0]));
	_weights[0].length = 0;
	for(unsigned int i = 1; i < _size; i++)
	{
		graph >> line;
		int length = line.length()/2;
		if(_numbers)
			length = atoi(line.c_str());
		else
			length = line.length()/2;

		if(lastLength != length)
			_indexBounds.push_back(&(_weights[i]));
		lastLength = length;
		_weights[i].length = length - zeroLength;
	}
	//Fill out dummy weight.
	_weights[_size].length = _indexBounds.size();
	_indexBounds.push_back(&(_weights[_size]));

	std::cout << "Filled in lengths." << std::endl;

	//Fill edges.
	for(unsigned int i = 0; i < _size; i++)
		for(int j = 0; j < RANK; j++)
		{
			int t;
			graph >> t;
			if(t == 0)
				_weights[i]._edges[j] = &notDominant;
			else
			{
				t--;
				_weights[i]._edges[j] = &_weights[t];
			}
		} 

	std::cout << "Filled in edges." << std::endl;

	graph.close();
}

Graph::Graph() //Creates graph from graph file (which is set in Globals.h).
{
	std::ifstream graph(GRAPH_FILE);
	std::cout << "Trying to read from " << GRAPH_FILE << std::endl;
	assert(graph);
	readFIL(graph);
}


const Weight& Graph::getZero() const
{
	return _weights[0];
}

const Weight& Graph::getMaximum() const
{
	return _weights[_size - 1];
}

const Weight& Graph::getUpperBoundEx(const Weight& gamma) const //Returns the largest possible index of an equivalent gamma to aGamma.
{
	return *(_indexBounds[gamma.length + 1]);
}

const Weight& Graph::getUpperBoundEx(int length) const
{
	return *(_indexBounds[length + 1]);
}

const Weight& Graph::getLesserBoundIn(const Weight& gamma) const //Returns the largest possible index of a gamma that is one less than aGamma.
{
	return *(_indexBounds[gamma.length]);
}

const Weight& Graph::getLesserBoundIn(int length) const
{
	return *(_indexBounds[length]);
}

int Graph::getNumberOfWeights() const //Returns highest index in graph.
{
	return _size - 1;
}

int Graph::getLongestLength() const
{
	return _weights[_size - 1].length;
}

bool Graph::isNotDominant(const Weight& weight)
{
	return weight.index == notDominant.index;
}

const Weight Graph::getNotDominant()
{
	Weight notDominant;
	notDominant.index = -1;
	for(int i = 0; i < RANK; i++)
		notDominant._edges[i] = NULL;
	notDominant.length = std::numeric_limits<unsigned char>::max();
	return notDominant;
}

int Graph::getSThatReduces(const Weight& gamma)
{
	for(int i = 0; i < RANK; i++)
		if(gamma._edges[i]->length < gamma.length)
			return i;
	assert(false);
	return -1;
}

std::vector<unsigned char> Graph::getWeylString(const Weight& gamma) const
{
	const Weight *start = &gamma;
	std::vector<unsigned char> weylString(start->length);
	int count = weylString.size() - 1;
	while(start->index != 0)
	{
		int s = Graph::getSThatReduces(*start);
		weylString[count] = s;
		start = start->_edges[s];
		count--;
	}

	return weylString;
}
