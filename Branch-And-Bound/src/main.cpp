#include "data.h"
#include "hungarian.h"
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

double ** matrizReal;
int dimension;

typedef struct {

	vector <pair <int, int>> arcosProibidos; // Lista de arcos que foram proibidos
	vector <vector <int>> subtours; // Lista de subtours gerados pela solução do algoritmo húngaro

	double lowerBound; // Resultado da solução gerada pelo algoritmo húngaro
	int escolhido; // Subtour escolhido que terá arcos proibidos
	bool podar; // Indica se o nó deve gerar filhos

} Node;

void printSolucao(Node &solucao){

	if(solucao.podar){
		cout << "Solucao: ";
		for(int i = 0; i < solucao.subtours[0].size(); i++)
			cout << solucao.subtours[0][i] << " ";

		cout << endl << "Custo: " << solucao.lowerBound << endl;

	}else{
		cout << "Nenhuma solução viável foi encontrada!" << endl;
	}
}

void proibeArcos(Node &node, double ** matrizModificada){

	// Matriz modificada recebe matriz original
	for(int i = 0; i < dimension; i++){
        for(int j = 0; j < dimension; j++){
			matrizModificada[i][j] = matrizReal[i][j];
        }
    }

	// Percorre arcos a serem proibidos
	for(int i = 0; i < node.arcosProibidos.size(); i++){
		// Adiciona restrição referente aos arcos proibidos
		matrizModificada[node.arcosProibidos[i].first-1][node.arcosProibidos[i].second-1] = INFINITE;
	}
}

void calcularSolucao(hungarian_problem_t *p, Node &node){
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

	if(node.subtours.size() == 1){
		node.podar = true;
	}else{
		node.podar = false;
	}

	int menorSubtour = INT_MAX;

	// Verifica subtour a ser escolhido
	for(int i = 0; i < node.subtours.size(); i++){
		if(node.subtours[i].size() < menorSubtour){
			menorSubtour = node.subtours[i].size();
			node.escolhido = i;
		}
	}
}

void melhorBound(Node raiz){

	clock_t inicio = clock();

	double tempo, custo = __DBL_MAX__;
	vector <Node> arvore;
	Node solucao, melhorNode;

	double ** matrizModificada = new double *[dimension];

	// Matriz modificada recebe matriz original
	for(int i = 0; i < dimension; i++){
		matrizModificada[i] = new double [dimension];

		for(int j = 0; j < dimension; j++){
			matrizModificada[i][j] = matrizReal[i][j];
		}
	}

	arvore.push_back(raiz);
	melhorNode = arvore[0];

	int index = 0;
	while(!arvore.empty()){

		Node node = melhorNode;

		// Verifica se o nó em questão tem solução viável
		if(!node.podar){

			// Percorre arcos a serem proibidos
			for(int j = 0; j < node.subtours[node.escolhido].size()-1; j++){

				Node newNode;
				newNode.arcosProibidos = node.arcosProibidos; // Novo nó herda arcos proibidos 

				// Define o arco proibido em questão
				pair <int, int> arco;

				arco.first = node.subtours[node.escolhido][j];
				arco.second = node.subtours[node.escolhido][j+1];

				// Adiciona novo arco proibido
				newNode.arcosProibidos.push_back(arco);
				proibeArcos(newNode, matrizModificada);

				hungarian_problem_t p;
				hungarian_init(&p, matrizModificada, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST); // Carregando o problema

				newNode.lowerBound = hungarian_solve(&p);

				// Verifica se há algum upper bound menor que o lower bound encontrado
				if(newNode.lowerBound < custo){
					calcularSolucao(&p, newNode);
					arvore.push_back(newNode); // Adiciona na árvore
				}
				
				hungarian_free(&p);
			}

		} else {

			// Verifica o melhor o custo
			if(node.lowerBound < custo){
				custo = node.lowerBound;
				solucao = node;
			}

		}

		arvore.erase(arvore.begin() + index); // Apaga nó já analisado

		clock_t fim = clock();
		tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
		if(tempo > 600) break;

		melhorNode.lowerBound = __DBL_MAX__;
		for(int i = 0; i < arvore.size(); i++){
			if(arvore[i].lowerBound < melhorNode.lowerBound){
				melhorNode = arvore[i];
				index = i;
			}
		}
	}

	for(int i = 0; i < dimension; i++) delete[] matrizModificada[i];
	delete[] matrizModificada;

	printSolucao(solucao);

	cout << "Tempo: " << tempo << endl;
}

