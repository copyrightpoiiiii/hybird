//
// Created by 张淇 on 2018/9/2.
//


#ifndef HYBIRD_P_P_H
#define HYBIRD_P_P_H

#include "Partition_Coloring_Problem.h"

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

extern void output_gene(gene p);

extern pair<int, int> find_connect_point(int x);

namespace Graph_Coloring_Problem {
    void init_gen(int lim, int size);

    void crossover(gene s1, gene s2, gene &s);

    bool judge(gene p);

    int f(gene p);

    void find(gene p);

    void localSearch(gene &p, int iter);

    int dis(gene s, gene t);

    void optimize();

    bool graph_color(vector<int> point_set,int color_number);
}
#endif //HYBIRD_P_P_H
