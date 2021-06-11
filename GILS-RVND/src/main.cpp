#include "readData.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <cfloat>
#include <algorithm>

using namespace std;

double ** matrizAdj; //Matriz de adjacencia
int dimension; //Quantidade total de vertices

struct insertionInfo{
  int noInserido; //Cidade inserida
  int arestaRemovida; 
  double custo;
};

struct vizinhoInfo{
  int iMenor;
  int jMenor;
  double custoMenor;
};

//Função de comparação utilizada para ordenar o vetor na construção:
bool comparacao(insertionInfo a, insertionInfo b){
  
  return a.custo < b.custo;
  
}

//Função que calcula a distancia total do percurso em questao:
double percursoTotal(vector <int> solucao){
  double percursoTotal = 0;
	
  for(int i = 0; i < solucao.size()-1; i++){
	  percursoTotal = percursoTotal + matrizAdj[solucao[i]][solucao[i+1]];
  }
	
  return percursoTotal;
}

//Estrutura de vizinhança swap:
vizinhoInfo swap(vector <int> &solucaoInicial){
  double delta, deltaParcial;
  int tam = solucaoInicial.size();
  vizinhoInfo melhorVizinho;
  melhorVizinho.custoMenor = DBL_MAX;
    
  for(int i = 1; i < tam-2; i++){
    deltaParcial = -(matrizAdj[solucaoInicial[i]][solucaoInicial[i+1]] + matrizAdj[solucaoInicial[i]][solucaoInicial[i-1]]);
    for(int j = i + 1; j < tam-1; j++){
      delta = deltaParcial;
   	
	//Calculo da diferença entre solução inicial e vizinha:       	
      if(j == i + 1){
        delta += matrizAdj[solucaoInicial[i]][solucaoInicial[j+1]] + matrizAdj[solucaoInicial[j]][solucaoInicial[i-1]] + matrizAdj[solucaoInicial[i]][solucaoInicial[i+1]] - matrizAdj[solucaoInicial[j]][solucaoInicial[j+1]];
      }else {
        delta += matrizAdj[solucaoInicial[i]][solucaoInicial[j+1]] + matrizAdj[solucaoInicial[i]][solucaoInicial[j-1]] + matrizAdj[solucaoInicial[j]][solucaoInicial[i+1]] + matrizAdj[solucaoInicial[j]][solucaoInicial[i-1]] - matrizAdj[solucaoInicial[j]][solucaoInicial[j+1]] - matrizAdj[solucaoInicial[j]][solucaoInicial[j-1]];
      }
            
      if(delta < melhorVizinho.custoMenor){
        melhorVizinho.custoMenor = delta;
        melhorVizinho.iMenor = i;
        melhorVizinho.jMenor = j;
        
      }           	
    }
  }

  return melhorVizinho;	

}

//Estrutura de vizinhança reinsertion:
vizinhoInfo reinsertion(vector <int> &solucaoInicial){
  double delta, deltaParcial;
  int tam = solucaoInicial.size();
  vizinhoInfo melhorVizinho;
  melhorVizinho.custoMenor = DBL_MAX;

 for(int i = 1; i < tam-2; i++){
   deltaParcial = matrizAdj[solucaoInicial[i-1]][solucaoInicial[i+1]] - matrizAdj[solucaoInicial[i]][solucaoInicial[i-1]] - matrizAdj[solucaoInicial[i]][solucaoInicial[i+1]];
    for(int j = i + 1; j < tam-1; j++){
      delta = deltaParcial;

      if(j == i + 1){
        delta += matrizAdj[solucaoInicial[i]][solucaoInicial[j+1]] + matrizAdj[solucaoInicial[j]][solucaoInicial[i-1]] - matrizAdj[solucaoInicial[j]][solucaoInicial[j+1]] - matrizAdj[solucaoInicial[i-1]][solucaoInicial[i+1]] + matrizAdj[solucaoInicial[i]][solucaoInicial[i+1]];
      } else {
        delta += matrizAdj[solucaoInicial[i]][solucaoInicial[j]] + matrizAdj[solucaoInicial[i]][solucaoInicial[j+1]] - matrizAdj[solucaoInicial[j]][solucaoInicial[j+1]];
      }

      if(delta < melhorVizinho.custoMenor){
        melhorVizinho.iMenor = i;
        melhorVizinho.jMenor = j;  
        melhorVizinho.custoMenor = delta;
      }	
    }
  }

  for(int j = 1; j < tam-2; j++){
    deltaParcial = -(matrizAdj[solucaoInicial[j]][solucaoInicial[j-1]]);
    for(int i = j + 1; i < tam-1; i++){
      delta = deltaParcial;

      if(i == j + 1){
        delta += matrizAdj[solucaoInicial[j]][solucaoInicial[i+1]] + matrizAdj[solucaoInicial[i]][solucaoInicial[j-1]] - matrizAdj[solucaoInicial[i]][solucaoInicial[i+1]];
      } else {
        delta += matrizAdj[solucaoInicial[i-1]][solucaoInicial[i+1]] + matrizAdj[solucaoInicial[i]][solucaoInicial[j]] + matrizAdj[solucaoInicial[i]][solucaoInicial[j-1]] - matrizAdj[solucaoInicial[i]][solucaoInicial[i-1]] - matrizAdj[solucaoInicial[i]][solucaoInicial[i+1]];
      }

      if(delta < melhorVizinho.custoMenor){

        melhorVizinho.iMenor = i;
        melhorVizinho.jMenor = j;  
        melhorVizinho.custoMenor = delta;
      }	
    }
  }

return melhorVizinho;

}

