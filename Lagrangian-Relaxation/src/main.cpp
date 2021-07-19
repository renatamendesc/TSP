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

bool subtourSearch (vector <pair <int, int>> graph, int dimension) {

	pair <int, int> depot;

	while (!graph.empty()) {

		depot.first = graph[0].first;
		depot.second = graph[0].second;

		// Searches graph
		for (int i = 1; i < graph.size(); i++) {

			// Verifies if its a cycle
			if ((graph[i].first == depot.first && graph[i].second == depot.second) || (graph[i].second == depot.first && graph[i].first == depot.second)) {
				return true; // Its a cycle

			} else {

				if (graph[i].first == depot.first) {
					depot.first = graph[i].second;
					graph.erase(graph.begin() + i);

				} else if (graph[i].second == depot.second) {
					depot.second = graph[i].first;
					graph.erase(graph.begin() + i);

				} else if (graph[i].second == depot.first) {
					depot.first = graph[i].first;
					graph.erase(graph.begin() + i);

				} else if (graph[i].first == depot.second) {
					depot.second = graph[i].second;
					graph.erase(graph.begin() + i);

				}
			}
		}

		graph.erase(graph.begin());

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

	// cout << "Cost: " << cost << endl;
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

void lagrangianDual (vector <vector <double>> &originalDistance, int dimension, Data *data) {

	vector <pair <int, int>> spanningTree;
	vector <double> subgradient, multipliers, lastSubgradient (dimension);

	vector <vector <double>> lagragianDistance = originalDistance;

	// Generates first 1-tree
	Kruskal kruskal(lagragianDistance, dimension);
	kruskal.change1Tree(lagragianDistance);

	// Gets 1-tree edges and cost
	spanningTree = kruskal.getEdges();
	double cost = kruskal.getCost();

	// cout << cost << endl;

	double epsilon = 1;
	double upperBound = primalBound(lagragianDistance, dimension);

	subgradient = stepDirection(spanningTree, dimension);
	multipliers = stepSize(upperBound, cost, epsilon, subgradient, lastSubgradient);

	for(int i = 0; i < dimension; i++){
		cout << subgradient[i] << " ";
	}

	cout << endl;

	for(int i = 0; i < dimension; i++){
		cout << multipliers[i] << " ";
	}

	updateLagrangianCost(lagragianDistance, multipliers, dimension);

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