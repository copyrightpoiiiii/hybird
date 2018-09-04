//
// Created by 张淇 on 2018/8/31.
//



#ifndef HYBIRD_CHANGE_PINT_H
#define HYBIRD_CHANGE_PINT_H

#include "PCP.h"


struct point_set {
    int size;
    vector<int> a[maxn];

    point_set() { size = 0; }
} PP;

namespace P_P {


    void init_point(int size, vector<int> *b, int num);

    void crossover(vector<int> x, vector<int> y, vector<int> *p);

    int f(gene p);

    void localSearch(vector<int> *a, int iter);

    int dis(vector<int> s, vector<int> t);

    void optimize();

    vector<int> find_point(int num);


}

#endif //HYBIRD_CHANGE_PINT_H