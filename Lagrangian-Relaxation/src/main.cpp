#include "data.h"
#include "kruskal.h"
#include "dual.h"
#include "node.h"

#include <iostream>
#include <vector>
#include <queue>

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

bool subtourSearch (vector <pair <int, int>> graph, int dimension) {

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

	return cost;
}


// Function to specify priority_queue's sequence
bool operator < (Node a, Node b){
	return a.getLowerBound() > b.getLowerBound();
}

void search (vector <vector <double>> &distance, double upperBound, int dimension, int type) {

	clock_t start = clock(); // Starts time counting

	Dual lagrangian;
	Node root, solution;

	double time, cost = upperBound;

	vector <Node> tree;
	priority_queue <Node> bestBoundTree;

	vector <vector <double>> changingDistance = distance;
	vector <double> rootMultiplier (dimension);

	root.setMultipliers(rootMultiplier);

	lagrangian.lagrangianDual(root, changingDistance, upperBound, dimension); // Calculates root
	root.verifiesNode (dimension); // Verifies information

	solution = root;

	// Adds on tree
	if (type == 1) {
		bestBoundTree.push(root);
	} else {
		tree.push_back(root);
	}

	int iter = 0;
	
	while (!tree.empty() || !bestBoundTree.empty()) {

		Node node;
		
		if (type == 1) {
			node = bestBoundTree.top();
			bestBoundTree.pop();

		} else if (type == 2) {
			node = tree.front();
			tree.erase(tree.begin());

		} else if (type == 3) {
			node = tree.back();
			tree.erase(tree.end());

		}

		if (!node.getUpperBound()) {

			for (int i = 0; i < node.getChosenEdges().size(); i++) {

				Node newNode;

				newNode.setProhibitedEdges(node.getProhibitedEdges());
				newNode.setMultipliers(node.getMultipliers());

				// Illegal edge
				pair <int, int> edge;

				edge.first = node.getChosenEdges()[i].first;
				edge.second = node.getChosenEdges()[i].second;

				newNode.addProhibitedEdge(edge);
				newNode.prohibitEdges(changingDistance, distance);

				lagrangian.lagrangianDual(newNode, changingDistance, upperBound, dimension);

				if (newNode.getLowerBound() < cost) {
					newNode.verifiesNode(dimension);

					// Adds new node
					if (type == 1) {
						bestBoundTree.push(newNode);
					} else {
						tree.push_back(newNode);
					}
				}
			}

		} else {

			if (node.getLowerBound() < cost) {
				cost = node.getLowerBound();
				solution = node;

				upperBound = cost;
			}

		}

		clock_t end = clock();
		time = ((double) (end - start)) / CLOCKS_PER_SEC;

		if (time > 1000) break;

		iter++;
	}

	if (solution.getUpperBound()) {
		cout << endl << "Cost: " << cost << endl;
	} else {
		cout << endl << "Couldn't find a feasible solution" << endl;
	}

	cout << "Time: " << time << endl << endl;

}

int main (int argc, char** argv) {

	Data *data = new Data(argc, argv[1]);
	data->readData();

	int dimension = data->getDimension(), type;

	vector <vector <double>> distance (dimension, vector <double> (dimension));

	// Generates matrix with distances
	for (int i = 0; i < dimension; i++) {
		for (int j = 0; j < dimension; j++) {
			distance[i][j] = data->getDistance(i, j);
		}
	}

	cout << endl << "Select your search method: " << endl << endl;

	int option;

	cout << "[1] Best bound" << endl;
	cout << "[2] Breadth" << endl;
	cout << "[3] Depth" << endl;

	cout << endl << "Option: ";
	cin >> option;

	switch (option) {

		case 1:
			type = 1; 
			break;

		case 2:
			type = 2;
			break;

		case 3:
			type = 3;
			break;

	}

	double upperBound = primalBound(distance, dimension);

	search(distance, upperBound, dimension, type);

	delete data;

	return 0;
}