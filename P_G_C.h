//
// Created by 张淇 on 2018/8/31.
//



#ifndef HYBIRD_CHANGE_PINT_H
#define HYBIRD_CHANGE_PINT_H

#include "PCP.h"

extern point_set PP;
extern rec_point conflict_number[maxn];
extern edge e[2 * maxm];
extern gene P[30], ans_p;
extern int nb_CFL, tot, m, n, p, p2, gene_size, color_size;
extern int tabutable[maxn][maxn], conflict_color[maxn][maxn];
extern int head[maxn], p_color_point[maxp], pro[maxn], book_color[maxn];
extern vector<int> choose_point;
extern vector<int> con_p[maxp];
extern int array_form_1_to_n[maxn];
extern bool choose_point_bool[maxn];

extern int rand(int a, int b);

extern pair<int, int> find_connect_point(int x);

namespace P_P {


    void init_point(int size, vector<int> *b, int num);

    void crossover(vector<int> x, vector<int> y, vector<int> *p);

    int f(gene p);

    void localSearch(vector<int> *a, int iter);

    int dis(vector<int> s, vector<int> t);

    void optimize();

    vector<int> find_point(int num);

    vector<int> find_point_old(int num);

}

#endif //HYBIRD_CHANGE_PINT_H