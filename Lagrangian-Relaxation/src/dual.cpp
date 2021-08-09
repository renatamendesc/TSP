#include "dual.h"

using namespace std;

int Dual::getDegree (vector <pair <int, int>> &edges, int vertex) {

	int degree = 0;

	for(int j = 0; j < edges.size(); j++){
		if(vertex == edges[j].first || vertex == edges[j].second){
			degree++;
		}
	}

	return degree;
}

vector <double> Dual::stepDirection (vector <pair <int, int>> &edges, int dimension) {

	vector <double> subgradient (dimension);

	// Calculates subgradient vector
	for(int i = 0; i < dimension; i++){
		subgradient[i] = 2 - this->getDegree(edges, i);
	}

	return subgradient;
}

vector <double> Dual::stepSize (double upperBound, double lowerBound, double epsilon, vector <double> &lastSubgradient, vector <double> &subgradient, int dimension) {
 
	vector <double> multiplier (dimension);
	double denominator = 0;

	for (int i = 0; i < dimension; i++) {
		denominator += subgradient[i] * subgradient[i];
	}

	double step = (epsilon * (upperBound - lowerBound)) / denominator;

	for (int i  = 0; i < dimension; i++) {
		multiplier[i] = lastSubgradient[i] + step * subgradient[i];
	}

	return multiplier;

}

void Dual::updateLagrangianCost (vector <vector <double>> &distance, vector <double> &multipliers, int dimension) {

	for (int i = 0; i < dimension; i++) {
		for (int j = i+1; j < dimension; j++) {
			distance[i][j] -= (multipliers[i] + multipliers[j]);
			distance[j][i] -= (multipliers[i] + multipliers[j]);
		}
	}
}

bool Dual::validateSubgradient (vector <double> &subgradient) {

	for (int i  = 0; i < subgradient.size(); i++) {
		if (subgradient[i] != 0) return true;
	}

	return false;

}

void Dual::lagrangianDual (Node &node, vector <vector <double>> &originalDistance, double upperBound, int dimension) {

	vector <pair <int, int>> spanningTree, solution;
	vector <vector <double>> lagragianDistance;
	vector <double> subgradient, multipliers, bestMultipliers, lastMultipliers (dimension);
	
	double epsilon, lowerBound, bestLowerBound = __DBL_MIN__;

	if (dimension < 70) epsilon = 1;
	else epsilon = 2;

	multipliers = node.getMultipliers();

	int iter = 0;

	while (true) {

		lagragianDistance = originalDistance;
		updateLagrangianCost(lagragianDistance, multipliers, dimension);

		// Generates first 1-tree
		Kruskal kruskal(lagragianDistance, dimension);
		kruskal.change1Tree(lagragianDistance);

		// Gets 1-tree edges and cost
		spanningTree = kruskal.getEdges();
		lowerBound = kruskal.getCost();

		if (lowerBound > bestLowerBound) {
			bestLowerBound = lowerBound;
			bestMultipliers = multipliers;
			solution = spanningTree;

			iter = 0;

		} else {
			iter++;

			if (iter > 30) {

				epsilon = epsilon / 2;
				if (epsilon < 0.0005) break;

			}
		}

		// upperBound = primalBound(originalDistance, lagragianDistance, dimension);
		if (upperBound - lowerBound <= epsilon) break;

		subgradient = stepDirection(spanningTree, dimension);

		if (!validateSubgradient(subgradient)) {
			bestLowerBound = lowerBound;
			bestMultipliers = multipliers;
			solution = spanningTree;

			break;
		}

		lastMultipliers = multipliers;
		multipliers = stepSize(upperBound, lowerBound, epsilon, lastMultipliers, subgradient, dimension);

	}

    node.setLowerBound(bestLowerBound);
    node.setMultipliers(bestMultipliers);
    node.setGraph(solution);

}