#include "Partition_Coloring_Problem.h"
#include "Graph_Coloring.h"
#include "Partition_Graph_coloring.h"
//#include "segtree_heap.h"

rec_point point_tot_conflicts[maxn];
edge e[2 * maxm];
gene P[30], ans_p;
int nb_CFL, tot, m, n, partition_size, gene_size, color_size;
int tabutable[maxn][maxn], conflict_color[maxn][maxn];
int head[maxn], p_color_point[maxp], partition_vertex_by_vertex[maxn], book_color[maxn];
int array_form_1_to_n[maxn];
bool choose_point_bool[maxn];
vector<int> choose_point;
vector<int> partition_vertex_by_group[maxp];
point_set Parition_Point_Set;

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

void swap(int a, int b) {
    a ^= b;
    b ^= a;
    a ^= b;
}

int rand(int a, int b) {
    if (a < b)swap(a, b);
    static default_random_engine e_engine(static_cast<unsigned int>(time(nullptr)));
    uniform_int_distribution<int> u(a, b);
    return u(e_engine);
}

void insert(int u, int v) {
    e[++tot] = (edge) {v, head[u]};
    head[u] = tot;
    e[++tot] = (edge) {u, head[v]};
    head[v] = tot;
}

void input_graph_data () {
    n = read();
    m = read();
    partition_size = read();
    for (int i = 1; i <= n; i++) {
        int j = read() + 1;
	    partition_vertex_by_group[j].push_back(i);
	    partition_vertex_by_vertex[i] = j;
        array_form_1_to_n[i] = i;
    }
    for (int i = 1; i <= m; i++) {
        int u = read() + 1, v = read() + 1;
        insert(u, v);
    }
}

pair<int, int> find_connect_point(int x) {
    pair<int, int> cnt;
    for (int i = head[x]; i; i = e[i].next)
        if (partition_vertex_by_vertex[e[i].go] != partition_vertex_by_vertex[x]) {
            if (choose_point_bool[e[i].go])
                cnt.first++;
            cnt.second++;
        }
    return cnt;
}


void output_gene(gene p) {
    cout << p.size << endl;
    for (int i = 1; i <= p.size; i++) {
        cout << p.v[i].a.size() << " ";
        for (auto x:p.v[i].a)
            cout << x << " ";
        cout << endl;
    }
}

bool check(int color_number) {
    int stop_check = CHEAK_ITERATIONS;
    int best_answer = partition_size * partition_size;
    while (stop_check--) {
        vector<int> coloring_point_set=Partition_Problem::find_point(partition_size);
        if(Graph_Coloring_Problem::graph_color(coloring_point_set,color_number))
            return true;
    }
    return false;
}


int main() {
    ios_base::sync_with_stdio(false);
    freopen("input.pcp", "r", stdin);
    freopen("output.txt", "w", stdout);
	input_graph_data ();
	Partition_Problem::init_point_to_color (POPULATION_SIZE, partition_vertex_by_group, partition_size);
    //int l = 1, r = partition_size;
    //while (l <= r) {
    //  int mid = (l + r) >> 1;
    //color_size = mid;
    //cout << mid << endl;
    //cout<<"sss"<<endl;
    check(7);
    //if (check(mid))r = mid - 1;
    //else l = mid + 1;
    //}
    if (Graph_Coloring_Problem::judge(ans_p))
        output_gene(ans_p);
    return 0;
}
