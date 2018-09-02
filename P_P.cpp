//
// Created by 张淇 on 2018/9/2.
//
#include "P_P.h"

void init_gen(int lim, const int size) {
    for (int i = 1; i <= gene_size; i++) {
        for (int j = 1; j <= P[i].size; j++)
            P[i].v[j].a.clear();
    }
    bool vispoint[maxn];
    gene_size = size;
    vector<int> book_point;
    for (int i = 1; i <= size; i++) {
        book_point.clear();
        for (int j = 1; j <= p; j++)
            book_point.push_back(j);
        P[i].size = 0;
        for (int j = 1; j <= p; j++) {
            int t = rand() % book_point.size();
            int x = choose_point[book_point[t]];
            book_point.erase(book_point.begin() + t);
            memset(vispoint, 0, sizeof(vispoint));
            for (int k = head[x], y; k; k = e[k].next) {
                y = e[k].go;
                vispoint[y] = 1;
            }
            int flag = 0;
            for (int k = 1; k <= P[i].size; k++) {
                int flaggro = 0;
                for (int t = 0; t < P[i].v[k].a.size(); t++) {
                    if (vispoint[P[i].v[k].a[t]]) {
                        flaggro = 1;
                        break;
                    }
                }
                if (!flaggro) {
                    P[i].v[k].a.push_back(x);
                    flag = 1;
                    break;
                }
            }
            if (!flag) {
                if (P[i].size < lim) {
                    P[i].size++;
                    P[i].v[P[i].size].a.push_back(x);
                } else {
                    int to_pri = rand() % lim + 1;
                    P[i].v[to_pri].a.push_back(x);
                }
            }
        }
    }
}


void crossover(gene s1, gene s2, gene &s) {
    int book[maxn], book_point[maxn];
    memset(book_point, 0, sizeof(book_point));
    s.size = s1.size;
    for (int i = 1; i <= s1.size; i++) {
        memset(book, 0, sizeof(book));
        int maxnum = 0, maxid = 1;
        if (i % 2 == 1) {
            for (int j = 1; j <= s1.size; j++)
                if (s1.v[j].a.size() > maxnum) {
                    maxnum = s1.v[j].a.size();
                    maxid = j;
                }
            s.v[i].a.swap(s1.v[maxid].a);
            for (int j = 0; j < s.v[i].a.size(); j++) {
                book[s.v[i].a[j]] = 1;
                book_point[s.v[i].a[j]] = 1;
            }
            gene news2;
            for (int j = 1; j <= s2.size; j++) {
                for (int k = 0, y; k < s2.v[j].a.size(); k++)
                    if (!book[y = s2.v[j].a[k]])
                        news2.v[j].a.push_back(y);
                s2.v[j].a.swap(news2.v[j].a);
            }
        } else {
            for (int j = 1; j <= s2.size; j++)
                if (s2.v[j].a.size() > maxnum) {
                    maxnum = s2.v[j].a.size();
                    maxid = j;
                }
            s.v[i].a.swap(s2.v[maxid].a);
            for (int j = 0; j < s.v[i].a.size(); j++) {
                book[s.v[i].a[j]] = 1;
                book_point[s.v[i].a[j]] = 1;
            }
            gene news1;
            for (int j = 1; j <= s1.size; j++) {
                for (int k = 0, y; k < s1.v[j].a.size(); k++)
                    if (!book[y = s1.v[j].a[k]])
                        news1.v[j].a.push_back(y);
                s1.v[j].a.swap(news1.v[j].a);
            }
        }
    }
    for (int i = 1; i <= p; i++)
        if (!book_point[choose_point[i]]) {
            int x = rand() % s.size + 1;
            s.v[x].a.push_back(choose_point[i]);
        }
}

bool judge(gene p) {
    int color[maxn];
    for (int i = 1; i <= p.size; i++)
        for (int j = 0; j < p.v[i].a.size(); j++)
            color[p.v[i].a[j]] = i;
    for (int i = 1; i <= p2; i++)
        for (int j = head[choose_point[i]]; j; j = e[j].next)
            if (color[e[j].go] == color[choose_point[i]])
                return 0;
    return 1;
}

int f(gene p) {
    int ans = 0;
    int color[maxn];
    for (int i = 1; i <= p.size; i++)
        for (int j = 0; j < p.v[i].a.size(); j++)
            color[p.v[i].a[j]] = i;
    for (int i = 1; i <= p2; i++)
        for (int j = head[choose_point[i]]; j; j = e[j].next)
            if (color[e[j].go] == color[choose_point[i]])
                ans++;
    return ans / 2;
}

