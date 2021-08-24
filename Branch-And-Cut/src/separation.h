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

extern vector <vector <int>> MaxBack (double ** x, int dimension);
extern vector <vector <int>> MinCut (double ** x, int dimension);

double getCut (int vertex, double ** weight, int dimension);

void getMaxBackValues (vector <int> &maxBackValues, vector <int> s, double ** weight, int dimension);

int getMaximumMaxBack (vector <int> &maxBackValues, int dimension);

#endif

//---------------------------------------------------------------------------
