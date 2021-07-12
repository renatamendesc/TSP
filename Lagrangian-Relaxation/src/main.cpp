#include "data.h"
#include "kruskal.h"
#include <iostream>
#include <vector>

using namespace std;

int getDegree (vector <pair <int, int>> &edges, int vertex) {

	int degree = 0;

	for(int j = 0; j < edges.size(); j++){
		if(vertex == edges[j].first || vertex == edges[j].second){
			degree++;
		}
	}

	return degree;
}

vector <double> stepDirection (vector <pair <int, int>> &edges, int dimension) {

	vector <double> subgradient (dimension);

	// Calculates subgradient vector
	cout << "Subgradient: ";
	for(int i = 0; i < dimension; i++){
		subgradient[i] = 2 - getDegree(edges, i+1);
		cout << subgradient[i] << " ";
	}

	return subgradient;
}

vector <double> stepSize (double upperBound, double lowerBound, double epsilon, vector <double> &subgradient, vector <double> &lastSubgradient) {

	int lenght = subgradient.size();
	double denominator = 0;
	vector <double> multiplier (lenght);

	for (int i = 0; i < lenght; i++) {
		denominator += subgradient[i] * subgradient[i];
	}

	double step = (epsilon * (upperBound - lowerBound)) / denominator;

	for (int i  = 0; i < lenght; i++) {
		multiplier[i] = lastSubgradient[i] + step * subgradient[i];
	}

	return multiplier;

}

void lagrangianDual (vector <vector <double>> &distance, int dimension, Data *data) {

	vector <pair <int, int>> spanningTree;
	vector <double> subgradient, multipliers, lastSubgradient (dimension);

	// Generates first 1-tree
	Kruskal kruskal(distance, dimension);
	kruskal.change1Tree(distance);

	// Gets 1-tree edges and cost
	spanningTree = kruskal.getEdges();
	double cost = kruskal.getCost();

	// cout << cost << endl;
	double upperBound, epsilon;

	subgradient = stepDirection(spanningTree, dimension);
	multipliers = stepSize(upperBound, cost, epsilon, subgradient, lastSubgradient);

}

int main (int argc, char** argv) {

	Data *data = new Data(argc, argv[1]);
	data->readData();

	int dimension = data->getDimension();

	vector <vector <double>> distance (dimension, vector <double> (dimension));
	// vector <vector <double>> originalDistance (dimension, vector <double> (dimension));

	// Generates matrix with distances
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			distance[i][j] = data->getDistance(i, j);
			// originalDistance[i][j] = data->getDistance(i, j);
		}
	}

	lagrangianDual(distance, dimension, data);

	delete data;

	return 0;
}