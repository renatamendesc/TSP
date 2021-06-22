#include "readData.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <ctime>
#include <cfloat>
#include <algorithm>

using namespace std;

double ** distanceMatrix; // Matrix with distances
int dimension; // Vertices number

struct insertionInfo{
  int insertedNode;
  int deletedEdge; 
  double cost;
};

struct neighborInfo{
  int iBest;
  int jBest;
  double bestCost;
};

//Função de comparação utilizada para ordenar o vetor na construção:
bool comparacao(insertionInfo a, insertionInfo b){
  return a.cost < b.cost;
}

//Função que calcula a distancia total do percurso em questao:
double calculateCost(vector <int> solution){
  double cost = 0;
	
  for(int i = 0; i < solution.size()-1; i++){
	  cost = cost + distanceMatrix[solution[i]][solution[i+1]];
  }
	
  return cost;
}

neighborInfo swap(vector <int> &initialSolution){
  int size = initialSolution.size();
  double delta, partialDelta;

  neighborInfo bestNeighbor;
  bestNeighbor.bestCost = DBL_MAX;
    
  for(int i = 1; i < size-2; i++){
    partialDelta = -(distanceMatrix[initialSolution[i]][initialSolution[i+1]] + distanceMatrix[initialSolution[i]][initialSolution[i-1]]);

    for(int j = i + 1; j < size-1; j++){
      delta = partialDelta;
   	
	    // Diference from initial solution and neighbor      	
      if(j == i + 1){
        delta += distanceMatrix[initialSolution[i]][initialSolution[j+1]] + distanceMatrix[initialSolution[j]][initialSolution[i-1]] + distanceMatrix[initialSolution[i]][initialSolution[i+1]] - distanceMatrix[initialSolution[j]][initialSolution[j+1]];
      }else {
        delta += distanceMatrix[initialSolution[i]][initialSolution[j+1]] + distanceMatrix[initialSolution[i]][initialSolution[j-1]] + distanceMatrix[initialSolution[j]][initialSolution[i+1]] + distanceMatrix[initialSolution[j]][initialSolution[i-1]] - distanceMatrix[initialSolution[j]][initialSolution[j+1]] - distanceMatrix[initialSolution[j]][initialSolution[j-1]];
      }
            
      if(delta < bestNeighbor.bestCost){
        bestNeighbor.bestCost = delta;
        bestNeighbor.iBest = i;
        bestNeighbor.jBest = j; 
      }
    }
  }

  return bestNeighbor;	
}

neighborInfo reinsertion(vector <int> &initialSolution){
  int size = initialSolution.size();
  double delta, partialDelta;

  neighborInfo bestNeighbor;
  bestNeighbor.bestCost = DBL_MAX;

 for(int i = 1; i < size-2; i++){
   partialDelta = distanceMatrix[initialSolution[i-1]][initialSolution[i+1]] - distanceMatrix[initialSolution[i]][initialSolution[i-1]] - distanceMatrix[initialSolution[i]][initialSolution[i+1]];

    for(int j = i + 1; j < size-1; j++){
      delta = partialDelta;

      if(j == i + 1){
        delta += distanceMatrix[initialSolution[i]][initialSolution[j+1]] + distanceMatrix[initialSolution[j]][initialSolution[i-1]] - distanceMatrix[initialSolution[j]][initialSolution[j+1]] - distanceMatrix[initialSolution[i-1]][initialSolution[i+1]] + distanceMatrix[initialSolution[i]][initialSolution[i+1]];
      } else {
        delta += distanceMatrix[initialSolution[i]][initialSolution[j]] + distanceMatrix[initialSolution[i]][initialSolution[j+1]] - distanceMatrix[initialSolution[j]][initialSolution[j+1]];
      }

      if(delta < bestNeighbor.bestCost){
        bestNeighbor.iBest = i;
        bestNeighbor.jBest = j;  
        bestNeighbor.bestCost = delta;
      }	
    }
  }

  for(int j = 1; j < size-2; j++){
    partialDelta = -(distanceMatrix[initialSolution[j]][initialSolution[j-1]]);

    for(int i = j + 1; i < size-1; i++){
      delta = partialDelta;

      if(i == j + 1){
        delta += distanceMatrix[initialSolution[j]][initialSolution[i+1]] + distanceMatrix[initialSolution[i]][initialSolution[j-1]] - distanceMatrix[initialSolution[i]][initialSolution[i+1]];
      } else {
        delta += distanceMatrix[initialSolution[i-1]][initialSolution[i+1]] + distanceMatrix[initialSolution[i]][initialSolution[j]] + distanceMatrix[initialSolution[i]][initialSolution[j-1]] - distanceMatrix[initialSolution[i]][initialSolution[i-1]] - distanceMatrix[initialSolution[i]][initialSolution[i+1]];
      }

      if(delta < bestNeighbor.bestCost){
        bestNeighbor.iBest = i;
        bestNeighbor.jBest = j;  
        bestNeighbor.bestCost = delta;
      }	
    }
  }

  return bestNeighbor;
}

