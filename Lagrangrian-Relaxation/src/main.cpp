#include "data.h"
#include "kruskal.h"
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char** argv) {

	Data * data = new Data(argc, argv[1]);
	data->readData();

	int dimension = data->getDimension();

	// vector <int> solution;
	vector <pair <int, int>> spanningTree;
	vector <vector <double>> distance (dimension, vector <double> (dimension));
	vector <vector <double>> incompleteDistance (dimension-1, vector <double> (dimension-1));

	for (int i = 0; i < dimension; i++){
		for (int j = 0; j < dimension; j++){
			if(i > 0 && j > 0) incompleteDistance[i-1][j-1] = data->getDistance(i, j);
			distance[i][j] = data->getDistance(i, j);
		}
	}

	Kruskal kruskal(incompleteDistance);
	kruskal.MST(dimension-1);

	spanningTree = kruskal.get1Tree(distance);

	for(int i = 0; i < spanningTree.size(); i++){
		cout << spanningTree[i].first << " - " << spanningTree[i].second << endl;
	}

	delete data;

	return 0;
}