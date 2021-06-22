#include "node.h"

void Node::prohibitArcs(int dimension, double ** matrizModificada, double ** matrizReal){

	// New matrix is assigned with original matrix
	for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
			matrizModificada[i][j] = matrizReal[i][j];
        }
    }

	// Covers illegal arcs
	for(int i = 0; i < this->illegalArcs.size(); i++){
		// Adds illegal arcs constraints 
		matrizModificada[this->illegalArcs[i].first-1][this->illegalArcs[i].second-1] = INFINITE;
	}

}

void Node::calculateSolution(hungarian_problem_t *p, int dimension){
	vector <int> candidatesList;
	vector <int> subtour;

	// Creates candidates list
	for(int i = 0; i < dimension; i++){
		candidatesList.push_back(i+1);
	}

	// Creates substours solution
	while(!candidatesList.empty()){
		int index, start;

		index = candidatesList[0]-1;
		start = index;

		subtour.push_back(start + 1);

		while(true){
			// Deletes node from candidates list
			for(int j = 0; j < candidatesList.size(); j++){
				if(candidatesList[j] == index+1){
					candidatesList.erase(candidatesList.begin() + j);
				}
			}

			// Verifies arc
			for(int j = 0; j < dimension; j++){
				if(p->assignment[index][j]){
					subtour.push_back(j+1);
					index = j;
					break;
				}
			}

			// End of subtour
			if(start == index){
				this->subtours.push_back(subtour);
				subtour.clear();

				break;
			}
		}
	}

	if(this->subtours.size() == 1){
		this->upperBound = true;
	}else{
		this->upperBound = false;
	}

	int smallerSubtour = INT_MAX;

	// Chooses smaller subtour
	for(int i = 0; i < this->subtours.size(); i++){
		if(this->subtours[i].size() < smallerSubtour){
			smallerSubtour = this->subtours[i].size();
			this->choosen = i;
		}
	}
}

void Node::printSolution(double time){

    if(this->upperBound){

		cout << "\nSolution: ";
		for(int i = 0; i < this->subtours[0].size(); i++)
			cout << this->subtours[0][i] << " ";

		cout << endl << "Cost: " << this->lowerBound << endl;

	}else{
		cout << "No feasible solution was found!" << endl;
	}

	cout << "Time: " << time << endl << endl;

}

bool Node::getUpperBound(){
    return this->upperBound;
}

double Node::getLowerBound(){
    return this->lowerBound;
}

vector <int> Node::getChoosenSubtour(){
    return this->subtours[this->choosen];
}

vector <pair <int, int>> Node::getProhibitedArcs(){
    return this->illegalArcs;
}

void Node::setProhibitedArc(pair <int, int> arc){
    this->illegalArcs.push_back(arc);
}

void Node::setProhibitedArcs(vector <pair <int, int>> arcs){
    this->illegalArcs = arcs;
}

void Node::setLowerBound(double lowerBound){
    this->lowerBound = lowerBound;
}