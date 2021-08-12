#ifndef KRUSKAL_H
#define KRUSKAL_H

#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
#include <utility>

using namespace std;

typedef pair <int, int> ii;
typedef vector <vector <double>> vvi;
typedef vector <ii> vii;

class Kruskal{
	
	public:
		Kruskal(vector <vector <double>> &distance, int dimension);

		void MST(int nodes);
		void change1Tree(int dimension);

		double getCost();
		vii getEdges();

	private:
		priority_queue <pair <double, ii>> graph;
		vector <int> pset;
		vvi dist;
		vii edges;

		double cost = 0;

		void initDisjoint(int n);
		int findSet(int i);
		void unionSet(int i, int j);
		bool isSameSet(int i, int j);
};

#endif