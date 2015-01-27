#include <list>
#include <vector>
#include <set>
#include <fstream>
#include <string>
#include "weylstr.h"
#include "weight.h"
#include "globals.h"

//Change RANK in globals.h.

const int mMax[] = MAX_WEIGHT;
const Weight& mMaxWeight = Weight(mMax);
const Weight& mMaxRoot = ConvertToRootNotation(mMaxWeight);
const Weight mZero;

class Entry
{
public:
	Entry(const WeylStr aWeylStr, const Weight aWeight, int aIndex) : mWeylStr(aWeylStr), mWeight(aWeight) 
	{
		mIndex = aIndex;
		for(int i = 0; i < RANK; i++)
			mEdges[i] = 0;
	}
	WeylStr mWeylStr;
	Weight mWeight;
	int mEdges[RANK];
	int mIndex;
};

bool isDominantAndLimited(const Weight& aTest)
{
	return (mZero <= aTest && aTest <= mMaxRoot);	
}

Entry* init()
{
	WeylStr tStartString(w0array);
	Entry* tStart = new Entry(tStartString,tStartString.DotOn((-2)*Weight(alpharootarray[0])),1);
	for(int j = 0; j < RANK; j++)
		tStart->mEdges[j] = 1;
	return tStart;
}

int main() 
{
	vector<Entry*> *tNMinus = new vector<Entry*>();
	vector<Entry*> *tN = new vector<Entry*>();
	vector<Entry*> *tNPlus = new vector<Entry*>();
	ofstream mFile(GRAPH_FILE);
	ofstream oWeights("Weights.tmp");
	ofstream oLengths("Lengths.tmp");
	ofstream oEdges("Edges.tmp");

	const Weight rho(alpharootarray[0]); //The constant rho.

	//Need to initialize computation with the zero weight.
	tN->push_back(init());

	bool tFinish = false;
	bool tLastOne = false;
	int tCount = 1;
	while(!tFinish)
	{
		if(tLastOne)
			tFinish = true;
		for(unsigned int k = 0; k < tN->size(); k++)
		{
			//Get the current weight information.
			const WeylStr& tString = tN->at(k)->mWeylStr;
			const Weight& tCurrentWeight = tN->at(k)->mWeight;
			const Weight& tCurrentRoot = ConvertToRootNotation(tCurrentWeight);

			for(int i = 0; i < RANK; i++)
			{
				//Calculate new weight based off the current with s_i.
				WeylStr tNew(tString);
				tNew.Append_S(i+1);
				const Weight& tNewWeight = tNew.DotOn((-2)*rho);
				const Weight& tNewRoot = ConvertToRootNotation(tNewWeight);

				if (isDominantAndLimited(tNewRoot)) //Check that the properties hold.
				{
					if(tCurrentRoot == tNewRoot)
						tN->at(k)->mEdges[i] = tN->at(k)->mIndex; 
					else if(tNewRoot < tCurrentRoot)
					{
						//Look through the list to find where we have computed this weight already.
						for(unsigned int j = 0; j < tNMinus->size(); j++)
							if(tNMinus->at(j)->mWeight == tNewWeight)
							{
								tN->at(k)->mEdges[i] = tNMinus->at(j)->mIndex;
								break;
							}
						if(tN->at(k)->mEdges[i] == 0) //Note that if we didn't find it we were actually looking for  ourselves.
							tN->at(k)->mEdges[i] = tN->at(k)->mIndex; 
					}
					else if ((tCurrentRoot < tNewRoot) && !tFinish)
					{
						//Look through the list to find where we have computed this weight already.
						bool tFound = false;
						for(unsigned int j = 0; j < tNPlus->size(); j++)
							if(tNPlus->at(j)->mWeight == tNewWeight)
							{
								tN->at(k)->mEdges[i] = tNPlus->at(j)->mIndex;
								tFound = true;
								break;
							}
						if(!tFound) //Note that if we didn't find it then we can add it
						{
							tCount++;
							tN->at(k)->mEdges[i] = tCount; //Note not off by one since edges are not zero-valued;
							Entry* tEntry = new Entry(tNew, tNewWeight, tCount);
							tNPlus->push_back(tEntry);
							if(tNewWeight == mMaxWeight)
								tLastOne = true;
						}
					}
				}
			}
		}

		for(unsigned int i = 0; i < tNMinus->size(); i++)
		{
			oWeights << tNMinus->at(i)->mWeight << '\n';
			oLengths << tNMinus->at(i)->mWeylStr.GetLength() << '\n';
			oEdges << tNMinus->at(i)->mEdges[0];
			for(int j = 1; j < RANK; j++)
				oEdges << ' ' << tNMinus->at(i)->mEdges[j];
			oEdges << endl;
		}

		delete tNMinus;
		tNMinus = tN;
		tN = tNPlus;
		tNPlus = new vector<Entry*>();
	}

	for(unsigned int i = 0; i < tNMinus->size(); i++)
	{
		oWeights << tNMinus->at(i)->mWeight << '\n';
		oLengths << tNMinus->at(i)->mWeylStr.GetLength() << '\n';
		oEdges << tNMinus->at(i)->mEdges[0];
		for(int j = 1; j < RANK; j++)
			oEdges << ' ' << tNMinus->at(i)->mEdges[j];
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