void find(gene p) {
    int color[maxn];
    nb_CFL = 0;
    memset(conflict_color, 0, sizeof(conflict_color));
    for (int i = 1; i <= p.size; i++)
        for (int j = 0; j < p.v[i].a.size(); j++) {
            color[p.v[i].a[j]] = i;
            conflict_number[p.v[i].a[j]].color = i;
        }
    for (int i = 1; i <= p2; i++) {
        int x = choose_point[i];
        conflict_number[x].sum = 0;
        for (int j = head[x]; j; j = e[j].next) {
            conflict_color[x][color[e[j].go]]++;
            if (color[e[j].go] == color[x])
                conflict_number[x].sum++;
        }
        nb_CFL += conflict_number[x].sum;
    }
    nb_CFL /= 2;
}

void localSearch(gene &p, int iter) {
    gene best_res = p;
    find(p);
    int best_fun = nb_CFL;
    memset(tabutable, 0x3f, sizeof(tabutable));
    while (iter--) {
        int tl, new_pri = -1, new_pri_f = n * n;
        tl = rand() % A + arf * nb_CFL;
        int color_change, rec_id = 1;
        for (int i = 1; i <= p2; i++)
            if (conflict_number[choose_point[i]].sum > 0) {
                for (int j = 1; j <= p.size; j++)
                    if (j != conflict_number[choose_point[i]].color && tabutable[choose_point[i]][j] >= iter) {
                        if (conflict_number[choose_point[i]].sum - conflict_color[choose_point[i]][j] >
                            conflict_number[rec_id].sum - new_pri_f) {
                            new_pri_f = conflict_color[choose_point[i]][j];
                            new_pri = j;
                            rec_id = choose_point[i];
                        }
                    }
            }
        if (new_pri == -1) {
            p = best_res;
            return;
        } else {
            color_change = conflict_number[rec_id].color;
            for (int j = 0; j < p.v[color_change].a.size(); j++)
                if (p.v[color_change].a[j] == rec_id)
                    p.v[color_change].a.erase(p.v[color_change].a.begin() + j);
            nb_CFL -= conflict_number[rec_id].sum;
            nb_CFL += new_pri_f;
            for (int j = head[rec_id]; j; j = e[j].next) {
                if (conflict_number[e[j].go].color == color_change)
                    conflict_number[e[j].go].sum--;
                else if (conflict_number[e[j].go].color == new_pri)
                    conflict_number[e[j].go].sum++;
                conflict_color[e[j].go][color_change]--;
                conflict_color[e[j].go][new_pri]++;
            }
            if (nb_CFL <= best_fun) {
                best_fun = nb_CFL;
                best_res = p;
            }
            tabutable[rec_id][color_change] = iter - tl;
            conflict_number[rec_id].sum = new_pri_f;
            conflict_number[rec_id].color = new_pri;
            p.v[new_pri].a.push_back(rec_id);
            if (!nb_CFL)
                return;
        }
    }
    p = best_res;
}

int dis(gene s, gene t) {
    int color[maxn];
    int sum = 0;
    for (int i = 1; i <= t.size; i++)
        for (int j = 0; j < t.v[i].a.size(); j++)
            color[t.v[i].a[j]] = i;
    for (int i = 1; i <= s.size; i++) {
        int rec_num[maxn], max_rec = 0;
        memset(rec_num, 0, sizeof(rec_num));
        for (int j = 0; j < s.v[i].a.size(); j++) {
            rec_num[color[s.v[i].a[j]]]++;
            max_rec = max(max_rec, rec_num[color[s.v[i].a[j]]]);
        }
        sum += max_rec;
    }
    return p - sum;
}

void optimize() {
    long double s_gene[gene_size + 5];
    for (int i = 1; i <= gene_size; i++)
        s_gene[i] = f(P[i]);
    int min_dis[maxn];
    memset(min_dis, 0x3f, sizeof(min_dis));
    for (int i = 1; i <= gene_size; i++)
        for (int j = 1; j <= gene_size; j++)
            if (i != j) {
                min_dis[i] = min(min_dis[i], dis(P[i], P[j]));
            }
    long double max_index = 0;
    int max_id = gene_size;
    for (int i = 1; i <= gene_size; i++) {
        s_gene[i] += pow(E, (long double) 0.08 * p * (long double) p / min_dis[i]);
        if (s_gene[i] > max_index) {
            max_index = s_gene[i];
            max_id = i;
        }
    }
    P[max_id] = P[gene_size];
    gene_size--;
}

