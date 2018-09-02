//
// Created by 张淇 on 2018/9/2.
//
#include "PCP.h"

#ifndef HYBIRD_P_P_H
#define HYBIRD_P_P_H
namespace P_P {
    void init_gen(int lim, const int size);

    void crossover(gene s1, gene s2, gene &s);

    bool judge(gene p);

    int f(gene p);

    void find(gene p);

    void localSearch(gene &p, int iter);

    int dis(gene s, gene t);

    void optimize();

    int find_change_point(int *a, int x);

    int find_change_point_real(int *a, int x);

    void choose_all_point(int *a, int mode);

    bool check(int x);
}
#endif //HYBIRD_P_P_H