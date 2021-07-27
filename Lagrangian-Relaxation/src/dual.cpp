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
		subgradient[i] = 2 - getDegree(edges, i);
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

bool Dual::subtourSearch (vector <pair <int, int>> graph, int dimension) {

	pair <int, int> depot;
	bool changeDepot = false;

	while (!graph.empty()) {

		if (!changeDepot) {

			depot.first = graph[0].first;
			depot.second = graph[0].second;

			graph.erase(graph.begin());

		}

		changeDepot = false;

		for (int i = 0; i < graph.size(); i++) {

			if ((graph[i].first == depot.first && graph[i].second == depot.second) || (graph[i].second == depot.first && graph[i].first == depot.second)) {
				
				return true;

			} else {

				if (depot.first == graph[i].first) {

					depot.first = graph[i].second;
					changeDepot = true;	

				} else if (depot.first == graph[i].second) {

					depot.first = graph[i].first;
					changeDepot = true;	

				} else if (depot.second == graph[i].first) {

					depot.second = graph[i].second;
					changeDepot = true;	

				} else if (depot.second == graph[i].second) {

					depot.second = graph[i].first;
					changeDepot = true;	

				}

				if (changeDepot) {

					graph.erase(graph.begin() + i);
					break;

				}
			}
		}
	}

	return false;
}

double Dual::primalBound (vector <vector <double>> &distance, int dimension) {

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

	return cost;

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

Node Dual::lagrangianDual (Node &node, vector <vector <double>> &originalDistance, int dimension) {

	vector <pair <int, int>> spanningTree, solution;
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

		// cout << "Lower Bound: " << lowerBound << endl;

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

		upperBound = primalBound(lagragianDistance, dimension);
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

		lagragianDistance = originalDistance;
		updateLagrangianCost(lagragianDistance, multipliers, dimension);

		// cout << endl << "Upper Bound: " << upperBound << endl;
		// break;

	}

    node.setLowerBound(bestLowerBound);
    node.setMultipliers(bestMultipliers);
    node.setGraph(solution);

    return node;

}