#include "separation.h"

/********************************************* MAX BACK FUNCTIONS *********************************************/

vector <vector <int>> MaxBack(double ** weight, int dimension) { // Main function of max back heuristic

    vector <vector <int>> sets;
    vector <int> searched, sMin, s;

    int vertex;

    while (searched.size() < dimension) { // Until all vertices are added

        vertex = findVertex(searched, dimension);

        sMin.push_back(vertex);
        s = sMin;

        double cutMin = getCut(weight, s.front(), dimension); // Calculates cut of first vertex
        double cutValue = cutMin;

        vector <double> maxBackValues(dimension);
        getMaxBackValues(maxBackValues, s, weight, dimension); // Calculates max back

        while (s.size() < dimension) { // Adds vertices and update
 
            int maximumMaxBack = getMaximumMaxBack(maxBackValues, s, dimension);

            cutValue += 2 - 2 * maxBackValues[maximumMaxBack];
            updateMaxBack(maxBackValues, s, weight, maximumMaxBack, dimension);
            s.push_back(maximumMaxBack);

            if (cutValue < cutMin) {
                cutMin = cutValue;
                sMin = s;
            }
        }

        if (sMin.size() == dimension) break; 
        sets.push_back(sMin);

        updateSearched(sMin, searched);

    }

    return sets;
}

int findVertex (vector <int> &searched, int dimension) { // Finds first vertex that wasnt searched

    int vertex;

    for (int i = 0; i < dimension; i++) {
            
        bool flag = false;

        for (int j = 0; j < searched.size(); j++) {

            if (i == searched[j]) { 
                flag = true;
                break;
            }  
        }

        if (!flag) {
            vertex = i;
            break;
        }
    }

    return vertex;
}

double getCut (double ** weight, int vertex, int dimension) { // Calculates sum of weights to a single vertex

    double cut = 0;

    for (int i = 0; i < dimension; i++) {

        if (i > vertex) cut += weight[vertex][i];
        else if (i < vertex) cut += weight[i][vertex];
    }

    return cut;
}

void getMaxBackValues (vector <double> &maxBackValues, vector <int> &s, double ** weight, int dimension) {

    // Sum of weights to vertices that doesnt belongs to S
    for (int i = 0; i < dimension; i++) {

        bool flag = false;

        // Verifies if vertex belongs to S
        for (int j = 0; j < s.size(); j++) {

            if (i == s[j]) {
                flag = true;
                break;
            }

        }

        if (!flag) {

            for (int j = 0; j < s.size(); j++) {

                if (i > s[j]) maxBackValues[i] += weight[s[j]][i];
                else if (i < s[j]) maxBackValues[i] += weight[i][s[j]];
            }
        }
    }
}

int getMaximumMaxBack (vector <double> &maxBackValues, vector <int> &s, int dimension) {

    double maximum = __DBL_MIN__;
    int index;

    // Gets maximum max back value that does not belong to S
    for (int i = 0; i < dimension; i++) {

        bool flag = false;

        for (int j = 0; j < s.size(); j++) {

            if (i == s[j]) {
                flag = true;
                break;
            }
        }

        if (maxBackValues[i] > maximum && !flag) {
            maximum = maxBackValues[i];
            index = i;
        }
    }

    return index;
}

void updateMaxBack (vector <double> &maxBackValues, vector <int> &s, double ** weight, int maximumMaxBack, int dimension) {

    // Updates max back values to new vertex added to S
    for (int i = 0; i < dimension; i++) {

        bool flag = false;

        for (int j = 0; j < s.size(); j++) {

            if (i == s[j]) {
                flag = true;
                break;
            }
        }

        if (!flag) {

            if (i > maximumMaxBack) maxBackValues[i] += weight[maximumMaxBack][i];
            else if (i < maximumMaxBack) maxBackValues[i] += weight[i][maximumMaxBack];
        }
    }
}

void updateSearched (vector <int> &s, vector <int> &searched) {

    // Updates searched with vertices added to S
    for (int i = 0; i < s.size(); i++) {

        bool flag = false;

        for (int j = 0; j < searched.size(); j++) {
            if (s[i] == searched[j]) {
                flag = true;
                break;
            }
        }

        if (!flag) {
            searched.push_back(s[i]);
        }
    }
}

