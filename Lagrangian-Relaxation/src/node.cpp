#include "node.h"

using namespace std;

void Node::verifiesNode (int dimension) {

    if (!this->upperBound) {

        int maxEdges = INT_MAX;

        for (int i = 0; i < dimension; i++) {

            if (this->subgradient[i] < maxEdges) {
                this->chosenVertex = i;
                maxEdges = this->subgradient[i];
            }

        }

        this->setChosenEdges();
    }
    
}

void Node::prohibitEdges (vector <vector <double>> &newDistance, vector <vector <double>> &originalDistance) {

    newDistance = originalDistance;

    for (int i = 0; i < this->prohibitedEdges.size(); i++) {
        newDistance[this->prohibitedEdges[i].first][this->prohibitedEdges[i].second] = INFINITE;
        newDistance[this->prohibitedEdges[i].second][this->prohibitedEdges[i].first] = INFINITE;
    }

}

void Node::setChosenEdges () {

    for (int i = 0; i < this->graph.size(); i++) {

        if(this->chosenVertex == this->graph[i].first || this->chosenVertex == this->graph[i].second) {

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

vector <double> Node::getMultipliers () {
    return this->multipliers;
}

vector <pair <int, int>> Node::getProhibitedEdges () {
    return this->prohibitedEdges;
}

void Node::setUpperBound (bool validate) {
    this->upperBound = validate;
}

void Node::setProhibitedEdges (vector <pair <int, int>> edges) {
    this->prohibitedEdges = edges;
}

void Node::setLowerBound (double lowerBound) {
    this->lowerBound = lowerBound;
}

void Node::setSubgradient (vector <double> subgradient) {
    this->subgradient = subgradient;
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