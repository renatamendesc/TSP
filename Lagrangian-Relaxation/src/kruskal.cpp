#include "kruskal.h"

// Creates graph
Kruskal::Kruskal(vvi dist){

	for(int i = 0; i < dist.size(); ++i){
		for(int j = 0; j < dist[i].size(); ++j){
			graph.push(make_pair(-dist[i][j], make_pair(i, j)));
		}	
	}

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

void Kruskal::change1Tree(vvi dist){

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

	cost += dist[0][edges[0].second-1] + dist[edges[0].first-1][0];

	// Adds edges
	edges.push_back(firstEdge);
	edges.push_back(secondEdge);

}