neighborInfo twoOpt(vector <int> &initialSolution){
  int size = initialSolution.size();
  double delta, partialDelta;
  
  neighborInfo bestNeighbor;
  bestNeighbor.bestCost = DBL_MAX;

  for(int i = 1; i < size; i++){
    partialDelta = -(distanceMatrix[initialSolution[i-1]][initialSolution[i]]);

    for(int j = i + 1; j < size-1; j++){
      delta = partialDelta;
	            		  		
	    delta += distanceMatrix[initialSolution[i-1]][initialSolution[j]] + distanceMatrix[initialSolution[j+1]][initialSolution[i]] - distanceMatrix[initialSolution[j]][initialSolution[j+1]];
	            		
	    if(delta < bestNeighbor.bestCost){
	      
        bestNeighbor.iBest = i;
        bestNeighbor.jBest = j;
        bestNeighbor.bestCost = delta;
      }
    }
  }

  return bestNeighbor;
}

neighborInfo oropt2(vector <int> &initialSolution){
  int size = initialSolution.size();
  double delta, partialDelta;
  
  neighborInfo bestNeighbor;
  bestNeighbor.bestCost = DBL_MAX;

  for(int i = 1; i < size-2; i++){
    partialDelta = distanceMatrix[initialSolution[i-1]][initialSolution[i+2]] - distanceMatrix[initialSolution[i-1]][initialSolution[i]] - distanceMatrix[initialSolution[i+1]][initialSolution[i+2]];

    for(int j = 1; j <= size-3; j++){
      delta = partialDelta;

      if(i != j){
       
        if(i < j){
          delta += distanceMatrix[initialSolution[j+1]][initialSolution[i]] + distanceMatrix[initialSolution[j+2]][initialSolution[i+1]] - distanceMatrix[initialSolution[j+1]][initialSolution[j+2]];
        } else {
          delta += distanceMatrix[initialSolution[j]][initialSolution[i+1]] + distanceMatrix[initialSolution[j-1]][initialSolution[i]] - distanceMatrix[initialSolution[j]][initialSolution[j-1]];
        }
                  
        if(delta < bestNeighbor.bestCost){

          bestNeighbor.iBest = i;	
          bestNeighbor.jBest = j;
          bestNeighbor.bestCost = delta;
        } 
      }
    }
  }

  return bestNeighbor;
}

neighborInfo oropt3(vector <int> &initialSolution){
  int size = initialSolution.size();
  double delta, partialDelta;
  
  neighborInfo bestNeighbor;
  bestNeighbor.bestCost = DBL_MAX;

  for(int i = 1; i < size-3; i++){
    partialDelta = distanceMatrix[initialSolution[i-1]][initialSolution[i+3]] - distanceMatrix[initialSolution[i-1]][initialSolution[i]] - distanceMatrix[initialSolution[i+2]][initialSolution[i+3]];

    for(int j = 1; j <= size-4; j++){

      if(i != j){
        delta = partialDelta;

	    	if(i < j){
          delta += distanceMatrix[initialSolution[j+2]][initialSolution[i]] + distanceMatrix[initialSolution[j+3]][initialSolution[i+2]] - distanceMatrix[initialSolution[j+2]][initialSolution[j+3]];
		    } else {
          delta += distanceMatrix[initialSolution[j]][initialSolution[i+2]] + distanceMatrix[initialSolution[j-1]][initialSolution[i]] - distanceMatrix[initialSolution[j-1]][initialSolution[j]];
        }
            		
        if(delta < bestNeighbor.bestCost){
	        bestNeighbor.iBest = i;
          bestNeighbor.jBest = j;
          bestNeighbor.bestCost = delta;
	    	} 
      }
    }
  }

  return bestNeighbor;
}

