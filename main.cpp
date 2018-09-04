#include "C_P.h"
#include "P_P.h"

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


void output_gene(gene p) {
    cout << p.size << endl;
    for (int i = 1; i <= p.size; i++) {
        for (int j = 0; j < p.v[i].a.size(); j++)
            cout << p.v[i].a[j] << " ";
        cout << endl;
    }
}

bool check(int x) {
    memset(point_choose, 0, sizeof(point_choose));
    int stop_check = L_check;
    int last_opti = L_check;
    int tabu_table_point[maxn];
    int best_answer = p * p;
    memset(tabu_table_point, 0x3f, sizeof(tabu_table_point));
    while (stop_check--) {
        P_P::init_gen(x, init_size);
        int stop_cond = L_check;
        for (int i = 1; i <= init_size; i++)
            if (P_P::judge(P[i])) {
                ans_p = P[i];
                return 1;
            }
        int good_answer = p * p;
        while (stop_cond--) {
            int p1 = rand() % init_size + 1, p2 = rand() % init_size + 1;
            gene ps;
            P_P::crossover(P[p1], P[p2], ps);
            P_P::localSearch(ps, L_LS);
            int tmp = P_P::f(ps);
            good_answer = min(good_answer, tmp);
            cout << tmp << endl;
            if (abs(tmp - good_answer) < 50 && tmp > 50 && L_check - stop_cond > 200)
                break;
            if (P_P::judge(ps)) {
                ans_p = ps;
                cout << "I got it!" << endl;
                cout << L_check - stop_cond << endl;
                return 1;
            }
            P[++gene_size] = ps;
            P_P::optimize();
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



int main() {
    ios_base::sync_with_stdio(false);
    freopen("input.pcp", "r", stdin);
    freopen("output.txt", "w", stdout);
    srand((unsigned) time(NULL));
    init();
    //int l = 1, r = p;
    //while (l <= r) {
    //  int mid = (l + r) >> 1;
    //color_size = mid;
    //cout << mid << endl;
    check(50);
    //if (check(mid))r = mid - 1;
    //else l = mid + 1;
    //}
    if (P_P::judge(ans_p))
        output_gene(ans_p);
    return 0;
}
