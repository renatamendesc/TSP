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

	int start, next, depot, graphSize, counter = 0, index = 0;

	graphSize = graph.size();

	start = graph[0].first;
	next = graph[0].second;

	depot = start;

	for (int i = 0; i < graph.size(); i++) {
		cout << graph[i].first << " - " << graph[i].second << endl;
	}

	cout << endl;

	while (true) {

		// Searches for path
		for (int i = 0; i < graph.size(); i++) {

			if (i != index) {

				if (graph[i].first == next) {
					start = graph[i].first;
					next = graph[i].second;

					graph.erase(graph.begin() + index);
					index = i-1;

					counter++;
					break;

				} else if (graph[i].second == next) {
					start = graph[i].second;
					next = graph[i].first;

					graph.erase(graph.begin() + index);
					index = i-1;

					counter++;
					break;

				}
			}

			// If all edges were searched
			if (i == graph.size()-1) {

				if (next == depot) { // It's a cycle
					return true;

				} else {

					// If all the edges were found
					if (counter == graphSize-1) {
						return false;

					} else { 
						graph.erase(graph.begin() + index); // Starts searching another path

						start = graph[0].first;
						next = graph[0].second;

						index = 0;
						depot = start;

						graphSize = graph.size();

					}
				}
			}
		}

		// cout << "oi" << endl;
	}
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

		// cout << edge.first << " - " << edge.second << endl;

		if ((getDegree(edgesTest, edge.second.first) <= 2) && (getDegree(edgesTest, edge.second.second) <= 2)) { // Validates node degree

			if (edges.size() != dimension - 1) {

				if (subtourSearch(edgesTest, dimension)) continue; // Validates subtours
				
			}

			// Adds edge if not illegal
			edges = edgesTest;
			cost += -edge.first;

		}

		// cout << "bom dia" << endl;
	}

	return cost;

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

	epsilon = 1;
	upperBound = primalBound(distance, dimension);

	subgradient = stepDirection(spanningTree, dimension);
	multipliers = stepSize(upperBound, cost, epsilon, subgradient, lastSubgradient);

	for(int i = 0; i < dimension; i++){
		cout << subgradient[i] << " ";
	}

	cout << endl;

	for(int i = 0; i < dimension; i++){
		cout << multipliers[i] << " ";
	}

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