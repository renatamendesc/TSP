#include "kruskal.h"

// Creates graph
Kruskal::Kruskal(vector <vector <double>> &distance, int dimension){

	// Changes matrix with incomplete distance
	vector <vector <double>> dist (dimension-1, vector <double> (dimension-1));

	for (int i = 1; i < dimension; i++){
		for (int j = 1; j < dimension; j++){
			dist[i-1][j-1] = distance[i][j];
		}
	}

	for(int i = 0; i < dimension-1; ++i){
		for(int j = 0; j < dimension-1; ++j){
			graph.push(make_pair(-dist[i][j], make_pair(i, j)));
		}	
	}

	MST(dimension-1);

}

void Kruskal::initDisjoint(int n){
	pset.resize(n);
	for (int i = 0; i < n; ++i){
		pset[i] = i;
	}
}

int Kruskal::findSet(int i){
	return (pset[i] == i) ? i : (pset[i] = findSet(pset[i]));
}

// Adds on set
void Kruskal::unionSet(int i, int j){
	pset[findSet(i)] = findSet(j);
}

// Verifies if nodes are on same set
bool Kruskal::isSameSet(int i, int j){
	return (findSet(i) == findSet(j))? true:false;
}

vii Kruskal::getEdges(){
	return edges;
}

void Kruskal::MST(int nodes){
	initDisjoint(nodes);

	while(!graph.empty()){
		pair <double, ii> p = graph.top();
		graph.pop();

		if(!isSameSet(p.second.first, p.second.second)){
			edges.push_back(make_pair(p.second.first, p.second.second)); // Creates edge
			cost += (-p.first); // Calculates cost
			unionSet(p.second.first, p.second.second);
		}
	}

	// cout << cost << endl;
}

double Kruskal::getCost() {
	return cost;
}

void Kruskal::change1Tree(vvi originalDist){

	ii firstEdge, secondEdge;

	for(int i = 0; i < edges.size(); i++){
		edges[i].first = edges[i].first + 2;
		edges[i].second = edges[i].second + 2;
	}

	// Creates new edges
	firstEdge.first = 1;
	firstEdge.second = edges[0].second;

	secondEdge.first = edges[0].first;
	secondEdge.second = 1;

	cost += originalDist[0][edges[0].second-1] + originalDist[edges[0].first-1][0];

	// Adds edges
	edges.push_back(firstEdge);
	edges.push_back(secondEdge);

}