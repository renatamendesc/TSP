#include "kruskal.h"

// Creates graph
Kruskal::Kruskal(vector <vector <double>> &distance, int dimension){

	// Changes matrix with incomplete distance
	this->dist = distance;

	for (int i = 0; i < dimension-1; i++) {
		for (int j = 0; j < dimension-1; j++){
			graph.push(make_pair(-dist[i+1][j+1], make_pair(i, j)));
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
}

double Kruskal::getCost() {
	return cost;
}

void Kruskal::change1Tree(int dimension){

	ii firstEdge, secondEdge;

	// Creates edges
	firstEdge.first = 0;
	secondEdge.second = 0;

	double bestDist = __DBL_MAX__;
	
	for(int i = 0; i < edges.size(); i++){
		edges[i].first++;
		edges[i].second++;
	}

	for (int i = 0; i < dimension; i++) {

		double dist = this->dist[i][0];

		if (dist < bestDist) {
			bestDist = dist;
			firstEdge.second = i;
		}

	}

	// Adds new edge
	cost += this->dist[firstEdge.first][firstEdge.second];
	edges.push_back(firstEdge);

	this->dist[firstEdge.second][0] = __DBL_MAX__;
	bestDist = __DBL_MAX__;
	for (int i = 0; i < dimension; i++) {

		double dist = this->dist[i][0];

		if (dist < bestDist) {
			bestDist = dist;
			secondEdge.first = i;
		}

	}

	// Adds new edge
	cost += this->dist[secondEdge.first][secondEdge.second];
	edges.push_back(secondEdge);

}