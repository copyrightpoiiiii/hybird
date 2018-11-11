//
// Created by 张淇 on 2018/8/31.
//



#ifndef HYBIRD_CHANGE_PINT_H
#define HYBIRD_CHANGE_PINT_H

#include "Partition_Coloring_Problem.h"

extern point_set Parition_Point_Set;
extern rec_point point_tot_conflicts[maxn];
extern edge e[2 * maxm];
extern gene P[30], ans_p;
extern int nb_CFL, tot, m, n, partition_size, partition_size, gene_size, color_size;
extern int tabutable[maxn][maxn], conflict_color[maxn][maxn];
extern int head[maxn], p_color_point[maxp], partition_vertex_by_vertex[maxn], book_color[maxn];
extern vector<int> choose_point;
extern vector<int> partition_vertex_by_group[maxp];
extern int array_form_1_to_n[maxn];
extern bool choose_point_bool[maxn];

extern int rand(int a, int b);

extern pair<int, int> find_connect_point(int x);

namespace Partition_Problem {


    void init_point_to_color (int size, vector<int> *b, int num);

    void crossover(vector<int> x, vector<int> y, vector<int> *p);

    int f(gene p);

    void localSearch(vector<int> *a, int iter);

    int dis(vector<int> s, vector<int> t);

    void optimize();

    vector<int> find_point(int num);

    vector<int> find_point_old(int num);

}

#endif //HYBIRD_CHANGE_PINT_H