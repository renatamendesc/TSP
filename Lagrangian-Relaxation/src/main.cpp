#include "data.h"
#include "kruskal.h"
#include "dual.h"
#include "node.h"

#include <iostream>
#include <vector>

using namespace std;

void search (vector <vector <double>> &distance, int dimension) {

	clock_t start = clock(); // Starts time counting

	Node root;
	Dual lagrangian;

	root = lagrangian.lagrangianDual(root, distance, dimension);
	root.verifiesNode (dimension);

	vector <Node> tree;
	tree.push_back(root);

	for (int i = 0; i < root.getGraph().size(); i++) {
		cout << root.getGraph()[i].first << " - " << root.getGraph()[i].second << endl;
	}

	cout << endl << "Illegal edges: ";

	for (int i = 0; i < root.getIllegalEdges().size(); i++) {
		cout << root.getIllegalEdges()[i] << " ";
	}

	cout << endl << "Lower Bound: " << root.getLowerBound() << endl;
	

	while (!tree.empty()) {

		// Depth search
		Node node = tree.back();
		tree.erase(tree.end());

		if (!node.getUpperBound()) {

			cout << "LOWER BOUND" << endl;

		} else {

			cout << "UPPER BOUND" << endl;

		}

		break;

	}
	
	clock_t fim = clock();
	double time = ((double) (fim - start)) / CLOCKS_PER_SEC;

	cout << "Time: " << time << endl;

	// if (time > 600) break;

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