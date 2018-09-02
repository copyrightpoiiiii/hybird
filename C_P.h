//
// Created by 张淇 on 2018/8/31.
//



#ifndef HYBIRD_CHANGE_PINT_H
#define HYBIRD_CHANGE_PINT_H


#include "PCP.h"

struct point_set {
    int size;
    vector<int> a[maxn];
} PP;

namespace C_P {


    void init_point(int size, gene p, vector<int> *b, int num);

    void crossover(vector<int> x, vector<int> y, vector<int> *p);

    int f(gene p);

    void localSearch(vector<int> *a, int iter);

    int dis(gene s, gene t);

    void optimize();

    void find_point(vector<int> *a, int num,point_set t);


}

#endif //HYBIRD_CHANGE_PINT_H