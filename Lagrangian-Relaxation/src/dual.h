#ifndef DUAL_H
#define DUAL_H

#include "kruskal.h"
#include "node.h"

#include <iostream>
#include <vector>

class Dual {

    public:
        void lagrangianDual (Node &node, std::vector <std::vector <double>> &originalDistance, double upperBound, int dimension);

        std::vector <double> stepDirection (std::vector <std::pair <int, int>> &edges, int dimension);
        std::vector <double> stepSize (double upperBound, double lowerBound, double epsilon, std::vector <double> &lastSubgradient, std::vector <double> &subgradient, int dimension);

        //bool subtourSearch (std::vector <std::pair <int, int>> graph, int dimension);
        // double primalBound (std::vector <std::vector <double>> &originalDistance, std::vector <std::vector <double>> &distance, int dimension);

        void updateLagrangianCost (std::vector <std::vector <double>> &distance, std::vector <double> &multipliers, int dimension);
        bool validateSubgradient (std::vector <double> &subgradient);

        int getDegree (std::vector <std::pair <int, int>> &edges, int vertex);
        
};

#endif