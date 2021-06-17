#include "data.h"
#include "hungarian.h"
#include "node.h"
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

// Função para especificar ordem da priority_queue, sobrecarregando o operador
bool operator < (Node a, Node b){
	return a.getLowerBound() > b.getLowerBound();
}

void search(string tipo, double ** matrizReal, int dimension){

	clock_t inicio = clock(); // Inicia contagem do tempo

	Node raiz, solucao;
	hungarian_problem_t p;

	vector <Node> arvore;
	priority_queue <Node> arvoreMenorBound;

	hungarian_init(&p, matrizReal, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST);
	raiz.setLowerBound(hungarian_solve(&p));
	raiz.calcularSolucao(&p, dimension);
	hungarian_free(&p);

	int index;
	double tempo, custo = __DBL_MAX__;
	double ** matrizModificada = new double *[dimension];

	// Matriz modificada recebe matriz original
	for(int i = 0; i < dimension; i++){
		matrizModificada[i] = new double [dimension];

		for(int j = 0; j < dimension; j++){
			matrizModificada[i][j] = matrizReal[i][j];
		}
	}

	solucao = raiz;

	if(tipo == "menorBound"){
		arvoreMenorBound.push(raiz);
	} else {
		arvore.push_back(raiz);
	}

	while(!arvore.empty() || !arvoreMenorBound.empty()){

		Node node;

		if(tipo == "menorBound"){
			node = arvoreMenorBound.top();
			arvoreMenorBound.pop();
			
		}else if(tipo == "largura"){
			node = arvore.front();
			arvore.erase(arvore.begin());

		}else if(tipo == "profundidade"){
			node = arvore.back();
			arvore.erase(arvore.end());

		}

		if(!node.getPodar()){

			// Percorre arcos a serem proibidos
			for(int j = 0; j < node.getSubtourEscolhido().size()-1; j++){

				Node newNode;
				newNode.setArcosProibidos(node.getArcosProibidos()); // Novo nó herda arcos proibidos

				// Define o arco proibido em questão
				pair <int, int> arco;

				arco.first = node.getSubtourEscolhido()[j];
				arco.second = node.getSubtourEscolhido()[j+1];

				// Adiciona novo arco proibido
				newNode.setArcoProibido(arco);
				newNode.proibeArcos(dimension, matrizModificada, matrizReal);

				hungarian_problem_t p;
				hungarian_init(&p, matrizModificada, dimension, dimension, HUNGARIAN_MODE_MINIMIZE_COST);
				newNode.setLowerBound(hungarian_solve(&p));

				// Verifica se há algum upper bound menor que o lower bound encontrado
				if(newNode.getLowerBound() < custo){
					newNode.calcularSolucao(&p, dimension);

					// Adiciona na árvore
					if(tipo == "menorBound"){
						arvoreMenorBound.push(newNode);
					}else{
						arvore.push_back(newNode); 
					}	
				}

				hungarian_free(&p);
			}

		} else {

			// Verifica o melhor o custo
			if(node.getLowerBound() < custo){
				custo = node.getLowerBound();
				solucao = node;
			}

		}

		clock_t fim = clock();
		tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;
		if(tempo > 600) break;

	}

	for(int i = 0; i < dimension; i++) delete[] matrizModificada[i];
	delete[] matrizModificada;

	solucao.printSolucao(tempo);

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

	double ** matrizReal = cost;
	int selection, dimension = data->getDimension();

	string tipo;

	cout << "\n------------------------------------------------\n\n";
	cout << "Selecione um tipo de busca:\n\n";
	cout << "[1] Melhor bound\n";
	cout << "[2] Largura\n";
	cout << "[3] Profundidade\n";
	cout << "\n------------------------------------------------\n";

	cin >> selection;

	switch(selection){

		case 1:
			tipo = "menorBound";
			break;

		case 2:
			tipo = "largura";
			break;

		case 3:
			tipo = "profundidade";
			break;

	}

	search(tipo, matrizReal, dimension);

	for (int i = 0; i < data->getDimension(); i++) delete [] cost[i];
	delete [] cost;
	delete data;

	return 0;
}