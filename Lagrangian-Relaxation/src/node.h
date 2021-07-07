#ifndef NODE_H
#define NODE_H

#include "data.h"
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Node {

    private:
        vector <pair <int, int>> illegalArcs; // Vector with illegal arcs
        vector <vector <int>> subtours; // Vector with subtours

        double lowerBound; // Hungarian algorithm solution cost
        int chosen; // Chosen subtour with illegal arcs
        bool upperBound; // Indicates wether the node is an upper bound

    public:
        void prohibitArcs(int dimension, double ** newMatrix, double ** originalMatrix);
        void printSolution(double time);

        bool getUpperBound();
        double getLowerBound();
        vector <int> getChosenSubtour();
        vector <pair <int, int>> getProhibitedArcs();

        void setProhibitedArc(pair <int, int> arc);
        void setProhibitedArcs(vector <pair <int, int>> arcs);
        void setLowerBound(double lowerBound);

};


#endif