void RVND(vector <int> &solution){
  vector <int> neighborhoods = {0, 1, 2, 3, 4};
  neighborInfo neighbor;

  while(!neighborhoods.empty()){
    int choosen = rand() % neighborhoods.size();

    if(neighborhoods[choosen] == 0){

      neighbor = swap(solution);

      if(neighbor.bestCost < 0){
        int aux = solution[neighbor.iBest];

	      solution[neighbor.iBest] = solution[neighbor.jBest];
	      solution[neighbor.jBest] = aux;

        neighborhoods = {0, 1, 2, 3, 4};

      }else {
        neighborhoods.erase(neighborhoods.begin() + choosen);
      }

    }else if(neighborhoods[choosen] == 1){

      neighbor = reinsertion(solution);

      if(neighbor.bestCost < 0){
        vector <int> solucaoInicial = solution;

        solution.erase(solution.begin()+neighbor.iBest);
        solution.insert(solution.begin()+neighbor.jBest, solucaoInicial[neighbor.iBest]);

        neighborhoods = {0, 1, 2, 3, 4};

      }else {
        neighborhoods.erase(neighborhoods.begin() + choosen);
      }

    }else if(neighborhoods[choosen] == 2){

      neighbor = twoOpt(solution);

      if(neighbor.bestCost < 0){
        int aux, k = neighbor.jBest - neighbor.iBest;

        if(k % 2 != 0){
          k = k + 1;
        }

        for(int q = 0; q < k/2; q++){
          aux = solution[neighbor.iBest + q];
          solution[neighbor.iBest + q] = solution[neighbor.jBest - q];
          solution[neighbor.jBest - q] = aux;
        }

        neighborhoods = {0, 1, 2, 3, 4};

      } else {
        neighborhoods.erase(neighborhoods.begin() + choosen);
      }

    }else if(neighborhoods[choosen] == 3){

      neighbor = oropt2(solution);

      if(neighbor.bestCost < 0){

        if(neighbor.iBest < neighbor.jBest){
          solution.insert(solution.begin() + neighbor.jBest + 2, solution[neighbor.iBest]); 
          solution.insert(solution.begin() + neighbor.jBest + 3, solution[neighbor.iBest+1]); 
          solution.erase(solution.begin() + neighbor.iBest);
          solution.erase(solution.begin() + neighbor.iBest);
        } else {
          solution.insert(solution.begin() + neighbor.jBest, solution[neighbor.iBest]); 
          solution.insert(solution.begin() + neighbor.jBest + 1, solution[neighbor.iBest + 2]); 
          solution.erase(solution.begin() + neighbor.iBest + 2);
          solution.erase(solution.begin() + neighbor.iBest + 2);
        }

        neighborhoods = {0, 1, 2, 3, 4};

      } else {
        neighborhoods.erase(neighborhoods.begin() + choosen);

      }   

    }else if(neighborhoods[choosen] == 4){

      neighbor = oropt3(solution);

      if(neighbor.bestCost < 0){

        if(neighbor.iBest < neighbor.jBest){
          solution.insert(solution.begin() + neighbor.jBest + 3, solution[neighbor.iBest]);
          solution.insert(solution.begin() + neighbor.jBest + 4, solution[neighbor.iBest+1]); 
          solution.insert(solution.begin() + neighbor.jBest + 5, solution[neighbor.iBest+2]);
          solution.erase(solution.begin() + neighbor.iBest);
          solution.erase(solution.begin() + neighbor.iBest);
          solution.erase(solution.begin() + neighbor.iBest);
        } else {
          solution.insert(solution.begin() + neighbor.jBest, solution[neighbor.iBest]);
          solution.insert(solution.begin() + neighbor.jBest + 1, solution[neighbor.iBest + 2]); 
          solution.insert(solution.begin() + neighbor.jBest + 2, solution[neighbor.iBest + 4]); 
          solution.erase(solution.begin() + neighbor.iBest + 3);
          solution.erase(solution.begin() + neighbor.iBest + 3);
          solution.erase(solution.begin() + neighbor.iBest + 3);
        }

        neighborhoods = {0, 1, 2, 3, 4};

      } else {
        neighborhoods.erase(neighborhoods.begin() + choosen);
      }
    }
  }
}

