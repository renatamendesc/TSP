#ifndef NODE_H
#define NODE_H

#include "data.h"

#include <iostream>
#include <vector>
#include <climits>

using namespace std;

class Node {

    private:
        vector <pair <int, int>> chosenEdges; // Vector with illegal arcs

        vector <pair <int, int>> prohibitedEdges;
        vector <pair <int, int>> graph;

        vector <double> multipliers;

        int choosenVertex;
        double lowerBound; // Hungarian algorithm solution cost
        bool upperBound; // Indicates wether the node is an upper bound

    public:
        void verifiesNode(int dimension);
        void prohibitEdges(vector <vector <double>> &newDistance, vector <vector <double>> &originalDistance);

        void setChosenEdges();
        void setProhibitedEdges(vector <pair <int, int>> edges);

        void addProhibitedEdge(pair <int, int> edge);

        void setMultipliers(vector <double> multipliers);
        void setLowerBound(double lowerBound);
        void setGraph(vector <pair <int, int>> graph);

        vector <double> getMultipliers();
        double getLowerBound();
        bool getUpperBound();

        vector <pair <int, int>> getChosenEdges();
        vector <pair <int, int>> getProhibitedEdges();

        vector <pair <int, int>> getGraph();

        int getDegree(int vertex);

    //  void prohibitArcs(int dimension, double ** newMatrix, double ** originalMatrix);
    //  void printSolution(double time);

    //  vector <int> getChosenSubtour();
    //  vector <pair <int, int>> getProhibitedArcs();

    //  void setProhibitedArc(pair <int, int> arc);
    //  void setProhibitedArcs(vector <pair <int, int>> arcs);



};

#endif