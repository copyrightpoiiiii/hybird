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
#include <algorithm>

#define inf (int)1e9
#define maxn 2005
#define maxp 2005
#define maxm 400005
#define POPULATION_SIZE 10
#define GCP_LOCAL_SEARCH_ITER 700
#define CHEAK_ITERATIONS 700
#define E  2.7182818285
#define PCP_LOCAL_SEARCH_ITER 200
#define A 10
#define arf 0.6
#define Endl endl
#define ednl endl
#define extren extern
#define DEBUG(x) cout<<x<<endl
using namespace std;

struct edge {
    int go, next;
};

struct grou {
    vector<int> a;

    grou() {
        a.clear();
    };
};

struct gene {
    int size;
    grou v[maxn];

    gene() {
        size = 0;
    }
};

struct rec_point {
    int color, sum;
};

struct point_set {
    int size;
    vector<int> a[maxn];

    point_set() { size = 0; }
};

#endif //HYBIRD_PCP_H
