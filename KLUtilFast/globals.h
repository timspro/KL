#ifndef GLOBALTVAR_H
#define GLOBALTVAR_H

//RANK should be the only thing that needs to be changed.
#define RANK 7

#define STRINGIFY(X) #X
#define TOSTRING(X) STRINGIFY(X)
#define GRAPH_FILE "graph-" TOSTRING(RANK) "," TOSTRING(RANK) ".film" 

//Slightly unsure about max weight for n=4.

#if RANK==4
#define MAX_WEIGHT {2,2,2}
#include "globals-4.h"
#elif RANK==5
#define MAX_WEIGHT {3,3,3,3}
#include "globals-5.h"
#elif RANK==6
#define MAX_WEIGHT {4,4,4,4,4}
#include "globals-6.h"
#elif RANK==7
#define MAX_WEIGHT {5,5,5,5,5,5}
#include "globals-7.h"
#elif RANK==8
#define MAX_WEIGHT {5,6,6,6,6,6,5}
#include "globals-8.h"
#elif RANK==9
#define MAX_WEIGHT {6,7,7,7,7,7,7,6}
#include "globals-9.h"
#elif RANK==10
#define MAX_WEIGHT {7,8,8,8,8,8,8,8,7}
#include "globals-10.h"
#endif

#define PRIME RANK

#endif
