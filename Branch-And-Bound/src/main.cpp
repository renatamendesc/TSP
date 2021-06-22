#include "data.h"
#include "hungarian.h"
#include "node.h"
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// Function to specify priority_queue's sequence
bool operator < (Node a, Node b){
	return a.getLowerBound() > b.getLowerBound();
}

void search(string type, double ** originalMatrix, int dimension){

	clock_t start = clock(); // Starts time counting

	Node root, solution;
	hungarian_problem_t p;

	vector <Node> tree;
	priority_queue <Node> bestBoundTree;

	hungarian_init(&p, originalMatrix, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST);
	root.setLowerBound(hungarian_solve(&p));
	root.calculateSolution(&p, dimension);
	hungarian_free(&p);

	int index;
	double time, cost = __DBL_MAX__;
	double ** newMatrix = new double *[dimension];

	// New matrix is assigned with original matrix
	for(int i = 0; i < dimension; i++){
		newMatrix[i] = new double [dimension];

		for(int j = 0; j < dimension; j++){
			newMatrix[i][j] = originalMatrix[i][j];
		}
	}

	solution = root;

	if(type == "bestBound"){
		bestBoundTree.push(root);
	} else {
		tree.push_back(root);
	}

	while(!tree.empty() || !bestBoundTree.empty()){

		Node node;

		if(type == "bestBound"){
			node = bestBoundTree.top();
			bestBoundTree.pop();
			
		}else if(type == "breadth"){
			node = tree.front();
			tree.erase(tree.begin());

		}else if(type == "depth"){
			node = tree.back();
			tree.erase(tree.end());

		}

		if(!node.getUpperBound()){

			// Covers illegal arcs
			for(int j = 0; j < node.getChoosenSubtour().size()-1; j++){

				Node newNode;
				newNode.setProhibitedArcs(node.getProhibitedArcs()); // New node inherits illegal arcs

				// Defines new illegal arc
				pair <int, int> arc;

				arc.first = node.getChoosenSubtour()[j];
				arc.second = node.getChoosenSubtour()[j+1];

				// Adds new illegal arc
				newNode.setProhibitedArc(arc);
				newNode.prohibitArcs(dimension, newMatrix, originalMatrix);

				hungarian_problem_t p;
				hungarian_init(&p, newMatrix, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST);
				newNode.setLowerBound(hungarian_solve(&p));

				// Verifies if there's an upper bound smaller than the lower bound
				if(newNode.getLowerBound() < cost){
					newNode.calculateSolution(&p, dimension);

					// Adds new node
					if(type == "bestBound"){
						bestBoundTree.push(newNode);
					}else{
						tree.push_back(newNode); 
					}	
				}

				hungarian_free(&p);
			}

		} else {

			// Verifies best upper bound
			if(node.getLowerBound() < cost){
				cost = node.getLowerBound();
				solution = node;
			}

		}

		clock_t fim = clock();
		time = ((double) (fim - start)) / CLOCKS_PER_SEC;
		if(time > 600) break;

	}

	for(int i = 0; i < dimension; i++) delete[] newMatrix[i];
	delete[] newMatrix;

	solution.printSolution(time);

}

int main(int argc, char** argv){

	Data * data = new Data(argc, argv[1]);
	data->readData();

	double ** cost = new double*[data->getDimension()];
	for(int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}

	double ** originalMatrix = cost;
	int selection, dimension = data->getDimension();

	string type;

	cout << "\n------------------------------------------------\n\n";
	cout << "Select a type of search:\n\n";
	cout << "[1] Best bound\n";
	cout << "[2] Breadth\n";
	cout << "[3] Depth\n";
	cout << "\n------------------------------------------------\n";

	cin >> selection;

	switch(selection){

		case 1:
			type = "bestBound";
			break;

		case 2:
			type = "breadth";
			break;

		case 3:
			type = "depth";
			break;

	}

	search(type, originalMatrix, dimension);

	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}