//Estrutura de vizinhança 2-opt:
vizinhoInfo inversao(vector <int> &solucaoInicial){
  double delta, deltaParcial;
  int tam = solucaoInicial.size();
  vizinhoInfo melhorVizinho;
  melhorVizinho.custoMenor = DBL_MAX;

  for(int i = 1; i < tam; i++){
    deltaParcial = -(matrizAdj[solucaoInicial[i-1]][solucaoInicial[i]]);
    for(int j = i + 1; j < tam-1; j++){
      delta = deltaParcial;
	            		
	    //Calculo da diferença entre solução inicial e vizinha:    		
	    delta += matrizAdj[solucaoInicial[i-1]][solucaoInicial[j]] + matrizAdj[solucaoInicial[j+1]][solucaoInicial[i]] - matrizAdj[solucaoInicial[j]][solucaoInicial[j+1]];
	            		
	    if(delta < melhorVizinho.custoMenor){
	      
        melhorVizinho.iMenor = i;
        melhorVizinho.jMenor = j;
        melhorVizinho.custoMenor = delta;
	    }

    }
  }

  return melhorVizinho;
  
}

//Estrutura de vizinhança or-opt-2:
vizinhoInfo oropt2(vector <int> &solucaoInicial){
  double delta, deltaParcial;
  int tam = solucaoInicial.size();
  vizinhoInfo melhorVizinho;
  melhorVizinho.custoMenor = DBL_MAX;

  for(int i = 1; i < tam-2; i++){
    deltaParcial = matrizAdj[solucaoInicial[i-1]][solucaoInicial[i+2]] - matrizAdj[solucaoInicial[i-1]][solucaoInicial[i]] - matrizAdj[solucaoInicial[i+1]][solucaoInicial[i+2]];
    for(int j = 1; j <= tam-3; j++){
      delta = deltaParcial;

      if(i != j){
        //Calculo da diferença entre solução inicial e vizinha:           
        if(i < j){
          delta += matrizAdj[solucaoInicial[j+1]][solucaoInicial[i]] + matrizAdj[solucaoInicial[j+2]][solucaoInicial[i+1]] - matrizAdj[solucaoInicial[j+1]][solucaoInicial[j+2]];
        } else {
          delta += matrizAdj[solucaoInicial[j]][solucaoInicial[i+1]] + matrizAdj[solucaoInicial[j-1]][solucaoInicial[i]] - matrizAdj[solucaoInicial[j]][solucaoInicial[j-1]];
        }
                  
        if(delta < melhorVizinho.custoMenor){

          melhorVizinho.iMenor = i;	
          melhorVizinho.jMenor = j;
          melhorVizinho.custoMenor = delta;
        } 
      }
    }
  }

  return melhorVizinho;

}

