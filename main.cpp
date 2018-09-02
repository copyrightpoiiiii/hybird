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
    if (judge(ans_p))
        output_gene(ans_p);
    return 0;
}
