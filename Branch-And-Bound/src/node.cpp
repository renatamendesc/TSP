#include "node.h"

void Node::proibeArcos(int dimension, double ** matrizModificada, double ** matrizReal){

	// Matriz modificada recebe matriz original
	for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
			matrizModificada[i][j] = matrizReal[i][j];
        }
    }

	// Percorre arcos a serem proibidos
	for(int i = 0; i < this->arcosProibidos.size(); i++){
		// Adiciona restrição referente aos arcos proibidos
		matrizModificada[this->arcosProibidos[i].first-1][this->arcosProibidos[i].second-1] = INFINITE;
	}

}

void Node::calcularSolucao(hungarian_problem_t *p, int dimension){
	vector <int> listaCandidatos;
	vector <int> subtour;

	// Forma lista de candidatos
	for(int i = 0; i < dimension; i++){
		listaCandidatos.push_back(i+1);
	}

	// Forma solução de subtours
	while(!listaCandidatos.empty()){
		int i = listaCandidatos[0]-1; 
		int inicio = i;
		subtour.push_back(inicio + 1);

		while(true){
			// Apaga nó em questão da lista de candidatos
			for(int j = 0; j < listaCandidatos.size(); j++){
				if(listaCandidatos[j] == i+1){
					listaCandidatos.erase(listaCandidatos.begin() + j);
				}
			}

			// Verifica existência do arco
			for(int j = 0; j < dimension; j++){
				if(p->assignment[i][j]){
					subtour.push_back(j+1);
					i = j;
					break;
				}
			}

			// Encerra percurso do subtour
			if(inicio == i){
				this->subtours.push_back(subtour);
				subtour.clear();

				break;
			}
		}
	}

	if(this->subtours.size() == 1){
		this->podar = true;
	}else{
		this->podar = false;
	}

	int menorSubtour = INT_MAX;

	// Verifica subtour a ser escolhido
	for(int i = 0; i < this->subtours.size(); i++){
		if(this->subtours[i].size() < menorSubtour){
			menorSubtour = this->subtours[i].size();
			this->escolhido = i;
		}
	}
}

void Node::printSolucao(double tempo){

    if(this->podar){

		cout << "\nSolução: ";
		for(int i = 0; i < this->subtours[0].size(); i++)
			cout << this->subtours[0][i] << " ";

		cout << endl << "Custo: " << this->lowerBound << endl;

	}else{
		cout << "Nenhuma solução viável foi encontrada!" << endl;
	}

	cout << "Tempo: " << tempo << endl << endl;

}

bool Node::getPodar(){
    return this->podar;
}

double Node::getLowerBound(){
    return this->lowerBound;
}

vector <int> Node::getSubtourEscolhido(){
    return this->subtours[this->escolhido];
}

vector <pair <int, int>> Node::getArcosProibidos(){
    return this->arcosProibidos;
}

void Node::setArcoProibido(pair <int, int> arco){
    this->arcosProibidos.push_back(arco);
}

void Node::setArcosProibidos(vector <pair <int, int>> arcos){
    this->arcosProibidos = arcos;
}

void Node::setLowerBound(double lowerBound){
    this->lowerBound = lowerBound;
}