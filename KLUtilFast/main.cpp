#include <list>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include "weylstr.h"
#include "weight.h"
#include "globals.h"
#include <boost/unordered_set.hpp>
#include <omp.h>

//Change RANK in globals.h.

const int mMax[] = MAX_WEIGHT;
const Weight& mMaxWeight = Weight(mMax);
const Weight& mMaxRoot = ConvertToRootNotation(mMaxWeight);
const Weight mZero;

//IndexedWeight
//mWeight denotes actual weight.
//mIndex denotes order of being found (and hence its number in the edges).
struct IndexedWeight {
	IndexedWeight(const Weight& aWeight, const int aIndex = 0) : mWeight(aWeight) {
		mIndex = aIndex;
	}
	Weight mWeight;
	int mIndex;
};

//Entry
//mWeylString is the weyl string.
//mIndexedWeight is defined above.
//mEdges note what other weights can be formed from this weight.
class Entry {
public:
	Entry(const WeylStr& aWeylStr, const IndexedWeight* aWeight) : mWeylStr(aWeylStr)
	{
		mWeight = aWeight;
		for(int i = 0; i < RANK; i++)
			mEdges[i] = 0;
	}
	WeylStr mWeylStr;
	const IndexedWeight* mWeight;
	int mEdges[RANK];
};

//Equality operator. Note that we can't assume mIndex is set correctly so we just check weights.
bool operator==(IndexedWeight const& e1, IndexedWeight const& e2) {
	return (bool) (e1.mWeight == e2.mWeight);
}

//A fairly simple hash function for a Weight. Note that Get function is not zero-valued.
std::size_t hash_value(IndexedWeight const& e) {
    std::size_t seed = 0;
	for(int i = 1; i < RANK; i++) {
		seed = (e.mWeight.Get(i) + seed) * 2;
	}
    return seed;
}

//A property check to ensure weights are well-defined.
bool isDominantAndLimited(const Weight& aTest) {
	return (mZero <= aTest && aTest <= mMaxRoot);	
}

//Creates the starting entry.
void init(vector<Entry> *tN, boost::unordered_set<IndexedWeight> *tNHash )
{
	//Create weyl string.
	WeylStr tStartString(w0array);
	//Put weight based off string and index into container.
	pair<boost::unordered_set<IndexedWeight>::iterator, bool> tIter = 
		tNHash->insert(
			IndexedWeight(
				Weight(tStartString.DotOn((-2)*Weight(alpharootarray[0]))),
				1
			)
		);

	const IndexedWeight* ptr = &(*(tIter.first));
	//Pass pointer to newly created weight and weyl string to Entry constructor.
	Entry tStart(tStartString, ptr);
	for(int j = 0; j < RANK; j++)
		tStart.mEdges[j] = 1;

	//Add entry to current iteration.
	tN->push_back(tStart);
}

