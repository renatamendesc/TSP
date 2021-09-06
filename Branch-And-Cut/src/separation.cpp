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
                maxBack = 0;

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
        // cout << "Max Back de " << i << ": " << maxBackValues[i] << endl;
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

    while (searched.size() != dimension) {

        // cout << "searched.size() != dimension" << endl;

        vector <int> sMin, s;

        int vertex = 0, counter = 0;
        for (int i = 0; i < dimension; i++) {
            for (int j = 0; j < searched.size(); j++) {
                if (i == searched[j]) {

                    if (counter == 0) vertex = i;

                    counter++;
                }
            }
        }

        // cout << "vertice escolhido: " << vertex << endl;

        sMin.push_back(vertex);

        s = sMin;

        double cutMin, cutValue;

        cutMin = getCut(s.front(), weight, dimension);
        cutValue = cutMin;

        vector <double> maxBackValues (dimension);
        getMaxBackValues(maxBackValues, s, weight, dimension);

        while (s.size() < dimension) {

            int maximumMaxBack = getMaximumMaxBack(maxBackValues, s, dimension);

            // cout << "MAIOR: " << maximumMaxBack << endl;

            cutValue += 2 - 2 * maxBackValues[maximumMaxBack];
            s.push_back(maximumMaxBack);

            updateMaxBack (maxBackValues, maximumMaxBack, s, weight, dimension);

            if (cutValue < cutMin) {
                cutMin = cutValue;
                sMin = s;
            }

        }

        searched = sMin;
        sets.push_back(sMin);

        break;

    }

    return sets;

}

double minCutPhase (vector <vector <int>> &V, double ** weight, int a, int dimension) {

    vector <int> A;
    A.push_back (a);

    while (A.size() != V.size()) {

        // Calcular a soma dos pesos

        // verificar maior soma dos pesos

    }

}

vector <vector <int>> MinCut(double ** weight, int dimension) {

    cout << "MIN CUT" << endl;

    srand (time(NULL));
    int a = rand() % dimension;

    vector <vector <int>> V, sets;

    for (int i = 0; i < dimension; i++) {

        V.push_back ({i});

    }

    while (V.size() > 1) {

        double cutOfPhase = minCutPhase (V, weight, a, dimension);

    }

    return vetor;

}