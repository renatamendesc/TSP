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

    if (maxEdges > 2) {
        this->upperBound = false;
        this->setChosenEdges();
    } else {
        this->upperBound = true;
    }
    
}

void Node::prohibitEdges (vector <vector <double>> &newDistance, vector <vector <double>> &originalDistance) {

    newDistance = originalDistance;

    for (int i = 0; i < this->prohibitedEdges.size(); i++) {
        newDistance[this->prohibitedEdges[i].first][this->prohibitedEdges[i].second] = INFINITE;
        newDistance[this->prohibitedEdges[i].second][this->prohibitedEdges[i].first] = INFINITE;
        // cout <<  endl << "Proibindo: " << this->prohibitedEdges[i].first << " e " << this->prohibitedEdges[i].second << endl;
    }

}

void Node::setChosenEdges () {

    for (int i = 0; i < this->graph.size(); i++) {
        if(this->choosenVertex == this->graph[i].first || this->choosenVertex == this->graph[i].second){
            pair <int, int> prohibited;

            prohibited.first = this->graph[i].first;
            prohibited.second = this->graph[i].second;

            this->chosenEdges.push_back(prohibited);
		}
    }
}

void Node::addProhibitedEdge (pair <int, int> edge) {
    this->prohibitedEdges.push_back(edge);
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

vector <double> Node::getMultipliers () {
    return this->multipliers;
}

vector <pair <int, int>> Node::getProhibitedEdges () {
    return this->prohibitedEdges;
}

void Node::setProhibitedEdges (vector <pair <int, int>> edges) {
    this->prohibitedEdges = edges;
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

vector <pair <int, int>> Node::getChosenEdges (){
    return this->chosenEdges;
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