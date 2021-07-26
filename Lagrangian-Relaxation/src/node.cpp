#include "node.h"

using namespace std;

void Node::verifiesNode (int dimension) {

    int maxEdges = 0;

    for (int i = 0; i < dimension; i++) {

        int degree = this->getDegree(i);

        if (degree > maxEdges) {
            this->choosenVertex = i;
            maxEdges = degree;
        }

    }

    cout << "Choosen vertex: " << this->choosenVertex << endl;

    if (maxEdges > 2) {
        this->upperBound = false;
        this->setIllegalEdges();
    } else {
        this->upperBound = true;
    }
    
}

void Node::setIllegalEdges () {

    for (int i = 0; i < this->graph.size(); i++) {
        if(this->choosenVertex == this->graph[i].first || this->choosenVertex == this->graph[i].second){
			this->illegalEdges.push_back(i);
		}
    }
}

int Node::getDegree (int vertex) {

	int degree = 0;

	for(int i = 0; i < this->graph.size(); i++){
		if(vertex == this->graph[i].first || vertex == this->graph[i].second){
			degree++;
		}
	}

	return degree;
}

void Node::setLowerBound (double lowerBound) {
    this->lowerBound = lowerBound;
}

void Node::setMultipliers (vector <double> multipliers) {
    this->multipliers = multipliers;
}

void Node::setGraph(vector <pair <int, int>> graph) {
    this->graph = graph;
}

vector <int> Node::getIllegalEdges (){
    return this->illegalEdges;
}

double Node::getLowerBound () {
    return this->lowerBound;
}

bool Node::getUpperBound () {
    return this->upperBound;
}

vector <pair <int, int>> Node::getGraph () {
    return this->graph;
}