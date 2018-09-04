//
// Created by 张淇 on 2018/8/31.
//

#include "P_G_C.h"

int book_conflict[maxn];

namespace P_P {
    void init_point(int size, vector<int> *b, int num) {
        PP.size = size;
        while (size--) {
            for (int i = 1; i <= num; i++)
                PP.a[size].push_back(b[i][rand(1, (int) b[i].size())]);
        }
    }

    vector<int> crossover(vector<int> x, vector<int> y) {
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
        bool choose_point[maxn];
        memset(choose_point, 0, sizeof(choose_point));
        memset(book_conflict, 0, sizeof(book_conflict));
        for (auto i:a)
            choose_point[i] = true;
        for (auto j:a)
            for (int i = head[j]; i; i = e[i].next)
                if (choose_point[e[i].go]) {
                    book_conflict[pro[j]]++;
                    book_conflict[pro[e[i].go]]++;
                    sum++;
                }
        return sum;
    }

    vector<int> localSearch(vector<int> a, int iter) {
        vector<int> best_point = a;
        int best_fun = find(a);
        int tabutable[maxn];
        memset(tabutable, 0x3f, sizeof(tabutable));
        for (auto i:a)
            choose_point[i] = true;
        while (iter--) {
            int tl, new_pri = -1, new_pri_f = 0;
            tl = rand(0, A) + (int) (arf * best_fun);
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
                for (auto i:a)
                    if (pro[i] == pro[new_pri]) {
                        for (int j = head[a[i]]; j; j = e[j].next)
                            if (choose_point[e[j].go]) {
                                best_fun--;
                                book_conflict[pro[e[j].go]]--;
                            }
                        book_conflict[pro[i]] = 0;
                        for (int j = head[new_pri]; j; j = e[j].next)
                            if (choose_point[e[j].go]) {
                                best_fun--;
                                book_conflict[pro[e[j].go]]++;
                                book_conflict[pro[new_pri]]++;
                            }
                        tabutable[i] = iter - tl;
                    }
            }
        }
        return best_point;
    }

    int dis(vector<int> x, vector<int> y) {
        int cnt = 0;
        for (int i = 0; i < x.size(); i++)
            if (x[i] != y[i])
                cnt++;
        return cnt;
    }

    void optimize() {
        long double s_gene[PP.size + 5];
        for (int i = 1; i <= PP.size; i++)
            s_gene[i] = find(PP.a[i]);
        int min_dis[maxn];
        memset(min_dis, 0x3f, sizeof(min_dis));
        for (int i = 1; i <= PP.size; i++)
            for (int j = 1; j <= PP.size; j++)
                if (i != j)
                    min_dis[i] = min(min_dis[i], dis(PP.a[i], PP.a[j]));
        long double max_index = 0;
        int max_id = PP.size;
        for (int i = 1; i <= PP.size; i++) {
            s_gene[i] += pow(E, (long double) 0.08 * p * (long double) p / min_dis[i]);
            if (s_gene[i] > max_index) {
                max_index = s_gene[i];
                max_id = i;
            }
        }
        PP.a[max_id] = PP.a[PP.size];
        PP.size--;
    }

    vector<int> find_point(int num) {
        default_random_engine e;
        e.seed((unsigned) time(nullptr));
        uniform_int_distribution<int> u(1, PP.size);
        int x = u(e), y = u(e);
        while (y == x) {
            y = u(e);
        }
        PP.a[++PP.size] = localSearch(crossover(PP.a[x], PP.a[y]), L_LS);
        vector<int> tmp = PP.a[PP.size];
        optimize();
        return tmp;
    }

}