/********************************************* MAX BACK FUNCTIONS *********************************************/

/********************************************* MIN CUT FUNCTIONS *********************************************/

vector <vector <int>> MinCut(double ** weight, int dimension) {

    srand (time(NULL));
    int a = rand() % dimension; // Chooses random vertex

    vector <vector <int>> V, sets;
    vector <int> sMin;

    for (int i = 0; i < dimension; i++) { // Creates set to each vertex
        V.push_back ({i});
    }

    while (V.size() > 1) { // While theres more than 1 set

        vector <int> A;

        double cutOfPhase = minCutPhase(V, A, weight, a, dimension);

        int s = A[A.size() - 1];
        int t = A[A.size() - 2];

        for (int i = 0; i < V.size(); i++) {
            if (s == V[i][0]) {
                sMin = V[i];
                break;
            }
        }

        // Merges s and t
        mergeVertices (V, weight, s, t, dimension);

        if (cutOfPhase < 2 - EPSILON) sets.push_back (sMin);

    }

    return sets;

}

double minCutPhase (vector <vector <int>> &V, vector <int> &A, double ** weight, int a, int dimension) {

    A.push_back(a);

    vector <double> weightSumValues(dimension); 

    // Calculates weight
    for (int i = 0; i < dimension; i++) {

        bool flag = false;

        for (int j = 0; j < A.size(); j++) {

            if (i == A[j]) {
                flag = true;
                break;
            }

        }

        if (!flag) {
            if (i > a) weightSumValues[i] += weight[a][i];
            else if (i < a) weightSumValues[i] += weight[i][a];
        }
    }

    int tightVertex;

    while (A.size() < V.size()) {

        double maximum = __DBL_MIN__;
        int tightVertex;

        // Gets most tightly connected vertex
        for (int i = 0; i < dimension; i++) {

            bool flag = false;

            for (int j = 0; j < A.size(); j++) {

                if (i == A[j]) {
                    flag = true;
                    break;
                }
            }

            if (weightSumValues[i] > maximum && !flag) {
                maximum = weightSumValues[i];
                tightVertex = i;
            }
        }

        // Adds on set
        updateMaxBack(weightSumValues, A, weight, tightVertex, dimension);
        A.push_back(tightVertex);

    }

    double cutOfPhase = getCut(weight, tightVertex, dimension); // Cut of vertex added last

    return cutOfPhase;
}

void mergeVertices (vector <vector<int>> &V, double ** weight, int s, int t, int dimension) {

    bool merged = false;
    int index;

    // Merges s and t
    for (int i = 0; i < V.size(); i++) {

        if (s == V[i][0] || t == V[i][0]) {

            if (!merged) {
                index = i;
                merged = true;
            } else {
                V[index].insert(V[index].end(), V[i].begin(), V[i].end()); // Adds vector in the end of another vector
                V.erase(V.begin() + i);
                break;
            }
        }
    }
    
    if (s < t) weight[s][t] = 0;
    else weight[t][s] = 0;

    for (int i = 0; i < V.size(); i++) { // Searches edges to be deleted

        if (t != V[i][0]) {

            if (s < t) {

                if (V[i][0] < s) {
                    weight[V[i][0]][s] += weight[V[i][0]][t];
                    weight[V[i][0]][t] = 0;
                } else if (V[i][0] > s && V[i][0] < t) {
                    weight[s][V[i][0]] += weight[V[i][0]][t];
                    weight[V[i][0]][t] = 0;
                } else if (V[i][0] > t) {
                    weight[s][V[i][0]] += weight[t][V[i][0]];
                    weight[t][V[i][0]] = 0;
                }

            } else {

                if (V[i][0] < t) {
                    weight[V[i][0]][t] += weight[V[i][0]][s];
                    weight[V[i][0]][s] = 0;
                } else if (V[i][0] > t && V[i][0] < s) {
                    weight[t][V[i][0]] += weight[V[i][0]][s];
                    weight[V[i][0]][s] = 0;
                } else if (V[i][0] > s) {
                    weight[t][V[i][0]] += weight[s][V[i][0]];
                    weight[s][V[i][0]] = 0;
                }
            }
        }
    }
}

/********************************************* MIN CUT FUNCTIONS *********************************************/