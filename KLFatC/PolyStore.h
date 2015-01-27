#include <string>
#include <vector>
#include "Poly.h"

#ifndef POLYSTORE_H
#define POLYSTORE_H

class PolyStore
{
public:
	PolyStore(int aSize);
	const Poly* store(Poly& aPoly);
	bool contains(const Poly& aPoly);
	void dispose(const Poly* aPoly);
private:
	std::vector<const Poly*> mStore;
};

#endif
