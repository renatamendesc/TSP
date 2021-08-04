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

	double cost = __DBL_MAX__;

	vector <vector <double>> changingDistance = distance;
	vector <double> rootMultiplier (dimension);

	root.setMultipliers(rootMultiplier);

	lagrangian.lagrangianDual(root, changingDistance, dimension); // Calculates root
	root.verifiesNode (dimension); // Verifies information

	vector <Node> tree;
	tree.push_back(root); // Adds on tree

	int iter = 0;
	
	while (!tree.empty()) {

		// Depth search
		Node node = tree.back();
		tree.erase(tree.end());

		if (!node.getUpperBound()) {

			for (int i = 0; i < node.getChosenEdges().size(); i++) {

				cout << root.getChosenEdges()[i].first << " - " << root.getChosenEdges()[i].second << endl;

				Node newNode;

				newNode.setProhibitedEdges(node.getProhibitedEdges());
				newNode.setMultipliers(node.getMultipliers());

				pair <int, int> edge;

				edge.first = node.getChosenEdges()[i].first;
				edge.second = node.getChosenEdges()[i].second;

				newNode.addProhibitedEdge(edge);
				newNode.prohibitEdges(changingDistance, distance);

				lagrangian.lagrangianDual(newNode, changingDistance, dimension);

				cout << "Lower Bound: " << newNode.getLowerBound() << endl;

				if (newNode.getLowerBound() < cost) {
					newNode.verifiesNode(dimension);
					tree.push_back(newNode);
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

		if (iter < 1) break;

		clock_t end = clock();
		double time = ((double) (end - start)) / CLOCKS_PER_SEC;
		cout << "Time: " << time << endl;

		if (time > 600) break;

		iter++;
	}

	cout << endl << "Cost: " << cost << endl;

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

	search(distance, dimension);

	delete data;

	return 0;
}