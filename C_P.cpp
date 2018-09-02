//
// Created by 张淇 on 2018/8/31.
//

#include "C_P.h"

int book_conflict[maxn];

using namespace C_P;

void init_point(int size, gene p, vector<int> *b, int num) {
    PP.size = size;
    while (size--) {
        for (int i = 1; i <= num; i++) {
            default_random_engine e;
            e.seed((unsigned) time(0));
            uniform_int_distribution<unsigned long> u(1, b[i].size());
            PP.a[size].push_back(b[i][u(e)]);
        }
    }
}

vector<int> crossover(vector<int> x, vector<int> y) {
    default_random_engine e1;
    e1.seed((unsigned) time(NULL));
    uniform_int_distribution<bool> u(false, true);
    bool choose[maxn];
    int choose_side = 0;
    memset(choose, 0, sizeof(choose));
    vector<int> a, p;
    a.clear();
    for (int i = 0; i < x.size(); i++) {
        a.push_back(i);
        p.push_back(0);
    }
    for (int i = 0; i < x.size(); i++) {
        int min_dex = inf, choose_point = -1;
        for (int j = 0; j < a.size(); j++)
            if (choose_side) {
                int poi = x[j], cnt = 0;
                for (int k = head[poi]; k; k = e[k].next)
                    if (choose[e[k].go])
                        cnt++;
                if (cnt < min_dex) {
                    min_dex = cnt;
                    choose_point = j;
                }
            } else {
                int poi = y[j], cnt = 0;
                for (int k = head[poi]; k; k = e[k].next)
                    if (choose[e[k].go])
                        cnt++;
                if (cnt < min_dex) {
                    min_dex = cnt;
                    choose_point = j;
                }
            }
        if (choose_point == -1)
            return p;
        else {
            a.erase(a.begin() + choose_point);
            if (choose_side) {
                choose[x[choose_point]] = true;
                p[choose_point] = x[choose_point];
            } else {
                choose[y[choose_point]] = true;
                p[choose_point] = y[choose_point];
            }
            choose_side ^= 1;
        }
    }
    return p;
}

int find(vector<int> a) {
    int sum = 0;
    memset(choose_point, 0, sizeof(choose_point));
    memset(book_conflict, 0, sizeof(book_conflict));
    for (int i = 0; i < a.size(); i++)
        choose_point[a[i]] = true;
    for (int j = 0; j < a.size(); j++)
        for (int i = head[a[j]]; i; i = e[i].next)
            if (choose_point[e[i].go]) {
                book_conflict[pro[a[j]]]++;
                book_conflict[pro[e[i].go]]++;
                sum++;
            }
    return sum;
}

vector<int> localSearch(vector<int> a, int iter) {
    vector best_point = a;
    int best_fun = find(a);
    int tabutable[maxn];
    memset(tabutable, 0x3f, sizeof(tabutable));
    for (int i = 0; i < a.size(); i++)
        choose_point[a[i]] = true;
    while (iter--) {
        int tl, new_pri = -1, new_pri_f = 0;
        tl = rand() % A + arf * nb_CFL;
        for (int i = 1; i <= n; i++)
            if (!choose_point[i] && tabutable[i] >= iter) {
                int cnt = 0;
                for (int j = head[i]; j; j = e[j].go)
                    if (choose_point[e[j].go])
                        cnt++;
                if (book_conflict[pro[i]] - cnt > new_pri_f) {
                    new_pri = i;
                    new_pri_f = book_conflict[pro[i]] - cnt;
                }
            }
        if (new_pri == -1)
            return best_point;
        else {
            for (int i = 0; i < a.size(); i++)
                if (pro[a[i]] == pro[new_pri]) {
                    for (int j = head[a[i]]; j; j = e[j].next)
                        if (choose_point[e[j].go]) {
                            best_fun--;
                            book_conflict[pro[e[j].go]]--;
                        }
                    book_conflict[pro[a[i]]] = 0;
                    for (int j = head[new_pri]; j; j = e[j].next)
                        if (choose_point[e[j].go]) {
                            best_fun--;
                            book_conflict[pro[e[j].go]]++;
                            book_conflict[pro[new_pri]]++;
                        }
                    tabutable[a[i]] = iter - tl;
                }
        }
    }

    return best_point;
}


vector<int> find_point(int num, point_set t) {
    default_random_engine e;
    e.seed((unsigned) time(NULL));
    uniform_int_distribution<int> u(1, t.size);
    int x = u(e), y = u(e);
    while (y == x) {
        y = u(e);
    }
    return localSearch(crossover(t.a[x], t.a[y]),L_LS);
}

