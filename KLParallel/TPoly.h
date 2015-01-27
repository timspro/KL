#ifndef TPOLY_H
#define TPOLY_H

#include <vector>
#include <string>
#include <fstream>
#include "Poly.h"
#include "Graph.h"

struct MyT
{
	const Weight *mIndex;
	Poly mPoly;
};

class TPoly
{
public:
	TPoly();
	TPoly(const std::vector<unsigned char> &weylString);
	~TPoly();
	void multiplyBy(int s);
	void multiplyBy(const std::vector<unsigned char> &weylString);
	void addSymmetric(int zeroDegree, const Poly& posPoly, Poly::poly_type zeroCoefficient, const TPoly& nuTPoly);
	void fix();
	void reset();
	void set(const std::vector<unsigned char> &weylString);
	std::string toString();

	static void setGraph(const Graph &aGraph, std::ofstream &aStream);
private:
	std::vector<Poly*> *_polys;
	std::vector<Poly*> *_back;
	std::vector<const Weight*> _nonzeroPolys;
	int _globalOffset;

	static int _longestLength;
	static const Graph *_graph;
	static std::ofstream *_stream;
};

#endif