//Estrutura de vizinhança or-opt-3:
vizinhoInfo oropt3(vector <int> &solucaoInicial){
  double delta, deltaParcial;
  int tam = solucaoInicial.size();
  vizinhoInfo melhorVizinho;
  melhorVizinho.custoMenor = DBL_MAX;

  for(int i = 1; i < tam-3; i++){
    deltaParcial = matrizAdj[solucaoInicial[i-1]][solucaoInicial[i+3]] - matrizAdj[solucaoInicial[i-1]][solucaoInicial[i]] - matrizAdj[solucaoInicial[i+2]][solucaoInicial[i+3]];
    for(int j = 1; j <= tam-4; j++){
      if(i != j){
        delta = deltaParcial;

        //Calculo da diferença entre solução inicial e vizinha: 
	    	if(i < j){
          delta += matrizAdj[solucaoInicial[j+2]][solucaoInicial[i]] + matrizAdj[solucaoInicial[j+3]][solucaoInicial[i+2]] - matrizAdj[solucaoInicial[j+2]][solucaoInicial[j+3]];
		    } else {
          delta += matrizAdj[solucaoInicial[j]][solucaoInicial[i+2]] + matrizAdj[solucaoInicial[j-1]][solucaoInicial[i]] - matrizAdj[solucaoInicial[j-1]][solucaoInicial[j]];
        }
            		
        if(delta < melhorVizinho.custoMenor){

	        melhorVizinho.iMenor = i;
          melhorVizinho.jMenor = j;
          melhorVizinho.custoMenor = delta;
	    	} 

      }

    }

  }

  return melhorVizinho;

}

//Função para chamar as estruturas de vizinhança:
void RVND(vector <int> &solucao){
  vector <int> movimentos = {0, 1, 2, 3, 4};
  vizinhoInfo vizinho;

  while(!movimentos.empty()){
    int escolhido = rand() % movimentos.size();

    if(movimentos[escolhido] == 0){

      vizinho = swap(solucao);

      //Realiza o movimento:
      if(vizinho.custoMenor < 0){

        int aux = solucao[vizinho.iMenor];
	      solucao[vizinho.iMenor] = solucao[vizinho.jMenor];
	      solucao[vizinho.jMenor] = aux;

        movimentos = {0, 1, 2, 3, 4};

      }else {
        movimentos.erase(movimentos.begin() + escolhido);
      }

    }else if(movimentos[escolhido] == 1){

      vizinho = reinsertion(solucao);

      if(vizinho.custoMenor < 0){
        vector <int> solucaoInicial = solucao;

        solucao.erase(solucao.begin()+vizinho.iMenor);
        solucao.insert(solucao.begin()+vizinho.jMenor, solucaoInicial[vizinho.iMenor]);

        movimentos = {0, 1, 2, 3, 4};
      }else {
        movimentos.erase(movimentos.begin() + escolhido);
      }

    }else if(movimentos[escolhido] == 2){

      vizinho = inversao(solucao);

      if(vizinho.custoMenor < 0){
        int aux, k = vizinho.jMenor - vizinho.iMenor;

        if(k % 2 != 0){
          k = k + 1;
        }

        for(int q = 0; q < k/2; q++){
          aux = solucao[vizinho.iMenor+q];
          solucao[vizinho.iMenor+q] = solucao[vizinho.jMenor-q];
          solucao[vizinho.jMenor-q] = aux;
        }

        movimentos = {0, 1, 2, 3, 4};

      } else {
        movimentos.erase(movimentos.begin() + escolhido);
      }

    }else if(movimentos[escolhido] == 3){

      vizinho = oropt2(solucao);

      //Realiza o movimento:
      if(vizinho.custoMenor < 0){
        if(vizinho.iMenor < vizinho.jMenor){
          solucao.insert(solucao.begin() + vizinho.jMenor + 2, solucao[vizinho.iMenor]); 
          solucao.insert(solucao.begin() + vizinho.jMenor + 3, solucao[vizinho.iMenor+1]); 
          solucao.erase(solucao.begin() + vizinho.iMenor);
          solucao.erase(solucao.begin() + vizinho.iMenor);
        } else {
          solucao.insert(solucao.begin() + vizinho.jMenor, solucao[vizinho.iMenor]); 
          solucao.insert(solucao.begin() + vizinho.jMenor + 1, solucao[vizinho.iMenor + 2]); 
          solucao.erase(solucao.begin() + vizinho.iMenor + 2);
          solucao.erase(solucao.begin() + vizinho.iMenor + 2);
        }

        movimentos = {0, 1, 2, 3, 4};
      } else {
        movimentos.erase(movimentos.begin() + escolhido);
      }   

    }else if(movimentos[escolhido] == 4){

      vizinho = oropt3(solucao);

      //Realiza o movimento:
      if(vizinho.custoMenor < 0){
        if(vizinho.iMenor < vizinho.jMenor){
          solucao.insert(solucao.begin() + vizinho.jMenor + 3, solucao[vizinho.iMenor]);
          solucao.insert(solucao.begin() + vizinho.jMenor + 4, solucao[vizinho.iMenor+1]); 
          solucao.insert(solucao.begin() + vizinho.jMenor + 5, solucao[vizinho.iMenor+2]);
          solucao.erase(solucao.begin() + vizinho.iMenor);
          solucao.erase(solucao.begin() + vizinho.iMenor);
          solucao.erase(solucao.begin() + vizinho.iMenor);
        } else {
          solucao.insert(solucao.begin() + vizinho.jMenor, solucao[vizinho.iMenor]);
          solucao.insert(solucao.begin() + vizinho.jMenor + 1, solucao[vizinho.iMenor + 2]); 
          solucao.insert(solucao.begin() + vizinho.jMenor + 2, solucao[vizinho.iMenor + 4]); 
          solucao.erase(solucao.begin() + vizinho.iMenor + 3);
          solucao.erase(solucao.begin() + vizinho.iMenor + 3);
          solucao.erase(solucao.begin() + vizinho.iMenor + 3);
        }

        movimentos = {0, 1, 2, 3, 4};
      } else {
        movimentos.erase(movimentos.begin() + escolhido);
      }
      
    }
  }
}