vector <int> pertub(vector <int> solution){
	vector <int> newSolution;
	vector <int> firstSubsequence;
	vector <int> secondSubsequence;
	int i, j, sizeFirst = 0, sizeSecond = 0, size = solution.size();
	
	// Generates subsequence sizes
	if(size < 20){
		sizeFirst = 2;
		sizeSecond = 2;
	} else {
		while(sizeFirst < 2 || sizeFirst > (size/10)) sizeFirst = rand() % size;
		while(sizeSecond < 2 || sizeSecond > (size/10)) sizeSecond = rand() % size; 
	}
	
	// Generates initial position of first subsequence
	i = rand() % (size - sizeFirst);
  while(i == 0) i = rand() % (size - sizeFirst);

	// Generates initial position of second subsequence
	j = rand() % (size - sizeSecond);
	while((j > (i - sizeSecond) && j < (i + sizeFirst)) || j == 0) j = rand() % (size - sizeSecond);

	// Creates vector with first subsequence
	int iter = 0;
	for(int k = 0; k < size; k++){
		if(k >= i){
			firstSubsequence.push_back(solution[k]);
      iter++;
			if(iter == sizeFirst) break;
		}
	}
	
	// Creates vector with second subsequence
	iter = 0;
	for(int k = 0; k < size; k++){
		if(k >= j){
			secondSubsequence.push_back(solution[k]);
      iter++;
			if(iter == sizeSecond) break;
		}
	}

	if(j < i){	
    newSolution = solution;

    // Deletes first subsequence
    int deleted = 0;
    for(int k = 0; k < size; k++){
      if(k >= i){
        newSolution.erase(newSolution.begin() + k - deleted);
        deleted++;
        if(deleted == sizeFirst) break;
      }
    }

    // Deletes second subsequence
    deleted = 0;
    for(int k = 0; k < size; k++){
      if(k >= j){
        newSolution.erase(newSolution.begin() + k - deleted);
        deleted++;
        if(deleted == sizeSecond) break;
      }
    }

    // Inserts second subsequence
    int inserted = 0;
    for(int k = 0; k < secondSubsequence.size(); k++){
      newSolution.insert(newSolution.begin() + i + inserted - sizeSecond, secondSubsequence[k]);
      inserted++;
    }
        
    // Inserts first subsequence
    inserted = 0;
    for(int k = 0; k < firstSubsequence.size(); k++){
      newSolution.insert(newSolution.begin() + j + inserted, firstSubsequence[k]);
      inserted++;
    }
      
	} else {
    newSolution = solution;

    // Deletes second subsequence
    int deleted = 0;
    for(int k = 0; k < size; k++){
      if(k >= j){
        newSolution.erase(newSolution.begin() + k - deleted);
        deleted++;
        if(deleted == sizeSecond) break;
      }
    }

    // Deletes first subsequence
    deleted = 0;
    for(int k = 0; k < size; k++){
      if(k >= i){
        newSolution.erase(newSolution.begin() + k - deleted);
        deleted++;
        if(deleted == sizeFirst) break; 
      }
	  }

    // Inserts first subsequence
    int inserted = 0;
    for(int k = 0; k < firstSubsequence.size(); k++){
      newSolution.insert(newSolution.begin() + j + inserted - sizeFirst, firstSubsequence[k]);
      inserted++;
    }
        
    // Inserts second subsequence
    inserted = 0;
    for(int k = 0; k < secondSubsequence.size(); k++){
      newSolution.insert(newSolution.begin() + i + inserted, secondSubsequence[k]);
      inserted++;
    }
  }
		
	return newSolution;	
}

