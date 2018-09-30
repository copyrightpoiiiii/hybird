#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <ctime>
#include <cmath>

#define maxn 2005
#define maxp 2005
#define maxm 400005
#define init_size 10
#define L_LS 1000
#define L_check 1000
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
} P[30], ans_p;

struct rec_point {
    int color, sum;
} conflict_number[maxn];

int nb_CFL;
int tabutable[maxn][maxn];
int head[maxn], tot, m, n, p, p2, gene_size, color_size;
int conflict_color[maxn][maxn];
int p_color_point[maxp];
int pro[maxn], book_color[maxn];
vector<int> con_p[maxp];

int read() {
    int x = 0, f = 1;
    auto ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-')f = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        x = x * 10 + ch - '0';
        ch = getchar();
    }
    return x * f;
}

void insert(int u, int v) {
    e[++tot] = (edge) {v, head[u]};
    head[u] = tot;
    e[++tot] = (edge) {u, head[v]};
    head[v] = tot;
}

void init() {
    n = read();
    m = read();
    p = read();
    //m /= 2;
    for (int i = 1; i <= n; i++) {
        int j = read() + 1;
        con_p[j].push_back(i);
        pro[i] = j;
    }
    for (int i = 1; i <= m; i++) {
        int u = read() + 1, v = read() + 1;
        insert(u, v);
    }
    //p = read();
    p2 = p;
}

void init_gen(int lim) {
    for (int i = 1; i <= init_size; i++)
        for (int j = 1; j <= P[i].size; j++)
            P[i].v[j].a.clear();
    gene_size = init_size;
    bool p_book[maxp], p_book_1[maxp];
    int rec_point_col[maxn];
    int t[maxn];
    memset(t, 0, sizeof(t));
    for (int i = 1; i <= init_size; i++) {
        P[i].size = 0;
        memset(rec_point_col, 0, sizeof(rec_point_col));
        memset(p_book, 0, sizeof(p_book));
        memset(p_book_1, 0, sizeof(p_book_1));
        memset(book_color, 0, sizeof(book_color));
        for (int j = 1; j <= p; j++) {
            int x = rand() % p + 1;
            while (p_book_1[x]) {
                x = rand() % p + 1;
            }
            p_book_1[x] = true;
            int min_co = n + 1, id;
            for (int k = 0; k < con_p[x].size(); k++) {
                for (int y = head[con_p[x][k]]; y; y = e[y].next)
                    t[book_color[e[y].go]] = 1;
                for (int y = 1; y <= lim; y++) {
                    if (!t[y]) {
                        if (y < min_co) {
                            min_co = y;
                            id = con_p[x][k];
                        }
                        //break;
                    }
                    t[y] = 0;
                }
            }
            if (min_co <= lim) {
                P[i].size = max(P[i].size, min_co);
                book_color[id] = min_co;
                rec_point_col[id] = min_co;
                P[i].v[min_co].a.push_back(id);
                p_book[x] = 1;
            }
        }
        for (int j = 1; j <= p; j++)
            if (!p_book[j]) {
                int x = rand() % con_p[j].size();
                int co = min(rand() % lim + 1, P[i].size + 1);
                P[i].size = max(P[i].size, co);
                rec_point_col[x] = co;
                P[i].v[co].a.push_back(con_p[j][x]);
            }
    }
}


void crossover(gene s1, gene s2, gene &s) {
    bool book[maxp];
    memset(book, 0, sizeof(book));
    s.size = s1.size;
    for (int i = 1; i <= s1.size; i++) {
        int maxnum = 0, maxid = 1;
        if (i % 2 == 1) {
            for (int j = 1; j <= s1.size; j++)
                if (s1.v[j].a.size() > maxnum) {
                    maxnum = s1.v[j].a.size();
                    maxid = j;
                }
            s.v[i].a.swap(s1.v[maxid].a);
            for (int j = 0; j < s.v[i].a.size(); j++) {
                book[pro[s.v[i].a[j]]] = true;
            }
            gene news2;
            for (int j = 1; j <= news2.size; j++)
                news2.v[j].a.clear();
            for (int j = 1; j <= s2.size; j++) {
                for (int k = 0, y; k < s2.v[j].a.size(); k++)
                    if (!book[pro[y = s2.v[j].a[k]]])
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
                book[pro[s.v[i].a[j]]] = true;
            }
            gene news1;
            for (int j = 1; j <= news1.size; j++)
                news1.v[j].a.clear();
            for (int j = 1; j <= s1.size; j++) {
                for (int k = 0, y; k < s1.v[j].a.size(); k++)
                    if (!book[pro[y = s1.v[j].a[k]]])
                        news1.v[j].a.push_back(y);
                s1.v[j].a.swap(news1.v[j].a);
            }
        }
    }
    for (int i = 1; i <= p; i++)
        if (!book[i]) {
            int x = rand() % s.size + 1;
            int y = rand() % con_p[i].size();
            s.v[x].a.push_back(con_p[i][y]);
        }
}

