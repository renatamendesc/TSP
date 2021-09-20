#include "separation.h"

int getMaximumMaxBack (vector <double> &maxBackValues, vector <int> &s, int dimension) {

    double maximum = __DBL_MIN__;
    int index;

    for (int i = 0; i < dimension; i++) {

        bool flag = false;

        // cout << maxBackValues[i] << " " << endl;
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

void getMaxBackValues (vector <double> &maxBackValues, vector <int> &s, double ** weight, int dimension) {

    // Calcular os valores do peso para vertices que não pertencem a S
    for (int i = 0; i < dimension; i++) {

        double maxBack = 0;
        bool flag = false;

        // Verifies if vertex belongs to S
        for (int j = 0; j < s.size(); j++) {

            if (i == s[j]) {
                flag = true;
                break;
            }

        }

        // if vertex don't belong to S
        if (!flag) {

            for (int j = 0; j < s.size(); j++) {

                if (i > s[j]) maxBack += weight[s[j]][i];
                else if (i < s[j]) maxBack += weight[i][s[j]];

            }

        }

        maxBackValues[i] = maxBack;

    }

}

double getCut (int vertex, double ** weight, int dimension) {

    double cut = 0;

    for (int i = 0; i < dimension; i++) {

        if (i > vertex) cut += weight[vertex][i];
        else if (i < vertex) cut += weight[i][vertex];

    }

    // cout << "CUT: " << cut << endl;

    return cut;

}

void updateMaxBack (vector <double> &maxBackValues, int maximumMaxBack, vector <int> &s, double ** weight, int dimension) {

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

vector <vector <int>> MaxBack(double ** weight, int dimension) {

    cout << "MAX BACK" << endl;

    vector <vector <int>> sets;
    vector <int> searched;

    int vertex, iter = 0;

    while (searched.size() < dimension) {

        vector <int> sMin, s;

        for (int i = 0; i < dimension; i++) {
            
            bool flag = false;

            for (int j = 0; j < searched.size(); j++) {

                if (i == searched[j]) flag = true;
                
            }

            if (!flag) {
                vertex = i;
                break;
            }

        }

        sMin.push_back (vertex);
        s = sMin;

        double cutMin = getCut (s.front(), weight, dimension);
        double cutValue = cutMin;

        vector <double> maxBackValues (dimension);
        getMaxBackValues (maxBackValues, s, weight, dimension);

        while (s.size() < dimension) {

            int maximumMaxBack = getMaximumMaxBack (maxBackValues, s, dimension);

            cutValue += 2 - 2 * maxBackValues[maximumMaxBack];

            updateMaxBack (maxBackValues, maximumMaxBack, s, weight, dimension);
            s.push_back (maximumMaxBack);

            if (cutValue < cutMin) {
                cutMin = cutValue;
                sMin = s;
            }

        }

        if (sMin.size() == dimension) break; 

        sets.push_back (sMin);

        for (int i = 0; i < sMin.size(); i++) {

            bool flag = false;

            for (int j = 0; j < searched.size(); j++) {
                if (sMin[i] == searched[j]) {
                    flag = true;
                    break;
                }
            }

            if (!flag) {
                searched.push_back (sMin[i]);
            }

        }

    }

    return sets;

}

void getWeightSum (vector <double> &weightSumValues, double ** weight, int dimension) {

    int a = weightSumValues.front();

    for (int i = 0; i < dimension; i++) {

        if (i > a) weightSumValues[i] += weight[a][i];
        else if (i < a) weightSumValues[i] += weight[i][a];

    }

}

double minCutPhase (vector <vector <int>> &V, vector <int> &A, double ** weight, int a, int dimension) {

    A.push_back (a);

    vector <double> weightSumValues (dimension);
    getWeightSum (weightSumValues, weight, dimension); // Calculate weight

    int tightVertex;

    while (A.size() < V.size()) {

        tightVertex = getMaximumMaxBack (weightSumValues, A, dimension);
        updateMaxBack (weightSumValues, tightVertex, A, weight, dimension);
        A.push_back (tightVertex);

    }

    double cutOfPhase = getCut (tightVertex, weight, dimension); // Cut of vertex added last

    return cutOfPhase;

}

void mergeVertices (vector <vector<int>> &V, vector <int> &A, double ** weight, int s, int t, int dimension) {

    bool merged = false;

    // Merges s and t
    for (int i = 0; i < V.size(); i++) {

        // Elemento se junta ao conjunto do de menor índice
        if (s == V[i][0]) {

            if (!merged) {
                V[i].push_back (t);
                merged = true;
            } else {
                V.erase (V.begin() + i);
                break;
            }

        } else if (t == V[i][0]) {

            if (!merged) {
                V[i].push_back (s);
                merged = true;
            } else {
                V.erase (V.begin() + i);
                break;
            }

        }

    }

    if (s < t) weight[s][t] = 0;
    else weight[t][s] = 0;

    for (int i = 0; i < V.size(); i++) { // Procura arestas que serão apagadas

        if (t != V[i][0]) {

            // Varia casos dependendo de qual o maior indice
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

vector <vector <int>> MinCut(double ** weight, int dimension) {

    cout << "MIN CUT" << endl;

    srand (time(NULL));
    int a = rand() % dimension;

    vector <vector <int>> V, sets;
    vector <int> minCutSet;

    for (int i = 0; i < dimension; i++) {
        V.push_back ({i});
    }

    double minCut = __DBL_MAX__;
    int cut;

    while (V.size() > 1) {

        vector <int> A;

        double cutOfPhase = minCutPhase (V, A, weight, a, dimension);

        int s = A[A.size() - 1];
        int t = A[A.size() - 2];

        if (cutOfPhase < minCut) {
            minCut = cutOfPhase;
            cut = s;

            for (int i = 0; i < V.size(); i++) {
                if (cut == V[i][0]) {
                    minCutSet = V[i];
                    break;
                }
            }
        }

        // for (int i = 0; i < V.size(); i++) {
        //     if (s == V[i][0]) {
        //         minCutSet = V[i];
        //         break;
        //     }
        // }

        sets.push_back(minCutSet);

        // Fazer o merge entre s e t
        mergeVertices (V, A, weight, s, t, dimension);

        // break;

    }

    return sets;

}