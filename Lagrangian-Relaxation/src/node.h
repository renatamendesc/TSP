#ifndef NODE_H
#define NODE_H

#include "data.h"

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Node {

    private:
        vector <int> illegalEdges; // Vector with illegal arcs
        vector <pair <int, int>> graph;

        vector <double> multipliers;

        int choosenVertex;
        double lowerBound; // Hungarian algorithm solution cost
        bool upperBound; // Indicates wether the node is an upper bound

    public:
        void verifiesNode (int dimension);

        void setIllegalEdges ();
        void setMultipliers (vector <double> multipliers);
        void setLowerBound(double lowerBound);
        void setGraph(vector <pair <int, int>> graph);

        double getLowerBound ();
        bool getUpperBound ();
        vector <int> getIllegalEdges ();
        vector <pair <int, int>> getGraph ();

        int getDegree (int vertex);

    //  void prohibitArcs(int dimension, double ** newMatrix, double ** originalMatrix);
    //  void printSolution(double time);

    //  vector <int> getChosenSubtour();
    //  vector <pair <int, int>> getProhibitedArcs();

    //  void setProhibitedArc(pair <int, int> arc);
    //  void setProhibitedArcs(vector <pair <int, int>> arcs);



};

#endif