#include "data.h"
#include "kruskal.h"
#include "dual.h"
#include "node.h"

#include <iostream>
#include <vector>

using namespace std;

void search (vector <vector <double>> &distance, int dimension) {

	clock_t start = clock(); // Starts time counting

	Dual lagrangian;
	Node root, solution;
	vector <vector <double>> changingDistance;

	double cost = __DBL_MAX__;

	root = lagrangian.lagrangianDual(root, distance, dimension); // Calculates root
	root.verifiesNode (dimension); // Verifies information

	vector <Node> tree;
	tree.push_back(root); // Adds on tree

	for (int i = 0; i < root.getChosenEdges().size(); i++) {
		cout << root.getChosenEdges()[i].first << " - " << root.getChosenEdges()[i].second << endl;
	}
	
	while (!tree.empty()) {

		// Depth search
		Node node = tree.back();
		tree.erase(tree.end());

		if (!node.getUpperBound()) {

			for (int i = 0; i < node.getChosenEdges().size(); i++) {

				Node newNode;

				newNode.setProhibitedEdges(node.getProhibitedEdges());

				pair <int, int> edge;

				edge.first = node.getChosenEdges()[i].first;
				edge.second = node.getChosenEdges()[i].second;

				newNode.addProhibitedEdge(edge);
				newNode.prohibitEdges(changingDistance, distance, dimension);

				newNode = lagrangian.lagrangianDual(newNode, changingDistance, dimension);

				if (newNode.getLowerBound() < cost) {
					newNode.verifiesNode(dimension);
					tree.push_back(newNode);
				}
				
			}

			// cout << "LOWER BOUND" << endl;

		} else {

			if (node.getLowerBound() < cost) {
				cost = node.getLowerBound();
				solution = node;
			}

			// cout << "UPPER BOUND" << endl;
		}

		break;

		clock_t end = clock();
		double time = ((double) (end - start)) / CLOCKS_PER_SEC;
		if (time > 600) break;

	}

	cout << "Time: " << time << endl;

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
			// cout << "Distance [" << i << "][" << j << "]: " << distance[i][j] << endl;
		}
	}

	search(distance, dimension);

	delete data;

	return 0;
}