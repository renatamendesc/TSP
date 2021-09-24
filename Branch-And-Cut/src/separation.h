//---------------------------------------------------------------------------

/***************************************************/
/* Functions prototypes by Prof. Anand Subramanian */
/***************************************************/

#ifndef Separation_H
#define Separation_H

#include <ilcplex/ilocplex.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <pthread.h>

#define EPSILON 0.00000001

using namespace std;

typedef struct{

    vector<int> nodes;
    int id;
    bool is_vertexA;
    
} vertex_type;

extern vector <vector <int>> MaxBack (double ** weight, int dimension);
extern vector <vector <int>> MinCut (double ** weight, int dimension);

// Max back functions
int findVertex (vector <int> &searched, int dimension);
double getCut (double ** weight, int vertex, int dimension);
void getMaxBackValues (vector <double> &maxBackValues, vector <int> &s, double ** weight, int dimension);
void updateSearched (vector <int> &s, vector <int> &searched);
int getMaximumMaxBack (vector <double> &maxBackValues, vector <int> &s, int dimension);
void updateMaxBack (vector <double> &maxBackValues, vector <int> &s, double ** weight, int maximumMaxBack, int dimension);
void updateSearched (vector <int> &s, vector <int> &searched);

// Min cut functions
double minCutPhase (vector <vector <int>> &V, vector <int> &A, double ** weight, int a, int dimension);
void mergeVertices (vector <vector<int>> &V, double ** weight, int s, int t, int dimension);

#endif

//---------------------------------------------------------------------------
