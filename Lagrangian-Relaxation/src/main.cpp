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
	// cout << "Subgradient: ";
	for(int i = 0; i < dimension; i++){
		subgradient[i] = 2 - getDegree(edges, i+1);
		// cout << subgradient[i] << " ";
	}

	return subgradient;
}

vector <double> stepSize (double upperBound, double lowerBound, double epsilon, vector <double> &lastSubgradient, vector <double> &subgradient, int dimension) {
 
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

bool subtourSearch (vector <pair <int, int>> graph, int dimension) {

	pair <int, int> depot;
	bool previewIsEdge = true, isEdge = true;

	while (!graph.empty()) {
		
		if (isEdge) {

			depot.first = graph[0].first;
			depot.second = graph[0].second;

		}

		// Searches graph
		for (int i = 0; i < graph.size(); i++){

			if (i != 0 || !isEdge) {

				// Verifies if its a cycle
				if ((graph[i].first == depot.first && graph[i].second == depot.second) || (graph[i].second == depot.first && graph[i].first == depot.second)) {
					return true; // Its a cycle

				} else {

					if (graph[i].first == depot.first) {
						depot.first = graph[i].second;
						graph.erase(graph.begin() + i);

						if (isEdge) {
							previewIsEdge = true;

						} else {
							previewIsEdge = false;

						}

						isEdge = false;
						break;

					} else if (graph[i].second == depot.second) {
						depot.second = graph[i].first;
						graph.erase(graph.begin() + i);

						if (isEdge) {
							previewIsEdge = true;

						} else {
							previewIsEdge = false;

						}

						isEdge = false;
						break;

					} else if (graph[i].second == depot.first) {
						depot.first = graph[i].first;
						graph.erase(graph.begin() + i);

						if (isEdge) {
							previewIsEdge = true;

						} else {
							previewIsEdge = false;

						}

						isEdge = false;
						break;

					} else if (graph[i].first == depot.second) {
						depot.second = graph[i].second;
						graph.erase(graph.begin() + i);

						if (isEdge) {
							previewIsEdge = true;

						} else {
							previewIsEdge = false;

						}

						isEdge = false;
						break;
					}
				}
			}

			if (i == graph.size()-1) isEdge == true;

		}

		if (previewIsEdge) graph.erase(graph.begin());

	}

	return false;
}

double primalBound (vector <vector <double>> &distance, int dimension) {

	priority_queue <pair <double, pair <int, int>>> graph;
	double cost = 0;

	// Creates graph
	for (int i = 0; i < dimension; ++i) {
		for (int j = 0; j < dimension; ++j) {
			graph.push(make_pair(-distance[i][j], make_pair(i, j)));
		}	
	}

	vector <pair <int, int>> edges, edgesTest;

	while (edges.size() < dimension) {

		pair <double, pair <int, int>> edge = graph.top();
		graph.pop();

		edgesTest = edges;
		edgesTest.push_back(edge.second);

		if ((getDegree(edgesTest, edge.second.first) <= 2) && (getDegree(edgesTest, edge.second.second) <= 2)) { // Validates node degree

			if (edges.size() != dimension - 1) {

				if (subtourSearch(edgesTest, dimension)) continue; // Validates subtours
				
			}

			// Adds edge if not illegal
			edges = edgesTest;
			cost += -edge.first;

		}

	}

	for (int i = 0; i < edges.size(); i++ ) {
		cout << edges[i].first << " - " << edges[i].second << endl;
	}

	return cost;

}

void updateLagrangianCost (vector <vector <double>> &distance, vector <double> &multipliers, int dimension) {

	for (int i = 0; i < dimension; i++) {
		for (int j = i+1; j < dimension; j++) {
			distance[i][j] -= (multipliers[i] + multipliers[j]);
			distance[j][i] -= (multipliers[i] + multipliers[j]);	
		}
	}

}

bool validateSubgradient (vector <int> &subgradient) {

	for (int i  = 0; i < subgradient.size(); i++) {
		if (subgradient[i] != 0) return true;
	}

	return false;

}

void lagrangianDual (vector <vector <double>> &originalDistance, int dimension) {

	vector <pair <int, int>> spanningTree, currentSolution;
	vector <vector <double>> lagragianDistance = originalDistance;
	vector <double> subgradient, multipliers, bestMultipliers, lastMultipliers (dimension);
	
	double epsilon, upperBound, lowerBound, bestLowerBound = __DBL_MIN__;

	if (dimension < 70) epsilon = 1;
	else epsilon = 2;

	int iter = 0;

	while (true) {

		// Generates first 1-tree
		Kruskal kruskal(lagragianDistance, dimension);
		kruskal.change1Tree(lagragianDistance);

		// Gets 1-tree edges and cost
		spanningTree = kruskal.getEdges();
		lowerBound = kruskal.getCost();

		if (lowerBound > bestLowerBound) {
			bestLowerBound = lowerBound;
			bestMultipliers = multipliers;
			currentSolution = spanningTree;

			iter = 0;

		} else {
			iter++;

			if (iter > 30) {

				epsilon = epsilon / 2;
				if (epsilon < 0.0005) break;

			}
		}

		upperBound = primalBound(lagragianDistance, dimension);
		if (upperBound - lowerBound <= epsilon) break;

		subgradient = stepDirection(spanningTree, dimension);

		if (!validateSubgradient) {
			bestLowerBound = lowerBound;
			bestMultipliers = multipliers;
			currentSolution = spanningTree;

			break;
		}

		lastMultipliers = multipliers;
		multipliers = stepSize(upperBound, lowerBound, epsilon, lastMultipliers, subgradient, dimension);

		/*
		cout << endl;

		for(int i = 0; i < dimension; i++){
			cout << multipliers[i] << " ";
		}
		*/

		lagragianDistance = originalDistance;
		updateLagrangianCost(lagragianDistance, multipliers, dimension);

		cout << endl << "Upper Bound: " << upperBound << endl;
		cout << "Lower Bound: " << bestLowerBound << endl;

	}

	cout << endl;

}

int main (int argc, char** argv) {

	Data *data = new Data(argc, argv[1]);
	data->readData();

	int dimension = data->getDimension();

	vector <vector <double>> distance (dimension, vector <double> (dimension));

	// Generates matrix with distances
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			distance[i][j] = data->getDistance(i, j);
		}
	}

	lagrangianDual(distance, dimension);

	delete data;

	return 0;
}