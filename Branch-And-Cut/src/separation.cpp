#include "separation.h"

int getMaximumMaxBack (vector <int> &maxBackValues, int dimension) {

    double maximum = __DBL_MIN__;
    int index;

    for (int i = 0; i < dimension; i++) {
        if (maxBackValues[i] > maximum) {
            maximum = maxBackValues[i];
            index = i;
        }
    }

    return index;

}

void getMaxBackValues (vector <int> &maxBackValues, vector <int> s, double ** weight, int dimension) {

    // Calcular os valores do peso para vertices que não pertencem a S
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < s.size(); j++) {

            if (i == s[j]) {
                break;
            }

            maxBackValues[i] = getCut(i, weight, dimension);

        }
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

vector <vector <int>> MaxBack(double ** weight, int dimension) {

    cout << "Max back" << endl;

    // Zera valores muito próximos a zero
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {

            // if (weight[i][j] < EPSILON && weight[i][j] > -EPSILON) {
            //     weight[i][j] = 0;
            // }

        }
    }

    vector <vector <int>> sets;
    vector <int> searched;

    while (searched.size() != dimension) {

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

        sMin.push_back(vertex);

        s = sMin;

        double cutMin, cutValue;

        cutMin = getCut(s.front(), weight, dimension);
        cutValue = cutMin;

        vector <int> maxBackValues (dimension);
        getMaxBackValues(maxBackValues, s, weight, dimension);

        while (s.size() != dimension) {

            int maximumMaxBack = getMaximumMaxBack(maxBackValues, dimension);

            cutValue += 2 - 2 * maxBackValues[maximumMaxBack];
            s.push_back(maximumMaxBack);

            maxBackValues.clear();
            getMaxBackValues(maxBackValues, s, weight, dimension);

            if (cutValue < cutMin) {
                cutMin = cutValue;
                sMin = s;
            }

        }

        if (sMin.size() == dimension) break;

        searched = sMin;
        sets.push_back(sMin);

    }

    cout << "Quantidade conjuntos: " << sets.size() << endl;

    for (int i = 0; i < sets.size(); i++) {
        cout << "Conjunto " << i << ": " << endl;
        for (int j = 0; j < sets[i].size(); j++) {
            cout << sets[i][j] << " " << endl;
        }
    }

    return sets;

}

vector <vector <int>> MinCut(double ** weight, int dimension) {

    cout << "Min cut" << endl;

    vector <vector <int>> vetor;

    return vetor;

}