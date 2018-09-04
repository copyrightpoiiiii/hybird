#include "P_G_C.h"
#include "G_C.h"

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
    for (auto i:p.v) {
        for (auto x:i.a)
            cout << x << " ";
        cout << endl;
    }
}

bool check(int x) {
    int stop_check = L_check;
    int best_answer = p * p;
    while (stop_check--) {
        choose_point = P_P::find_point(p2);
        G_C::init_gen(x, init_size);
        int stop_cond = L_check;
        for (int i = 1; i <= init_size; i++)
            if (G_C::judge(P[i])) {
                ans_p = P[i];
                return true;
            }
        int good_answer = p * p;
        while (stop_cond--) {

            int p1 = rand(1, init_size), p2 = rand(1, init_size);
            gene ps;
            G_C::crossover(P[p1], P[p2], ps);
            G_C::localSearch(ps, L_LS);
            int tmp = G_C::f(ps);
            good_answer = min(good_answer, tmp);
            cout << tmp << endl;
            if (abs(tmp - good_answer) < 50 && tmp > 50 && L_check - stop_cond > 200)
                break;
            if (G_C::judge(ps)) {
                ans_p = ps;
                cout << "I got it!" << endl;
                cout << L_check - stop_cond << endl;
                return true;
            }
            P[++gene_size] = ps;
            G_C::optimize();
        }
    }
    return false;
}


int main() {
    ios_base::sync_with_stdio(false);
    freopen("input.pcp", "r", stdin);
    freopen("output.txt", "w", stdout);
    init();
    P_P::init_point(init_size, con_p, p);
    //int l = 1, r = p;
    //while (l <= r) {
    //  int mid = (l + r) >> 1;
    //color_size = mid;
    //cout << mid << endl;
    check(50);
    //if (check(mid))r = mid - 1;
    //else l = mid + 1;
    //}
    if (G_C::judge(ans_p))
        output_gene(ans_p);
    return 0;
}
