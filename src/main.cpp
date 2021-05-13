#include "data.h"
#include "hungarian.h"
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

#define INFINITO 99999999

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

void proibeArcos(Node &node){
	matrizModificada = matrizReal;

	// Percorre arcos a serem proibidos
	for(int i = 0; i < node.arcosProibidos.size(); i++){
		// Adiciona restrição referente aos arcos proibidos
		matrizModificada[node.arcosProibidos[i].first-1][node.arcosProibidos[i].second-1] = INFINITO;
	}
}

void procuraArcosProibidos(Node &node){
	int menorSubtour = INT_MAX;

	if(node.subtours.size() == 1){
		node.podar = true;

	}else{
		node.podar = false;

		// Verifica subtour de menor tamanho
		for(int i = 0; i < node.subtours.size(); i++){
			if(node.subtours[i].size() < menorSubtour){
				menorSubtour = node.subtours[i].size();
				node.escolhido = i;
			}
		}

		// Define os arcos proibidos
		for(int i = 0; i < node.subtours[node.escolhido].size()-1; i++){
			pair <int, int> arco;

			arco.first = node.subtours[node.escolhido][i];
			arco.second = node.subtours[node.escolhido][i+1];

			node.arcosProibidos.push_back(arco);
		}
	}
}

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
	float custo = __FLT_MAX__;
	vector <Node> arvore;
	Node raiz;

	int mode = HUNGARIAN_MODE_MINIMIZE_COST;
	hungarian_problem_t p;
	hungarian_init(&p, matrizModificada, dimension, dimension, mode); // Carregando o problema

	raiz.lowerBound = hungarian_solve(&p);
	cout << raiz.lowerBound << endl;
	formaSubtours(&p, raiz);
	procuraArcosProibidos(raiz);

	arvore.push_back(raiz); // Adiciona raiz

	while(true){
		int tam = arvore.size();
		bool flag = true;

		// Percorre nós existentes
		for(int i = 0; i < tam; i++){

			Node node = arvore[i], newNode;

			// Caso o nó possua mais de um subtour
			if(!node.podar){
				flag = false;

				// Gera um novo nó para cada arco proibido
				for(int i = 0; i < node.arcosProibidos.size(); i++){

					newNode.arcosProibidos.push_back(node.arcosProibidos[i]); // Nó gerado herda arco proibido
					proibeArcos(newNode); // Adiciona restrição de proibição

					hungarian_init(&p, matrizModificada, dimension, dimension, mode); // Carregando o problema

					newNode.lowerBound = hungarian_solve(&p);
					cout << newNode.lowerBound << endl;

					formaSubtours(&p, newNode);
					procuraArcosProibidos(newNode);

					arvore.push_back(newNode);
					
				}

				arvore.erase(arvore.begin() + i); // Apaga nó já utilizado
			}
		}

		if(flag){
			break;
		}
	}

	hungarian_free(&p);

	for(int i = 0; i < arvore.size(); i++){
		if(arvore[i].lowerBound < custo){
			custo = arvore[i].lowerBound;
		}
	}

	cout << "Custo: " << custo << endl;
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
