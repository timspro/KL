#include <vector>
#include <cassert>
#include "PolyStore.h"
#include "Poly.h"

PolyStore::PolyStore(int aSize) : mStore(aSize, (Poly*) NULL) { }

//Stores the polynomial in the dictionary. Note that if the dictionary runs out of room, it will just dynamically allocate it.
const Poly* PolyStore::store(Poly& aPoly)
{
	aPoly.trim();
	int tIndex = hash_value(aPoly) % mStore.size();
	if(mStore[tIndex] == NULL)
	{
		mStore[tIndex] = new Poly(aPoly);
		return mStore[tIndex];
	}
	if(*(mStore[tIndex]) == aPoly)
		return mStore[tIndex];
	return new Poly(aPoly);
}

//Checks to see if the polynomial is contained in the dictionary.
bool PolyStore::contains(const Poly& aPoly)
{
	int iIndex = hash_value(aPoly) % mStore.size();
	if(mStore[iIndex] != NULL && *(mStore[iIndex]) == aPoly)
		return true;
	return false;
}

//Deletes the polynomial if it is not in the dictionary (which means that when it was inserted, the dictionary did
//not have a spot for it).
void PolyStore::dispose(const Poly* aPoly)
{
	if(!contains(*aPoly))
		delete aPoly;
}