vector <int> construction(double alpha, vector <int> candidatesList){
  vector <int> solution;

  // Insert depot
  solution.push_back(candidatesList[0]);
  solution.push_back(candidatesList[0]);
  candidatesList.erase(candidatesList.begin());
    
  // Subtours is randomly generated
  for(int i = 0; i < 3; i++){
    int index = rand() % candidatesList.size(); // Choose random node
    solution.insert(solution.begin()+1, candidatesList[index]); 
    candidatesList.erase(candidatesList.begin() + index);
  }

  // Calculates insertion cost of vertices on solution
  while(!candidatesList.empty()){
    vector <insertionInfo> insertionCost((solution.size()-1) * candidatesList.size());

    for(int i = 0, j = 1, k = 0; i < solution.size()-1; i++, j++){
      for(auto l : candidatesList){
        insertionCost[k].cost = distanceMatrix[solution[i]][l] + distanceMatrix[solution[j]][l] - distanceMatrix[solution[i]][solution[j]];
        insertionCost[k].insertedNode = l;
        insertionCost[k].deletedEdge = i;
        k++;
      }
    }

    // Orders insertion costs
    sort(insertionCost.begin(), insertionCost.end(), comparacao);

    int firstElements = alpha * insertionCost.size();
    int index = rand() % firstElements;

    // Inserts choosen node
    solution.insert(solution.begin() + (insertionCost[index].deletedEdge + 1), insertionCost[index].insertedNode);

    // Deletes inserted node from candidates list
    for(int i = 0; i < candidatesList.size(); i++){
      if(candidatesList[i] == insertionCost[index].insertedNode)
        candidatesList.erase(candidatesList.begin()+i);
    }

  }
        
  return solution;
}

double search(int iIls, int dimension){
  double finalCost = DBL_MAX;
  vector <int> finalSolution;
  vector <int> vertices;

  // Creates vector with vertices
  for(int i = 0; i < dimension; i++){
    vertices.push_back(i+1);
  }

  for(int iterMax = 0; iterMax < 50; iterMax++){
    double alpha = (rand() % 90) / 100.0 + 0.1;
    double currentCost, bestCurrentCost = DBL_MAX;
    
    vector <int> currentSolution = construction(alpha, vertices); // Generates initial solution
    vector <int> bestCurrentSolution = currentSolution;

    int iterIls = 0;
    while(iterIls < iIls){

      RVND(currentSolution);
      currentCost = calculateCost(currentSolution);

      if(currentCost < bestCurrentCost){
        bestCurrentSolution = currentSolution;
        bestCurrentCost = calculateCost(bestCurrentSolution);
        iterIls = 0;
      }

      currentSolution = pertub(bestCurrentSolution);
      iterIls++;

    }

    if(bestCurrentCost < finalCost){
      finalSolution = bestCurrentSolution;
      finalCost = bestCurrentCost;
    }
    	  
  }

  cout << endl << "Solution: ";
  for(int i = 0; i < finalSolution.size(); i++){
    cout << finalSolution[i] << " ";
  }

  return finalCost;
  
}

int main(int argc, char** argv) {
 
  clock_t start = clock(); // Starts time counting
    
  readData(argc, argv, &dimension, &distanceMatrix);
  srand(time(NULL));

  int iIls;
    
  if(dimension >= 150){
    iIls = dimension/2;
  } else {
    iIls = dimension;
  }

  double cost = search(iIls, dimension);

  // Ends time counting
  clock_t end = clock();
  double time = ((double) (end - start)) / CLOCKS_PER_SEC;

  cout << endl << "Cost: " << cost << endl;
  cout << "Time: " << time << endl << endl;
    
  return 0;

}
