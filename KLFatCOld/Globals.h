#ifndef GLOBALS_H
#define GLOBALS_H

//Note RANK should be the only thing that needs to be changed.
//FIL denotes what version of fil is needed (either 1, which is "" or 2, which is "2") 
#define RANK 7

#define STRINGIFY(X) #X
#define TOSTRING(X) STRINGIFY(X)
#define GRAPH_FILE "graph-" TOSTRING(RANK) "," TOSTRING(RANK) ".film" 
#define OUTPUT_FILE "output-" TOSTRING(RANK) "," TOSTRING(RANK) ".kl" 
#define DEBUG_FILE "debug-" TOSTRING(RANK) "," TOSTRING(RANK) ".txt" 

#if RANK==4
#define MAX_WEIGHT {2,2,2}
#define INVERSE {{3,2,1},{2,4,2},{1,2,3}}
#elif RANK==5
#define MAX_WEIGHT {3,3,3,3}
#define INVERSE {{4,3,2,1},{3,6,4,2},{2,4,6,3},{1,2,3,4}}
#elif RANK==6
#define MAX_WEIGHT {4,4,4,4,4}
#define INVERSE {{5,4,3,2,1},{4,8,6,4,2},{3,6,9,6,3},{2,4,6,8,4},{1,2,3,4,5}}
#elif RANK==7
#define MAX_WEIGHT {5,5,5,5,5,5}
#define INVERSE {{6,5,4,3,2,1},{5,10,8,6,4,2},{4,8,12,9,6,3},{3,6,9,12,8,4},{2,4,6,8,10,5},{1,2,3,4,5,6}}
#elif RANK==8
#define MAX_WEIGHT {5,6,6,6,6,6,5}
#define INVERSE {{7,6,5,4,3,2,1},{6,12,10,8,6,4,2},{5,10,15,12,9,6,3},{4,8,12,16,12,8,4},{3,6,9,12,15,10,5},{2,4,6,8,10,12,6},{1,2,3,4,5,6,7}}
#elif RANK==9
#define MAX_WEIGHT {6,7,7,7,7,7,7,6}
#define INVERSE {{8,7,6,5,4,3,2,1},{7,14,12,10,8,6,4,2},{6,12,18,15,12,9,6,3},{5,10,15,20,16,12,8,4},{4,8,12,16,20,15,10,5},{3,6,9,12,15,18,12,6},{2,4,6,8,10,12,14,7},{1,2,3,4,5,6,7,8}}
#endif

struct Weight
{
	int mIndex;
	const Weight *mEdges[RANK];
	unsigned char mLength;
	unsigned char mValue[RANK - 1];
};


#endif