bool judge(gene p_j) {
    int color[maxn];
    int rec_point[maxn];
    int sum=0;
    memset(rec_point, 0, sizeof(rec_point));
    memset(color, 0, sizeof(color));
    for (int i = 1; i <= p_j.size; i++) {
        for (int j = 0; j < p_j.v[i].a.size(); j++) {
            color[pro[p_j.v[i].a[j]]] = p_j.v[i].a[j];
            rec_point[p_j.v[i].a[j]] = i;
        }
        sum+=p_j.v[i].a.size();
    }
    for (int i = 1; i <= p; i++) {
        for (int j = head[color[i]]; j; j = e[j].next)
            if (rec_point[color[i]] == rec_point[e[j].go])
                return 0;
    }
    return 1;
}

int f(gene p_j) {
    int ans = 0;
    int color[maxn];
    int rec_point[maxn];
    memset(rec_point, 0, sizeof(rec_point));
    memset(color, 0, sizeof(color));
    for (int i = 1; i <= p_j.size; i++)
        for (int j = 0; j < p_j.v[i].a.size(); j++) {
            color[pro[p_j.v[i].a[j]]] = p_j.v[i].a[j];
            rec_point[p_j.v[i].a[j]] = i;
        }
    for (int i = 1; i <= p; i++) {
        for (int j = head[color[i]]; j; j = e[j].next)
            if (rec_point[color[i]] == rec_point[e[j].go])
                ans++;
    }
    return ans/2 ;
}

void find(gene p) {
    int color[maxn];
    bool rec_point[maxn];
    memset(rec_point, 0, sizeof(rec_point));
    memset(p_color_point,0, sizeof(p_color_point));
    memset(color, 0, sizeof(color));
    nb_CFL = 0;
    memset(conflict_color, 0, sizeof(conflict_color));
    memset(conflict_number, 0, sizeof(conflict_number));
    for (int i = 1; i <= p.size; i++)
        for (int j = 0; j < p.v[i].a.size(); j++) {
            color[p.v[i].a[j]] = i;
            conflict_number[p.v[i].a[j]].color = i;
            rec_point[p.v[i].a[j]] = true;
            p_color_point[pro[p.v[i].a[j]]] = p.v[i].a[j];
        }
    for (int i = 1; i <= n; i++) {
        conflict_number[i].sum = 0;
        for (int j = head[i]; j; j = e[j].next)
            if (rec_point[e[j].go]) {
                conflict_color[i][color[e[j].go]]++;
                if (color[e[j].go] == color[i])
                    conflict_number[i].sum++;
            }
        nb_CFL += conflict_number[i].sum;
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
        for (int i = 1; i <= n; i++)
            if (conflict_number[i].sum > 0) {
                for (int j = 1; j <= p.size; j++)
                    if (j != conflict_number[i].color && tabutable[i][j] >= iter) {
                        if (conflict_number[i].sum - conflict_color[i][j] > conflict_number[rec_id].sum - new_pri_f) {
                            new_pri_f = conflict_color[i][j];
                            new_pri = j;
                            rec_id = i;
                        }
                    }
            }
        int change_flag = 0, cha = conflict_number[rec_id].sum - new_pri_f;
        /*判断换点是否会更优*/
        for (int i = 1; i <= p2; i++)
            for (int j = 0; j < con_p[i].size(); j++)
                if (con_p[i][j] != p_color_point[i]) {
                    for (int k = 1; k <= p.size; k++)
                        if (tabutable[con_p[i][j]][k] >= iter)
                            if (conflict_number[p_color_point[i]].sum - conflict_color[con_p[i][j]][k] >
                                cha) {
                                change_flag = 1;
                                new_pri_f = conflict_color[con_p[i][j]][k];
                                new_pri = k;
                                rec_id = con_p[i][j];
                                cha = conflict_number[p_color_point[i]].sum - conflict_color[con_p[i][j]][k];
                            }
                }
        if (new_pri == -1) {
            p = best_res;
            return;
        } else if (!change_flag) {
            color_change = conflict_number[rec_id].color;
            for (int j = 0; j < p.v[color_change].a.size(); j++)
                if (p.v[color_change].a[j] == rec_id)
                    p.v[color_change].a.erase(p.v[color_change].a.begin() + j);
            nb_CFL -= conflict_number[rec_id].sum;
            nb_CFL += new_pri_f;
            for (int j = head[rec_id]; j; j = e[j].next) {
                if (conflict_number[e[j].go].color == color_change&&color_change!=0)
                    conflict_number[e[j].go].sum--;
                else if (conflict_number[e[j].go].color == new_pri)
                    conflict_number[e[j].go].sum++;
                conflict_color[e[j].go][color_change]--;
                conflict_color[e[j].go][new_pri]++;
            }
            tabutable[rec_id][color_change] = iter - tl;
            conflict_number[rec_id].sum = new_pri_f;
            conflict_number[rec_id].color = new_pri;
            p.v[new_pri].a.push_back(rec_id);
            if (nb_CFL <= best_fun) {
                best_fun = nb_CFL;
                best_res = p;
            }
            if (!nb_CFL)
                return;
        }else {
            for (int j = head[rec_id]; j; j = e[j].next) {
                if (conflict_number[e[j].go].color == new_pri)
                    conflict_number[e[j].go].sum++;
                conflict_color[e[j].go][new_pri]++;
            }
            int p_of_change = pro[rec_id];
            color_change = conflict_number[p_color_point[p_of_change]].color;
            for (int j = 0; j < p.v[color_change].a.size(); j++)
                if (p.v[color_change].a[j] == p_color_point[p_of_change])
                    p.v[color_change].a.erase(p.v[color_change].a.begin() + j);
            for (int j = head[p_color_point[p_of_change]]; j; j = e[j].next) {
                if (conflict_number[e[j].go].color == color_change)
                    conflict_number[e[j].go].sum--;
                conflict_color[e[j].go][color_change]--;
            }
            nb_CFL -= conflict_number[p_color_point[p_of_change]].sum;
            nb_CFL += new_pri_f;
            tabutable[p_color_point[p_of_change]][color_change] = iter - tl;
            conflict_number[p_color_point[p_of_change]].color = 0;
            conflict_number[p_color_point[p_of_change]].sum = 0;
            conflict_number[rec_id].sum = new_pri_f;
            conflict_number[rec_id].color = new_pri;
            p_color_point[p_of_change] = rec_id;
            p.v[new_pri].a.push_back(rec_id);
            if (nb_CFL <= best_fun) {
                best_fun = nb_CFL;
                best_res = p;
            }
            if (!nb_CFL)
                return;
        }
    }
    p = best_res;
}

