//
// Created by 张淇 on 2018/9/2.
//

#ifndef HYBIRD_PCP_H
#define HYBIRD_PCP_H

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <ctime>
#include <cmath>
#include <random>

#define inf (int)1e9
#define maxn 2005
#define maxp 2005
#define maxm 400005
#define init_size 10
#define L_LS 500
#define L_check 500
#define E  2.7182818285
#define A 10
#define arf 0.6
using namespace std;

struct edge {
    int go, next;
} e[2 * maxm];

struct grou {
    vector<int> a;
};

struct gene {
    int size;
    grou v[maxn];
    //gene(){size=0;}
} P[30], ans_p;

struct rec_point {
    int color, sum;
} conflict_number[maxn];


int nb_CFL, tot, m, n, p, p2, gene_size, color_size;
int tabutable[maxn][maxn], conflict_color[maxn][maxn];
int head[maxn], p_color_point[maxp], pro[maxn], book_color[maxn];
vector<int> choose_point;
vector<int> con_p[maxp];
default_random_engine e_engine(static_cast<unsigned int>( time(nullptr) ) );
#endif //HYBIRD_PCP_H
