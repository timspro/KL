#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <omp.h>
#include "TPoly.h"

const Graph *TPoly::_graph;
std::ofstream *TPoly::_stream;
int TPoly::_longestLength;

TPoly::TPoly() {
    int numberOfWeights = _graph->getUpperBoundEx(TPoly::_longestLength).index;

    _polys = new std::vector<Poly*>(numberOfWeights, (Poly*) NULL);
    _back = new std::vector<Poly*>(numberOfWeights, (Poly*) NULL);
}

//Expects

TPoly::TPoly(const std::vector<unsigned char> &weylString) {
    int length = weylString.size();
    TPoly::_longestLength = length;
    int numberOfWeights = _graph->getUpperBoundEx(length).index;

    _polys = new std::vector<Poly*>(numberOfWeights, (Poly*) NULL);
    _back = new std::vector<Poly*>(numberOfWeights, (Poly*) NULL);

    _polys->at(0) = new Poly(2 * (_graph->getLongestLength()));
    _polys->at(0)->add(1, 0, Poly(true)); //Sets T_0 = 1;
    _nonzeroPolys.push_back(&(_graph->getZero())); //Tells us that the poly at index 0 is nonzero.
    _globalOffset = 0;

    int end = length;
    for (int i = 0; i < end; i++)
        multiplyBy(weylString[i]);

    delete _back;
}

TPoly::~TPoly() {
    for (unsigned int i = 0; i < _nonzeroPolys.size(); i++)
        delete _polys->at(_nonzeroPolys[i]->index);
    delete _polys;
}

void TPoly::reset() {
    for (unsigned int i = 0; i < _nonzeroPolys.size(); i++) {
        delete _polys->at(_nonzeroPolys[i]->index);
        _polys->at(_nonzeroPolys[i]->index) = NULL;
    }
    _nonzeroPolys.clear();
}

void TPoly::multiplyBy(const std::vector<unsigned char> &weylString) {
    int length = weylString.size();
    _polys->at(0) = new Poly(2 * (_graph->getLongestLength()));
    _polys->at(0)->add(1, 0, Poly(true)); //Sets T_0 = 1;
    _nonzeroPolys.push_back(&(_graph->getZero())); //Tells us that the poly at index 0 is nonzero.
    _globalOffset = 0;

    int end = length;
    for (int i = 0; i < end; i++)
        multiplyBy(weylString[i]);
}

void TPoly::multiplyBy(int aS) {
    //We need to queue instead of modifying it in the loop.

    unsigned int startSize = _nonzeroPolys.size();
    for (unsigned int i = 0; i < startSize; i++) {
        Poly& current = *(_polys->at(_nonzeroPolys[i]->index));
        const Weight &nu = *_nonzeroPolys[i];
        const Weight &nuS = *(nu._edges[aS]);

        if (nuS.length > nu.length) {
            if (_back->at(nu.index) == NULL)
                _back->at(nu.index) = new Poly(current);
            else
                _back->at(nu.index)->add(1, 0, current);

            if (_back->at(nuS.index) == NULL) {
                _back->at(nuS.index) = new Poly(current);
                if (_polys->at(nuS.index) == NULL)
                    _nonzeroPolys.push_back(&nuS);
            } else
                _back->at(nuS.index)->add(1, 0, current);
        } else if (nuS.length < nu.length) {
            if (_back->at(nu.index) == NULL) {
                _back->at(nu.index) = new Poly(current);
                _back->at(nu.index)->multiplyByPower(2);
            } else
                _back->at(nu.index)->add(1, 2, current);

            if (_back->at(nuS.index) == NULL) {
                _back->at(nuS.index) = new Poly(current);
                _back->at(nuS.index)->multiplyByPower(2);
                if (_polys->at(nuS.index) == NULL)
                    _nonzeroPolys.push_back(&nuS);
            } else
                _back->at(nuS.index)->add(1, 2, current);
        } else {
            if (_back->at(nu.index) == NULL)
                _back->at(nu.index) = new Poly(current);
            else
                _back->at(nu.index)->add(1, 0, current);

            _back->at(nu.index)->add(1, 2, current);
        }
    }

    std::vector<Poly*> *swap = _polys;
    _polys = _back;
    _back = swap;
    _globalOffset++;

    for (unsigned int i = 0; i < startSize; i++)
        if (_back->at(_nonzeroPolys[i]->index) != NULL) {
            delete _back->at(_nonzeroPolys[i]->index);
            _back->at(_nonzeroPolys[i]->index) = NULL;
        }
}

std::string printWeight(const unsigned char* weight) {
    std::stringstream stream;
    stream << "( " << (int) weight[0];
    for (int i = 1; i < RANK - 1; i++)
        stream << " , " << (int) weight[i];
    stream << " )";
    return stream.str();
}

std::string TPoly::toString() {
    std::stringstream tStream;
    unsigned char tFinal[] = MAX_WEIGHT;
    std::string sFinal = printWeight(tFinal);
    for (unsigned int i = 0; i < _nonzeroPolys.size(); i++)
        tStream << printWeight(_nonzeroPolys[i]->_value) << '\t' << sFinal << '\t' << (int) (_nonzeroPolys[i]->length) << '\t' << _polys->at(_nonzeroPolys[i]->index)->toString() << std::endl;

    return tStream.str();
}