int pre_dis(gene s, gene t) {
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
    return n - sum;
}

int dis(gene s, gene t) {
    int color[maxp];
    int sum = 0;
    for (int i = 1; i <= t.size; i++)
        for (int j = 0; j < t.v[i].a.size(); j++)
            color[pro[t.v[i].a[j]]] = t.v[i].a[j];
    for (int i = 1; i <= s.size; i++) {
        for (int j = 0; j < s.v[i].a.size(); j++)
            if (color[pro[s.v[i].a[j]]] != s.v[i].a[j]) {
                sum++;
                s.v[i].a[j] = color[pro[s.v[i].a[j]]];
            }
    }
    sum += pre_dis(s, t);
    return sum;
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
        s_gene[i] += pow(E, (long double) 0.08 * n * (long double) n / min_dis[i]);
        if (s_gene[i] > max_index) {
            max_index = s_gene[i];
            max_id = i;
        }
    }
    P[max_id] = P[gene_size];
    gene_size--;
}

bool check(int x) {
    int stop_cond = L_check;
    for (int i = 1; i <= init_size; i++)
        if (judge(P[i])) {
            ans_p = P[i];
            return 1;
        }
    while (stop_cond--) {
        int p1 = rand() % init_size + 1, p2 = rand() % init_size + 1;
        gene ps;
        crossover(P[p1], P[p2], ps);
        localSearch(ps, L_LS);
        cout << "check: " << f(ps) <<endl;
        if (judge(ps)) {
            ans_p = ps;
            cout << L_check - stop_cond << endl;
            return 1;
        }
        P[++gene_size] = ps;
        optimize();
    }
    return 0;
}

void output_gene(gene p) {
    cout << p.size << endl;
    int tm[maxn];
    int tot=0;
    for (int i = 1; i <= p.size; i++) {
        for (int j = 0; j < p.v[i].a.size(); j++) {
            cout << p.v[i].a[j] << " ";
            tm[++tot] = p.v[i].a[j];
        }
        cout << endl;
    }
    cout<<"hh"<<" "<<tot<<endl;
    for(int i=1;i<=tot;i++)
        cout<<tm[i]<<" ";
}

int main() {
    cout<<"hhh"<<endl;
    ios_base::sync_with_stdio(false);
    freopen("input.pcp", "r", stdin);
    freopen("output.txt", "w", stdout);
    srand((unsigned) time(NULL));
    init();
    int l = 1, r = p;
    //while (l <= r) {
        int mid = 48;
        color_size = mid;
        init_gen(mid);
        //cout << mid << endl;
        if(check(mid))
        //if (check(mid))r = mid - 1;
      //  else l = mid + 1;
    //}
    output_gene(ans_p);
    return 0;
}