vector <int> pertub(vector <int> solucaoInicial){
	vector <int> solucaoModificada;
	vector <int> subsequencia1;
	vector <int> subsequencia2;
	int i, j, tam1 = 0, tam2 = 0, tam = solucaoInicial.size();
	
	//Gera tamanho das subsequencias:
	if(tam < 20){
		tam1 = 2;
		tam2 = 2;
	} else {
		while(tam1 < 2 || tam1 > (tam/10)){
			tam1 = rand() % tam;
		}

		while(tam2 < 2 || tam2 > (tam/10)){
			tam2 = rand() % tam;
		}
	}
	
	//Gera posição incial da subsequencia 1:
	i = rand() % (tam - tam1);
  	while(i == 0){
    		i = rand() % (tam - tam1);
  	}

	//Gera posição incial da subsequencia 2:
	j = rand() % (tam - tam2);
	while((j > (i - tam2) && j < (i + tam1)) || j == 0){
		j = rand() % (tam - tam2);
	}

	//Cria um vetor da subsequencia 1:
	int contadorIteracoes = 0;
	for(int q = 0; q < tam; q++){
		if(q >= i){
			subsequencia1.push_back(solucaoInicial[q]);
      contadorIteracoes++;
			if(contadorIteracoes == tam1){
				break;
			}
		}
	}
	
	//Cria um vetor da subsequencia 2:
	contadorIteracoes = 0;
	for(int q = 0; q < tam; q++){
		if(q >= j){
			subsequencia2.push_back(solucaoInicial[q]);
      contadorIteracoes++;
			if(contadorIteracoes == tam2){
				break;
			}
		}
	}

	if(j < i){	
    //Apaga subsequencia 1:
    solucaoModificada = solucaoInicial;
    int apagados = 0;
    for(int q = 0; q < tam; q++){
      if(q >= i){
        solucaoModificada.erase(solucaoModificada.begin() + q - apagados);
        apagados++;
        if(apagados == tam1){
          break;
        }
      }
    }

    //Apaga subsequencia 2:
    apagados = 0;
    for(int q = 0; q < tam; q++){
      if(q >= j){
        solucaoModificada.erase(solucaoModificada.begin() + q - apagados);
        apagados++;
        if(apagados == tam2){
          break;
        }
      }
    }

    //Insere subsequencia 2 no lugar da 1:
    int colocados = 0;
    for(int q = 0; q < subsequencia2.size(); q++){
      solucaoModificada.insert(solucaoModificada.begin() + i + colocados - tam2, subsequencia2[q]);
      colocados++;
    }
        
    //Insere subsequencia 1 no lugar da 2:
    colocados = 0;
    for(int q = 0; q < subsequencia1.size(); q++){
      solucaoModificada.insert(solucaoModificada.begin() + j + colocados, subsequencia1[q]);
      colocados++;
    }
      
	} else {
    //Apaga subsequencia 1:
    solucaoModificada = solucaoInicial;
    int apagados = 0;
    for(int q = 0; q < tam; q++){
      if(q >= j){
        solucaoModificada.erase(solucaoModificada.begin() + q - apagados);
        apagados++;
        if(apagados == tam2){
          break;
        }
      }
    }

    //Apaga subsequencia 2:
    apagados = 0;
    for(int q = 0; q < tam; q++){
      if(q >= i){
        solucaoModificada.erase(solucaoModificada.begin() + q - apagados);
        apagados++;
        if(apagados == tam1){
          break;
        }
      }
	  }

    //Insere subsequencia 1 no lugar da 2:
    int colocados = 0;
    for(int q = 0; q < subsequencia1.size(); q++){
      solucaoModificada.insert(solucaoModificada.begin() + j + colocados - tam1, subsequencia1[q]);
      colocados++;
    }
        
    //Insere subsequencia 2 no lugar da 1:
    colocados = 0;
    for(int q = 0; q < subsequencia2.size(); q++){
      solucaoModificada.insert(solucaoModificada.begin() + i + colocados, subsequencia2[q]);
      colocados++;
    }
  }
		
	return solucaoModificada;	
}

