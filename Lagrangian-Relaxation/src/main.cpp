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

void lagrangianCost (vector <pair <int, int>> &edges, int dimension) {

	vector <int> subgradient;

	// Calculates subgradient vector
	cout << "Subgradient: ";
	for(int i = 0; i < dimension; i++){
		subgradient.push_back(2 - getDegree(edges, i+1));
		cout << subgradient[i] << " ";
	}

}

int main (int argc, char** argv) {

	Data * data = new Data(argc, argv[1]);
	data->readData();

	int dimension = data->getDimension();
	double cost;

	vector <pair <int, int>> spanningTree;

	vector <vector <double>> distance (dimension, vector <double> (dimension));
	vector <vector <double>> originalDistance (dimension, vector <double> (dimension));
	vector <vector <double>> incompleteDistance (dimension-1, vector <double> (dimension-1));

	// Generates matrix
	for (int i = 0; i < dimension; i++){
		for (int j = 0; j < dimension; j++){
			if (i > 0 && j > 0) incompleteDistance[i-1][j-1] = data->getDistance(i, j);
			distance[i][j] = data->getDistance(i, j);
			originalDistance[i][j] = data->getDistance(i, j);
		}
	}

	// Generates first 1-tree
	Kruskal kruskal(incompleteDistance);
	kruskal.MST(dimension-1);
	kruskal.change1Tree(distance);

	// Gets 1-tree edges and cost
	spanningTree = kruskal.getEdges();
	cost = kruskal.getCost();

	/*
	for(int i = 0; i < spanningTree.size(); i++){
		cout << spanningTree[i].first << " - " << spanningTree[i].second << endl;
	}

	cout << "Cost: " << cost << endl;
	*/

	lagrangianCost(spanningTree, dimension);

	delete data;

	return 0;
}