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
	TPoly(const std::vector<unsigned char> &tWeylString);
	~TPoly();
	void multiplyBy(int aS);
	void addSymmetric(int aOurZeroDegree, const Poly& aPoly, int aZeroCoefficient, const TPoly& aTPoly);
	void fix();
	void set(const std::vector<unsigned char> &tWeylString);
	std::string toString();

	static void setGraph(const Graph &aGraph, std::ofstream &aStream);
private:
	std::vector<Poly*> *mPolys;
	std::vector<Poly*> *mBack;
	std::vector<const Weight*> mIndex;
	int mGlobalOffset;

	static const Graph *mGraph;
	static std::ofstream *mStream;
};

#endif