int find_change_point(int *a, int x) {
    int max_num = 0, fin_point = rand() % n + 1;
    for (int i = 1; i <= p; i++) {
        int new_num = m;
        for (int j = 0; j < con_p[i].size(); j++)
            if (con_p[i][j] != choose_point[i] && a[con_p[i][j]] >= x) {
                new_num = 0;
                for (int k = head[con_p[i][j]]; k; k = e[k].next)
                    if (pro[e[k].go] != i)
                        new_num++;
                if (new_num < max_num) {
                    max_num = new_num;
                    fin_point = con_p[i][j];
                }
            }
    }
    return fin_point;
}

int find_change_point_real(int *a, int x) {
    int max_num = 0, fin_point = rand() % n + 1;
    for (int i = 1; i <= p; i++) {
        int old_num = 0, new_num;
        for (int j = head[choose_point[i]]; j; j = e[j].next)
            old_num++;
        for (int j = 0; j < con_p[i].size(); j++)
            if (con_p[i][j] != choose_point[i] && a[con_p[i][j]] >= x) {
                new_num = 0;
                for (int k = head[con_p[i][j]]; k; k = e[k].next)
                    if (pro[e[k].go] != i)
                        new_num++;
                if (old_num - new_num > max_num) {
                    max_num = old_num - new_num;
                    fin_point = con_p[i][j];
                }
            }
    }
    return fin_point;
}

void choose_all_point(int *a, int mode) {
    if (mode == 1) {
        memset(point_choose, 0, sizeof(point_choose));
        for (int i = 1; i <= p; i++) {
            int min_con = n, min_id = con_p[i][rand() % con_p[i].size()];
            for (int j = 0; j < con_p[i].size(); j++) {
                int cnt = 0;
                for (int k = head[con_p[i][j]]; k; k = e[k].next)
                    if (pro[e[k].go] != i)
                        cnt++;
                if (cnt < min_con) {
                    min_con = cnt;
                    min_id = con_p[i][j];
                }
            }
            point_choose[min_id] = true;
            a[i] = min_id;
        }
    } else {
        memset(point_choose, 0, sizeof(point_choose));
        for (int i = 1; i <= p; i++) {
            int min_con = n, min_id = con_p[i][rand() % con_p[i].size()];
            for (int j = 0; j < con_p[i].size(); j++) {
                int cnt = 0;
                for (int k = head[con_p[i][j]]; k; k = e[k].next)
                    if (pro[e[k].go] != i)
                        cnt++;
                if (abs(cnt - min_con) < (p / 20) || min_con - cnt > (p / 5)) {
                    min_con = cnt;
                    min_id = con_p[i][j];
                }
            }
            point_choose[min_id] = true;
            a[i] = min_id;
        }
    }
}

bool check(int x) {
    memset(point_choose, 0, sizeof(point_choose));
    choose_all_point(choose_point, 1);
    //cout<<"sss"<<endl;
    int stop_check = L_check;
    int last_opti = L_check;
    int tabu_table_point[maxn];
    int best_answer = p * p;
    memset(tabu_table_point, 0x3f, sizeof(tabu_table_point));
    while (stop_check--) {
        init_gen(x, init_size);
        int stop_cond = L_check;
        for (int i = 1; i <= init_size; i++)
            if (judge(P[i])) {
                ans_p = P[i];
                return 1;
            }
        int good_answer = p * p;
        while (stop_cond--) {
            int p1 = rand() % init_size + 1, p2 = rand() % init_size + 1;
            gene ps;
            crossover(P[p1], P[p2], ps);
            localSearch(ps, L_LS);
            int tmp = f(ps);
            good_answer = min(good_answer, tmp);
            cout << tmp << endl;
            if (abs(tmp - good_answer) < 50 && tmp > 50 && L_check - stop_cond > 200)
                break;
            if (judge(ps)) {
                ans_p = ps;
                cout << "I got it!" << endl;
                cout << L_check - stop_cond << endl;
                return 1;
            }
            P[++gene_size] = ps;
            optimize();
        }
        cout << "I will change a point!" << endl;
        int change_point = find_change_point(tabu_table_point, stop_check);
        point_choose[choose_point[pro[change_point]]] = false;
        point_choose[change_point] = true;
        choose_point[pro[change_point]] = change_point;
        int nb_CFL = 0;
        for (int i = head[change_point]; i; i = e[i].next)
            nb_CFL++;
        int tl = rand() % A + arf * nb_CFL;
        tabu_table_point[change_point] = stop_check - tl;
        if (abs(best_answer - good_answer) < 50 && best_answer > 50 && last_opti - stop_check > 10) {
            cout << "huge change!" << endl;
            memset(point_choose, 0, sizeof(point_choose));
            choose_all_point(choose_point, 2);
            memset(tabu_table_point, 0x3f, sizeof(tabu_table_point));
            last_opti = stop_check;
        }
        best_answer = min(best_answer, good_answer);
    }
    return 0;
}