void largura(Node raiz){

	clock_t inicio = clock();

	double tempo, custo = __DBL_MAX__;
	vector <Node> arvore;
	Node solucao;

	double ** matrizModificada = new double *[dimension];

	// Matriz modificada recebe matriz original
	for(int i = 0; i < dimension; i++){
		matrizModificada[i] = new double [dimension];

		for(int j = 0; j < dimension; j++){
			matrizModificada[i][j] = matrizReal[i][j];
		}
	}

	arvore.push_back(raiz);

	while(!arvore.empty()){

		// Percorre nós da árvore	
		Node node = arvore[0];

		// Verifica se o nó em questão tem solução viável
		if(!node.podar){

			// Percorre arcos a serem proibidos
			for(int j = 0; j < node.subtours[node.escolhido].size()-1; j++){

				Node newNode;
				newNode.arcosProibidos = node.arcosProibidos; // Novo nó herda arcos proibidos 

				// Define o arco proibido em questão
				pair <int, int> arco;

				arco.first = node.subtours[node.escolhido][j];
				arco.second = node.subtours[node.escolhido][j+1];

				// Adiciona novo arco proibido
				newNode.arcosProibidos.push_back(arco);
				proibeArcos(newNode, matrizModificada);

				hungarian_problem_t p;
				hungarian_init(&p, matrizModificada, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST); // Carregando o problema

				newNode.lowerBound = hungarian_solve(&p);

				// Verifica se há algum upper bound menor que o lower bound encontrado
				if(newNode.lowerBound < custo){
					calcularSolucao(&p, newNode);
					arvore.push_back(newNode); // Adiciona na árvore

				}
				
				hungarian_free(&p);
			}

		} else {

			// Verifica o melhor o custo
			if(node.lowerBound < custo){
				custo = node.lowerBound;
				solucao = node;
			}

		}

		arvore.erase(arvore.begin()); // Apaga nó já analisado

		clock_t fim = clock();
		tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
		if(tempo > 600) break;

	}

	for(int i = 0; i < dimension; i++) delete[] matrizModificada[i];
	delete[] matrizModificada;

	printSolucao(solucao);

	cout << "Tempo: " << tempo << endl;

}

void profundidade(Node raiz){

	clock_t inicio = clock();

	double tempo, custo = __DBL_MAX__;
	vector <Node> arvore;
	Node solucao;

	double ** matrizModificada = new double *[dimension];

	// Matriz modificada recebe matriz original
	for(int i = 0; i < dimension; i++){
		matrizModificada[i] = new double [dimension];

		for(int j = 0; j < dimension; j++){
			matrizModificada[i][j] = matrizReal[i][j];
		}
	}

	arvore.push_back(raiz);

	while(!arvore.empty()){
		int tam = arvore.size();

		// Percorre nós da árvore	
		Node node = arvore[tam-1];

		// Verifica se o nó em questão tem solução viável
		if(!node.podar){

			// Percorre arcos a serem proibidos
			for(int j = 0; j < node.subtours[node.escolhido].size()-1; j++){

				Node newNode;
				newNode.arcosProibidos = node.arcosProibidos; // Novo nó herda arcos proibidos

				// Define o arco proibido em questão
				pair <int, int> arco;

				arco.first = node.subtours[node.escolhido][j];
				arco.second = node.subtours[node.escolhido][j+1];

				// Adiciona novo arco proibido
				newNode.arcosProibidos.push_back(arco);

				proibeArcos(newNode, matrizModificada);

				hungarian_problem_t p;
				hungarian_init(&p, matrizModificada, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST); // Carregando o problema
				newNode.lowerBound = hungarian_solve(&p);

				// Verifica se há algum upper bound menor que o lower bound encontrado
				if(newNode.lowerBound < custo){
					calcularSolucao(&p, newNode);
					arvore.push_back(newNode); // Adiciona na árvore
				}

				hungarian_free(&p);
			}

		} else {

			// Verifica o melhoro o custo
			if(node.lowerBound < custo){
				custo = node.lowerBound;
				solucao = node;
			}

		}

		arvore.erase(arvore.begin() + tam - 1); // Apaga nó já analisado

		clock_t fim = clock();
		tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
		if(tempo > 600) break;
		
	}

	for(int i = 0; i < dimension; i++) delete[] matrizModificada[i];
	delete[] matrizModificada;

	printSolucao(solucao);

	cout << "Tempo: " << tempo << endl;

}

int main(int argc, char** argv){

	Data * data = new Data(argc, argv[1]);
	data->readData();

	double ** cost = new double*[data->getDimension()];
	for(int i = 0; i < data->getDimension(); i++){
		cost[i] = new double[data->getDimension()];
		for (int j = 0; j < data->getDimension(); j++){
			cost[i][j] = data->getDistance(i,j);
		}
	}

	dimension = data->getDimension();
	matrizReal = cost;

	Node raiz;
	hungarian_problem_t p;

	hungarian_init(&p, matrizReal, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST); // Carregando o problema
	raiz.lowerBound = hungarian_solve(&p);
	calcularSolucao(&p, raiz); // Calcula subtours da solução

	cout << "Selecione o tipo de busca:" << endl;
	cout << "[1] Melhor bound" << endl;
	cout << "[2] Largura" << endl;
	cout << "[3] Profundidade" << endl;

	int selection;
	cin >> selection;

	switch(selection){

		case 1:
			melhorBound(raiz);
			break;

		case 2:
			largura(raiz);
			break;

		case 3:
			profundidade(raiz);
			break;

	}

	hungarian_free(&p);

	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}
