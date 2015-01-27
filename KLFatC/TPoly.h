#ifndef TPOLY_H
#define TPOLY_H

#include <vector>
#include <string>
#include <fstream>
#include "Poly.h"
#include "Graph.h"
#include "PolyStore.h"

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

	static void setGraph(const Graph &aGraph);
private:
	//The current set of polynomials in terms of t^2.
	std::vector<const Poly*> *mPolys;
	//Another array of polynomials that is used during the construction of the TPoly to write new values too.
	std::vector<const Poly*> *mBack;
	//Contains the indices of mPolys that are nonzero.
	std::vector<const Weight*> mIndex;
	int mGlobalOffset;
	int mPolySize;

	const Poly* mZero;

	static std::ofstream *mStream;
	static PolyStore mStore;
	static const Graph *mGraph;
};

#endif