int main() 
{
	vector<Entry> *tNMinus = new vector<Entry>();
	vector<Entry> *tN = new vector<Entry>();
	vector<Entry> *tNPlus = new vector<Entry>();
	boost::unordered_set<IndexedWeight> *tNMinusHash = new boost::unordered_set<IndexedWeight>();
	boost::unordered_set<IndexedWeight> *tNHash = new boost::unordered_set<IndexedWeight>();
	boost::unordered_set<IndexedWeight> *tNPlusHash = new boost::unordered_set<IndexedWeight>();
	ofstream mFile(GRAPH_FILE);
	ofstream oWeights("Weights.tmp");
	ofstream oLengths("Lengths.tmp");
	ofstream oEdges("Edges.tmp");

	const Weight rho(alpharootarray[0]); //The constant rho.

	//Need to initialize computation with the zero weight.
	init(tN, tNHash);

	bool tFinish = false;
	bool tLastOne = false;
	int tCount = 1;
	while(!tFinish)
	{
		if(tLastOne)
			tFinish = true;
		#pragma omp parallel for
		for(unsigned int k = 0; k < tN->size(); k++)
		{
			//Get the current weight information.
			Entry& tEntry = tN->at(k);
			const WeylStr& tString = tEntry.mWeylStr;
			const Weight& tCurrentWeight = tEntry.mWeight->mWeight;
			const int tCurrentIndex = tEntry.mWeight->mIndex;
			const Weight& tCurrentRoot = ConvertToRootNotation(tCurrentWeight);

			for(int i = 0; i < RANK; i++)
			{
				//Calculate new weight based off the current with s_i.
				WeylStr tNew(tString);
				tNew.Append_S(i + 1);
				const Weight& tNewWeight = tNew.DotOn((-2)*rho);
				const Weight& tNewRoot = ConvertToRootNotation(tNewWeight);

				//First check that the properties hold.
				if (isDominantAndLimited(tNewRoot)) {
					//First check to see if its the same weight.
					if(tCurrentRoot == tNewRoot) {
						tEntry.mEdges[i] = tCurrentIndex; 
					} else if(tNewRoot < tCurrentRoot)	{
						//Look through the list to find where we have computed this weight already.
						boost::unordered_set<IndexedWeight>::iterator tIter = tNMinusHash->find(IndexedWeight(tNewWeight));
						if(tIter != tNMinusHash->end())	{
							tEntry.mEdges[i] = tIter->mIndex;
						} else {
							//Note that if we didn't find it we were actually looking for  ourselves.
							tEntry.mEdges[i] = tCurrentIndex; 
						}
					}
					else if ((tCurrentRoot < tNewRoot) && !tFinish)
					{
						//Look through the list to find where we have computed this weight already.
						//This code deals with adding the weight and has to be done critically.
						#pragma omp critical 
						{
							//Check to see if we have already computed this weight.
							boost::unordered_set<IndexedWeight>::iterator tIter = tNPlusHash->find(IndexedWeight(tNewWeight));
							if(tIter != tNPlusHash->end())	{
								tEntry.mEdges[i] = tIter->mIndex;
							} else {
								//If not then add it.
								tCount++;
								tEntry.mEdges[i] = tCount; //Note not off by one since edges are not zero-valued;

								//First add weight and index to container.
								pair<boost::unordered_set<IndexedWeight>::iterator, bool> tIter = 
									tNPlusHash->insert(
										IndexedWeight(
											tNewWeight,
											tCount
										)
									);

								const IndexedWeight* ptr = &(*(tIter.first));
								//Then pass pointer and weyl string to Entry constructor.
								Entry tEntry(tNew, ptr);
								tNPlus->push_back(tEntry);
								if(tNewWeight == mMaxWeight)
									tLastOne = true;
							}
						}
					}
				}
			}
		}

		for(unsigned int i = 0; i < tNMinus->size(); i++)
		{
			oWeights << tNMinus->at(i).mWeight->mWeight << '\n';
			oLengths << tNMinus->at(i).mWeylStr.GetLength() << '\n';
			oEdges << tNMinus->at(i).mEdges[0];
			for(int j = 1; j < RANK; j++)
				oEdges << ' ' << tNMinus->at(i).mEdges[j];
			oEdges << endl;
		}

		delete tNMinus;
		tNMinus = tN;
		tN = tNPlus;
		tNPlus = new vector<Entry>();
		//Go ahead and initialize to be at least as big.
		tNPlus->reserve(tN->size());

		delete tNMinusHash;
		tNMinusHash = tNHash;
		tNHash = tNPlusHash;

		//Go ahead and initialize to be at least as big.
		tNPlusHash = new boost::unordered_set<IndexedWeight>(tNHash->bucket_count());
	}

	for(unsigned int i = 0; i < tNMinus->size(); i++)
	{
		oWeights << tNMinus->at(i).mWeight->mWeight << '\n';
		oLengths << tNMinus->at(i).mWeylStr.GetLength() << '\n';
		oEdges << tNMinus->at(i).mEdges[0];
		for(int j = 1; j < RANK; j++)
			oEdges << ' ' << tNMinus->at(i).mEdges[j];
		oEdges << endl;
	}

	oWeights.close();
	oLengths.close();
	oEdges.close();

	ifstream iWeights("Weights.tmp");
	ifstream iLengths("Lengths.tmp");
	ifstream iEdges("Edges.tmp");

	std::string tLine;
	while(std::getline(iWeights,tLine))
		mFile << tLine << '\n';
	while(std::getline(iLengths,tLine))
		mFile << tLine << '\n';
	while(std::getline(iEdges,tLine))
		mFile << tLine << '\n';

	iWeights.close();
	iLengths.close();
	iEdges.close();

	remove("Weights.tmp");
	remove("Lengths.tmp");
	remove("Edges.tmp");

	mFile.close();
}
