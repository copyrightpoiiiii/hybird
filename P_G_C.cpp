//
// Created by 张淇 on 2018/8/31.
//

#include "P_G_C.h"

int book_conflict[maxn];

namespace P_P {
    void init_point(int size, vector<int> *b, int num) {
        PP.size = size;
        bool record_point[maxp];
        int rec_poi[maxn];
        vector<int> poi;
        int i = 1;
        while (size--) {
            for (i = 1; i <= num; i++)
                poi.push_back(i);
            memset(record_point, 0, sizeof(record_point));
            while (!poi.empty()) {
                i = rand(0, (int) poi.size() - 1);
                int po = -1, rec_num = inf;
                for (auto j:b[poi[i]]) {
                    //cout<<j<<endl;
                    int rec_num_tmp = 0;
                    for (int k = head[j]; k; k = e[k].next)
                        if (record_point[e[k].go])
                            rec_num_tmp++;
                    if (rec_num_tmp < rec_num) {
                        rec_num = rec_num_tmp;
                        po = j;
                    }
                }
                record_point[po]=true;
                rec_poi[poi[i]] = po;
                poi.erase(poi.begin() + i);
            }
            PP.a[size].insert(PP.a[size].begin(), rec_poi + 1, rec_poi + p + 1);
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
            int min_dex = inf, choose_point = -1, all_dex = inf;
            for (int j = 0; j < a.size(); j++)
                if (choose_side) {
                    int poi = x[a[j]], cnt = 0, cnt2 = 0;
                    for (int k = head[poi]; k; k = e[k].next) {
                        if (choose[e[k].go])
                            cnt++;
                        cnt2++;
                    }
                    if (cnt < min_dex) {
                        min_dex = cnt;
                        all_dex = cnt2;
                        choose_point = j;
                    } else if (cnt == min_dex && cnt2 < all_dex) {
                        all_dex = cnt2;
                        choose_point = j;
                    }
                } else {
                    int poi = y[a[j]], cnt = 0, cnt2 = 0;
                    for (int k = head[poi]; k; k = e[k].next) {
                        if (choose[e[k].go])
                            cnt++;
                        cnt2++;
                    }
                    if (cnt < min_dex) {
                        min_dex = cnt;
                        all_dex = cnt2;
                        choose_point = j;
                    } else if (cnt == min_dex && cnt2 < all_dex) {
                        all_dex = cnt2;
                        choose_point = j;
                    }
                }
            if (choose_point == -1)
                return p;
            else {
                if (choose_side) {
                    choose[x[a[choose_point]]] = true;
                    p[a[choose_point]] = x[a[choose_point]];
                } else {
                    choose[y[a[choose_point]]] = true;
                    p[a[choose_point]] = y[a[choose_point]];
                }
                a.erase(a.begin() + choose_point);
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
        return sum/2;
    }

    vector<int> localSearch(vector<int> a, int iter) {
        vector<int> best_point = a;
        int best_fun = find(a);
        int tabutable[maxn];
        memset(tabutable, 0x3f, sizeof(tabutable));
        bool choose_point[maxn];
        memset(choose_point, 0, sizeof(choose_point));
        for (auto i:a)
            choose_point[i] = true;
        while (iter--) {
            int tl, new_pri = -1, new_pri_f = 0;
            tl = rand(0, A) + (int) (arf * best_fun);
            for (int i = 1; i <= n; i++)
                if (!choose_point[i] && tabutable[i] >= iter) {
                    int cnt = 0;
                    for (int j = head[i]; j; j = e[j].next)
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
                        for (int j = head[i]; j; j = e[j].next)
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
                        i = new_pri;
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
        for (int i = 0; i <= PP.size; i++)
            s_gene[i] = find(PP.a[i]) / (p * p);
        int min_dis[maxn];
        memset(min_dis, 0x3f, sizeof(min_dis));
        for (int i = 0; i < PP.size; i++)
            for (int j = 0; j < PP.size; j++)
                if (i != j)
                    min_dis[i] = min(min_dis[i], dis(PP.a[i], PP.a[j]));
        long double max_index = 0;
        int max_id = PP.size;
        for (int i = 1; i <= PP.size; i++) {
            s_gene[i] += min_dis[i] / p;
            if (s_gene[i] > max_index) {
                max_index = s_gene[i];
                max_id = i;
            }
        }
        PP.a[max_id] = PP.a[PP.size];
        PP.size--;
    }

    vector<int> find_point(int num) {
        for(int i=0;i<PP.size;i++)
            cout<<"init_connect: "<<find(PP.a[i])<<endl;
        int x = rand(0, PP.size - 1), y = rand(0, PP.size - 1);
        while (y == x) {
            y = rand(1, PP.size - 1);
        }
        PP.a[PP.size] = localSearch(crossover(PP.a[x], PP.a[y]), 100);
        cout << "connext: " << find(PP.a[PP.size]) << endl;
        vector<int> tmp = PP.a[PP.size];
        optimize();
        return tmp;
    }

}