//Função de construção da solução inicial:
vector <int> construcao(double valorAleatorio, vector <int> listaCandidatos){
  vector <int> s;

  //Insere o depósito no percurso: 
  s.push_back(listaCandidatos[0]);
  s.push_back(listaCandidatos[0]);
  listaCandidatos.erase(listaCandidatos.begin());
    
  //Cidades do subtour sao escolhidas aleatoriamente:
  for(int j = 0; j < 3; j++){
    int l = rand() % listaCandidatos.size(); //Nó aleatório é escolhido
    s.insert(s.begin()+1, listaCandidatos[l]); 
    listaCandidatos.erase(listaCandidatos.begin()+l);
  }

  //Calcula os custo de insercao de cada cidade em s (percurso):
  while(!listaCandidatos.empty()){
    vector <insertionInfo> custoInsercao((s.size()-1) * listaCandidatos.size());
    for(int j = 0, l = 1, q = 0; j < s.size() - 1; j++, l++){
      for(auto k : listaCandidatos){
        custoInsercao[q].custo = matrizAdj[s[j]][k] + matrizAdj[s[l]][k] - matrizAdj[s[j]][s[l]];
        custoInsercao[q].noInserido = k;
        custoInsercao[q].arestaRemovida = j;
        q++;
      }
    }

    //Ordena os custos de insercao em ordem crescente:
    sort(custoInsercao.begin(), custoInsercao.end(), comparacao);

    //Gera quantidade de elementos iniciais a serem escolhidos aleatoriamente no percurso:
    int primeirosElementos = valorAleatorio * custoInsercao.size();
    int l = rand() % primeirosElementos;

    //Insere nó escolhido no percuso:
    s.insert(s.begin() + (custoInsercao[l].arestaRemovida + 1), custoInsercao[l].noInserido);

    //Apaga da lista de candidatos o nó que foi inserido:
    for(int q = 0; q < listaCandidatos.size(); q++){
      if(listaCandidatos[q] == custoInsercao[l].noInserido)
        listaCandidatos.erase(listaCandidatos.begin()+q);

    }

  }
        
  return s;
}

//Função com algoritmo principal:
double tsp(int iIls, int v){
  double distanciaFinal = DBL_MAX;
  vector <int> solucaoFinal;
  vector <int> cidades;

  //Gera vetor com cidades:
  for(int i = 1; i <= v; i++){
    cidades.push_back(i);
  }

  for(int iterMax = 0; iterMax < 50; iterMax++){
    double valorAleatorio = (rand() % 90) / 100.0 + 0.1;
    double distanciaS, distanciaSolucao = DBL_MAX;
    
    vector <int> s = construcao(valorAleatorio, cidades); //Fase de construção da solução inicial
    vector <int> solucao = s;

    int iterIls = 0;
    while(iterIls < iIls){
      RVND(s);

      distanciaS = percursoTotal(s);

      if(distanciaS < distanciaSolucao){
        solucao = s;
        iterIls = 0;
        distanciaSolucao = percursoTotal(solucao);
      }

      s = pertub(solucao);

      iterIls++;

    }

    if(distanciaSolucao < distanciaFinal){
      solucaoFinal = solucao;
      distanciaFinal = distanciaSolucao;
    }
    	  
  }

  cout << endl;

  cout << "Percurso final: ";
  for(int i = 0; i < solucaoFinal.size(); i++){
    printf("%d ", solucaoFinal[i]);
  }

  return distanciaFinal;
  
}

int main(int argc, char** argv) {
  //Inicio da contagem do tempo:  
  clock_t inicio = clock();
    
  readData(argc, argv, &dimension, &matrizAdj);

  srand(time(NULL));

  int iIls;
    
  if(dimension >= 150){
    iIls = dimension/2;
  } else {
    iIls = dimension;
  }

  double custoFinal = tsp(iIls, dimension);

  //Fim da contagem do tempo:
  clock_t fim = clock();
  double tempo = ((double) (fim - inicio)) / CLOCKS_PER_SEC;

  cout << endl;
  cout << "Custo: " << custoFinal << endl;
  cout << "Tempo: " << tempo << endl;
  cout << endl;
    
  return 0;

}
