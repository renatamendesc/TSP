#include "data.h"
#include "hungarian.h"
#include <iostream>
#include <vector>

using namespace std;

double ** matrizReal;
double ** matrizModificada;
int dimension;

typedef struct {

	vector <pair <int, int>> arcosProibidos; // Lista de arcos que foram proibidos
	vector <vector <int>> subtours; // Lista de subtours gerados pela solução do algoritmo húngaro

	double lowerBound; // Resultado da solução gerada pelo algoritmo húngaro
	int escolhido; // Subtour escolhido que terá arcos proibidos
	bool podar; // Indica se o nó deve gerar filhos

} Node;

void formaSubtours(hungarian_problem_t *p, Node &node){
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
				node.subtours.push_back(subtour);
				subtour.clear();

				break;
			}
		}
	}
}

void branchAndBound(){

	vector <Node> arvore;
	Node raiz;

	hungarian_problem_t p;
	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	hungarian_init(&p, matrizModificada, dimension, dimension, mode); // Carregando o problema

	raiz.lowerBound = hungarian_solve(&p);
	arvore.push_back(raiz);

	formaSubtours(&p, raiz);

	hungarian_free(&p);

}

int main(int argc, char** argv) {

	Data * data = new Data(argc, argv[1]);
	data->readData();

	double **cost = new double*[data->getDimension()];
	for (int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}

	matrizReal = cost;
	matrizModificada = cost;
	dimension = data->getDimension();

	branchAndBound();

	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}
