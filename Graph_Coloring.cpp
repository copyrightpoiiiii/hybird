//
// Created by 张淇 on 2018/9/2.
//
#include "Graph_Coloring.h"

namespace Graph_Coloring_Problem {
    void init_gen(int lim, int size) {
        for (int i = 1; i <= gene_size; i++) {
            for (int j = 1; j <= P[i].size; j++)
                P[i].v[j].a.clear();
        }
        bool vispoint[maxn];
        memset(vispoint, 0, sizeof(vispoint));
        gene_size = size;
        vector<int> book_point;
        for (int i = 1; i <= size; i++) {
            book_point.insert(book_point.begin(), array_form_1_to_n + 1, array_form_1_to_n + 1 + partition_size);
	        random_shuffle (book_point.begin (), book_point.end ());
            P[i].size = 0;
            for (int t = book_point.size()-1; t >= 0; t--) {
                int x = choose_point[book_point[t]];
                book_point.erase(book_point.begin() + t);
                for (int k = head[x], y; k; k = e[k].next) {
                    y = e[k].go;
                    vispoint[y] = true;
                }
                int flag = 0;
                for (int k = 1; k <= P[i].size; k++) {
                    int flaggro = 0;
                    for (auto q:P[i].v[k].a) {
                        if (vispoint[q]) {
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
                        int to_pri = rand(1, lim);
                        P[i].v[to_pri].a.push_back(x);
                    }
                }
                for (int k = head[x], y; k; k = e[k].next) {
                    y = e[k].go;
                    vispoint[y] = false;
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
                        maxnum = (int) s1.v[j].a.size();
                        maxid = j;
                    }
                s.v[i].a.swap(s1.v[maxid].a);
                for (auto j:s.v[i].a) {
                    book[j] = 1;
                    book_point[j] = 1;
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
                        maxnum = (int) s2.v[j].a.size();
                        maxid = j;
                    }
                s.v[i].a.swap(s2.v[maxid].a);
                for (auto j:s.v[i].a) {
                    book[j] = 1;
                    book_point[j] = 1;
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
        for (int i = 1; i <= partition_size; i++)
            if (!book_point[choose_point[i]]) {
                int x = rand(1, s.size);
                s.v[x].a.push_back(choose_point[i]);
            }
    }

    bool judge(gene p) {
        int color[maxn];
        for (int i = 1; i <= p.size; i++)
            for (int j = 0; j < p.v[i].a.size(); j++)
                color[p.v[i].a[j]] = i;
        for (int i = 1; i <= partition_size; i++)
            for (int j = head[choose_point[i]]; j; j = e[j].next)
                if (color[e[j].go] == color[choose_point[i]])
                    return false;
        return true;
    }

    int f(gene p) {
        int ans = 0;
        int color[maxn];
        for (int i = 1; i <= p.size; i++)
            for (int j = 0; j < p.v[i].a.size(); j++)
                color[p.v[i].a[j]] = i;
        for (int i = 1; i <= partition_size; i++)
            for (int j = head[choose_point[i]]; j; j = e[j].next)
                if (color[e[j].go] == color[choose_point[i]])
                    ans++;
        return ans / 2;
    }

    void find(gene p) {
        int point_color[maxn];
        nb_CFL = 0;
        memset(conflict_color, 0, sizeof(conflict_color));
        for (int i = 1; i <= p.size; i++)
            for (int j = 0; j < p.v[i].a.size(); j++) {
                point_color[p.v[i].a[j]] = i;
                point_tot_conflicts[p.v[i].a[j]].color = i;
            }
        for (int i = 1; i <= partition_size; i++) {
            int x = choose_point[i];
            point_tot_conflicts[x].sum = 0;
            for (int j = head[x]; j; j = e[j].next) {
                conflict_color[x][point_color[e[j].go]]++;
                if (point_color[e[j].go] == point_color[x])
                    point_tot_conflicts[x].sum++;
            }
            nb_CFL += point_tot_conflicts[x].sum;
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
            tl = rand(0, A) + (int) (arf * nb_CFL);
            int color_ori, rec_id = 0;
            for (int i = 1; i <= partition_size; i++)
                if (point_tot_conflicts[choose_point[i]].sum > 0) {
                    for (int j = 1; j <= p.size; j++)
                        if (j != point_tot_conflicts[choose_point[i]].color && tabutable[choose_point[i]][j] >= iter) {
                            if (point_tot_conflicts[choose_point[i]].sum - conflict_color[choose_point[i]][j] >
                                point_tot_conflicts[rec_id].sum - new_pri_f) {
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
                color_ori = point_tot_conflicts[rec_id].color;
                for (int j = 0; j < p.v[color_ori].a.size(); j++)
                    if (p.v[color_ori].a[j] == rec_id) {
                        p.v[color_ori].a[j]=p.v[color_ori].a[p.v[color_ori].a.size()-1];
                        p.v[color_ori].a.erase (p.v[color_ori].a.begin () + p.v[color_ori].a.size()-1);
                    }
                nb_CFL -= point_tot_conflicts[rec_id].sum;
                nb_CFL += new_pri_f;
                for (int j = head[rec_id]; j; j = e[j].next) {
                    if (point_tot_conflicts[e[j].go].color == color_ori)
                        point_tot_conflicts[e[j].go].sum--;
                    else if (point_tot_conflicts[e[j].go].color == new_pri)
                        point_tot_conflicts[e[j].go].sum++;
                    conflict_color[e[j].go][color_ori]--;
                    conflict_color[e[j].go][new_pri]++;
                }
                if (nb_CFL <= best_fun) {
                    best_fun = nb_CFL;
                    best_res = p;
                }
                tabutable[rec_id][color_ori] = iter - tl;
                point_tot_conflicts[rec_id].sum = new_pri_f;
                point_tot_conflicts[rec_id].color = new_pri;
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
            for (auto j:s.v[i].a) {
                rec_num[color[j]]++;
                max_rec = max(max_rec, rec_num[color[j]]);
            }
            sum += max_rec;
        }
        return partition_size - sum;
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
            s_gene[i] += pow(E, (long double) 0.08 * partition_size * (long double) partition_size / min_dis[i]);
            if (s_gene[i] > max_index) {
                max_index = s_gene[i];
                max_id = i;
            }
        }
        P[max_id] = P[gene_size];
        gene_size--;
    }

	bool graph_color(vector<int> point_set,int color_number){
			choose_point = point_set;
			Graph_Coloring_Problem::init_gen(color_number, POPULATION_SIZE);
			int stop_cond = 10;//CHEAK_ITERATIONS;
			for (int i = 1; i <= POPULATION_SIZE; i++)
				if (Graph_Coloring_Problem::judge(P[i])) {
					ans_p = P[i];
					return true;
				}
			int good_answer = partition_size * partition_size;
			while (stop_cond--) {
				int p1 = rand(1, POPULATION_SIZE), p2 = rand(1, POPULATION_SIZE);
				while (p1 == p2)
					p2 = rand(1, POPULATION_SIZE);
				gene ps;
				Graph_Coloring_Problem::crossover(P[p1], P[p2], ps);
				Graph_Coloring_Problem::localSearch(ps, GCP_LOCAL_SEARCH_ITER);
				int tmp = Graph_Coloring_Problem::f(ps);
				good_answer = min(good_answer, tmp);
				//cout << "conflict: " << tmp << endl;
				//if (abs(tmp - good_answer) < 50 && tmp > 50 && CHEAK_ITERATIONS - stop_cond > 200)
				//    break;
				if (Graph_Coloring_Problem::judge(ps)) {
					ans_p = ps;
					cout << "I got it!" << endl;
					cout << CHEAK_ITERATIONS - stop_cond << endl;
					return true;
				}
				//else cout<<Graph_Coloring_Problem::f(ps)<<endl;
				P[++gene_size] = ps;
				Graph_Coloring_Problem::optimize();
			}
			return false;
		}
}


