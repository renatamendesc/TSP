#ifndef NODE_H
#define NODE_H

#include "data.h"
#include "hungarian.h"
#include <iostream>
#include <vector>
#include <climits>
using namespace std;

class Node {

    private:
        vector <pair <int, int>> arcosProibidos; // Lista de arcos que foram proibidos
        vector <vector <int>> subtours; // Lista de subtours gerados pela solução do algoritmo húngaro

        double lowerBound; // Resultado da solução gerada pelo algoritmo húngaro
        int escolhido; // Subtour escolhido que terá arcos proibidos
        bool podar; // Indica se o nó deve gerar filhos

    public:
        void proibeArcos(int dimension, double ** matrizModificada, double ** matrizReal);
        void calcularSolucao(hungarian_problem_t *p, int dimension);
        void printSolucao();

        bool getPodar();
        double getLowerBound();
        vector <int> getSubtourEscolhido();
        vector <pair <int, int>> getArcosProibidos();

        void setArcoProibido(pair <int, int> arco);
        void setArcosProibidos(vector <pair <int, int>> arcos);
        void setLowerBound(double lowerBound);

};

#endif