struct Symmetric {
    int nu;
    TPoly* pNu;
    int ourZeroDegree;
    Poly add;
    Poly::poly_type zeroth;
};

void TPoly::fix() {
    std::sort(_nonzeroPolys.begin(), _nonzeroPolys.end());
    std::vector<int> lengthIndex(_nonzeroPolys[_nonzeroPolys.size() - 1]->length + 2);
    int lastLength = -1;
    for (int i = _nonzeroPolys.size() - 1; i >= 0; i--) {
        lastLength = _nonzeroPolys[i]->length;
        lengthIndex[lastLength] = i;
    }
    lengthIndex[lengthIndex.size() - 1] = _nonzeroPolys.size();

    int largestGap = -1;
    for (unsigned int i = 0; i < lengthIndex.size() - 1; i++) {
        if (lengthIndex[i + 1] - lengthIndex[i] > largestGap)
            largestGap = lengthIndex[i + 1] - lengthIndex[i];
    }

    std::vector<struct Symmetric> queue;
    std::vector<struct Symmetric* > all;
    std::vector<TPoly*> threadsTPolys;
    int numThreads;
    std::vector<int> queueLengths;
    int offset;
#pragma omp parallel 
    {
		//Initialization code based off the number of threads.
#pragma omp single 
        {
            numThreads = omp_get_num_threads();
            queue.resize(numThreads * largestGap);
            queueLengths.resize(numThreads);
            threadsTPolys.resize(numThreads);
            for (int i = 0; i < numThreads; i++) {
                threadsTPolys[i] = new TPoly();
            }
            all.resize(largestGap);
            for (int k = 0; k < numThreads; k++) {
                queueLengths[k] = 0;
            }
        }
        for (int i = lengthIndex.size() - 3; i >= 0; i--) {
            int id = omp_get_thread_num();
            queueLengths[id] = 0;
            //Parallel section
#pragma omp for schedule(dynamic)
            for (int j = lengthIndex[i]; j < lengthIndex[i + 1]; j++) {
                if (j % 100 == 0)
                    std::cout << j << std::endl;

                Poly& poly = *(_polys->at(_nonzeroPolys[j]->index));

                int length = _nonzeroPolys[j]->length;
                int ourZeroDegree = _globalOffset - length;
                Poly add = poly.greaterThan(ourZeroDegree);
                Poly::poly_type zeroth = poly.getCoefficient(ourZeroDegree);
                add.trim();
                if (add.isZero() && zeroth == 0)
                    continue;

				//We found an index that matched our conditions, so record its information for late.r
                struct Symmetric &sym = queue[id * largestGap + queueLengths[id]];
                sym.nu = j;
                sym.ourZeroDegree = ourZeroDegree;
                sym.zeroth = zeroth;
                sym.add = add;
                queueLengths[id]++;
            }

			//Condense the queue of computation to be done to one single continuous array.
#pragma omp single 
            {
                offset = 0;
                for (int j = 0; j < numThreads; j++) {
                    int k;
                    for (k = 0; k < queueLengths[j]; k++)
                        all[offset + k] = &(queue[j * largestGap + k]);
                    offset += k;
                }
            }

            int queueIndex = offset - 1;
            for (int j = 0; j <= queueIndex; j += numThreads) {
                int end = (j + numThreads - 1 > queueIndex ? queueIndex : j + numThreads - 1);
                //Parallel Section
				//For each found index compute FatC_Nu
#pragma omp for
                for (int k = j; k <= end; k++) {
                    threadsTPolys[id]->reset();
                    threadsTPolys[id]->multiplyBy(_graph->getWeylString(*(_nonzeroPolys[all[k]->nu])));
                    all[k]->pNu = threadsTPolys[id];
                }

                for (int k = j; k <= end; k++) {
                    //Parallel Section
					//Do the symmetric add.
                    addSymmetric(all[k]->ourZeroDegree, all[k]->add, all[k]->zeroth, *(all[k]->pNu));
                }
            }
        }
    }


    //(*mStream) << "Final:\n" << toString() << std::endl;
}

void TPoly::addSymmetric(int ourZeroDegree, const Poly& poly, Poly::poly_type zeroCoefficient, const TPoly& nuTPoly) {
    int end = nuTPoly._nonzeroPolys.size();
    int parityOffset = ourZeroDegree % 2;
#pragma omp for
    for (int i = 0; i < end; i++) {
        const Weight &weight = *(nuTPoly._nonzeroPolys[i]);

        const Poly& nuPoly = *(nuTPoly._polys->at(weight.index));
        assert(_polys->at(weight.index) != NULL);
        Poly& gammaPoly = *(_polys->at(weight.index));

        int polyLength = poly.getLength();
        for (int j = 0; j < polyLength; j++) {
            gammaPoly.add(-poly.getCoefficient(2 * j), ourZeroDegree + 2 * (j + 1) - parityOffset, nuPoly);
            gammaPoly.add(-poly.getCoefficient(2 * j), ourZeroDegree - 2 * (j + 1) + parityOffset, nuPoly);
        }
        if (parityOffset == 0) {
            gammaPoly.add(-zeroCoefficient, ourZeroDegree, nuPoly);
        }
    }
}

void TPoly::setGraph(const Graph& graph, std::ofstream& stream) {
    _graph = &graph;
    _stream = new std::ofstream(DEBUG_FILE);
}
