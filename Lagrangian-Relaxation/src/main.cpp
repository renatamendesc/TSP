#include "data.h"
#include "kruskal.h"
#include "dual.h"
#include "node.h"

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Function to specify priority_queue's sequence
bool operator < (Node a, Node b){
	return a.getLowerBound() > b.getLowerBound();
}

void search (vector <vector <double>> &distance, int dimension, int type) {

	clock_t start = clock(); // Starts time counting

	Dual lagrangian;
	Node root, solution;

	double time, cost = INFINITE;

	vector <Node> tree;
	priority_queue <Node> bestBoundTree;

	vector <vector <double>> changingDistance = distance;
	vector <double> rootMultiplier (dimension);

	root.setMultipliers(rootMultiplier);

	lagrangian.lagrangianDual(root, changingDistance, dimension); // Calculates root
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

				// cout << "Novo proibido: " << node.getChosenEdges()[i].first << " - " << node.getChosenEdges()[i].second << endl;

				// cout << "Antigos proibidos: " << endl;
				// for (int i  = 0; i < node.getProhibitedEdges().size(); i++) {
				// 	cout << node.getProhibitedEdges()[i].first << " - " << node.getProhibitedEdges()[i].second << endl;
				// }

				newNode.setProhibitedEdges(node.getProhibitedEdges());
				newNode.setMultipliers(node.getMultipliers());

				// Illegal edge
				pair <int, int> edge;

				edge.first = node.getChosenEdges()[i].first;
				edge.second = node.getChosenEdges()[i].second;

				newNode.addProhibitedEdge(edge);
				newNode.prohibitEdges(changingDistance, distance);

				lagrangian.lagrangianDual(newNode, changingDistance, dimension);

				cout << "Lower Bound: " << newNode.getLowerBound() << endl;

				if (newNode.getLowerBound() < cost) {
					newNode.verifiesNode(dimension);

					// Adds new node
					if (type == 1) {
						bestBoundTree.push(newNode);
					} else {
						tree.push_back(newNode);
					}

					cout << "VALIDO" << endl;

				} else {
		
					cout << "INVALIDO" << endl;

				}
				
			}

			cout << endl << "LOWER BOUND" << endl;

		} else {

			if (node.getLowerBound() < cost) {
				cost = node.getLowerBound();
				solution = node;
			}

			cout << endl << "UPPER BOUND" << endl;
		}

		if (iter > 0) break;

		clock_t end = clock();
		time = ((double) (end - start)) / CLOCKS_PER_SEC;
		cout << "Time: " << time << endl;

		if (time > 600) break;

		iter++;
	}

	cout << endl << "Cost: " << cost << endl;
	cout << "Time: " << time << endl;

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

	search(distance, dimension, type);

	delete data;

	return 0;
}