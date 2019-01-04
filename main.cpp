#include "Partition_Coloring_Problem.h"
#include "Graph_Coloring.h"
#include "Partition_Graph_coloring.h"

rec_point point_tot_conflicts[maxn];
edge e[2 * maxm];
gene P[30], ans_p;
int nb_CFL, tot, m, n, partition_size, gene_size, color_size;
int tabutable[maxn][maxn], conflict_color[maxn][maxn];
int head[maxn], p_color_point[maxp], partition_vertex_by_vertex[maxn], book_color[maxn];
int array_form_1_to_n[maxn];
long double s_gene[maxn];
int min_dis[maxn];
bool choose_point_bool[maxn];
vector<int> choose_point;
vector<int> partition_vertex_by_group[maxp];
point_set Parition_Point_Set;

int rand (int a, int b) {
	if (a > b)swap (a, b);
	static default_random_engine e_engine (static_cast<unsigned int>(time (nullptr)));
	uniform_int_distribution<int> u (a, b);
	return u (e_engine);
}

void insert (int u, int v) {
	e[++tot] = (edge) {v, head[u]};
	head[u] = tot;
	e[++tot] = (edge) {u, head[v]};
	head[v] = tot;
}

void input_graph_data () {
	cin >> n;//顶点个数
	cin >> m;//边数
	cin>>partition_size ;//划分个数
	for (int i = 1; i <= n; i++) {
		int j ;//每个点所属的划分，从1开始编号
		cin>>j;
		j++;
		partition_vertex_by_group[j].push_back (i);//划分j包含的顶点集合
		partition_vertex_by_vertex[i] = j;//顶点i所属的集合
		array_form_1_to_n[i] = i;//初始化一个从1到n的数组
	}
	for (int i = 1; i <= m; i++) {
		int u, v;
		cin >> u >> v;//读入一条边
		u++;
		v++;
		insert (u, v);//邻接表加边操作
	}
}

pair<int, int> find_connect_point (int x) {
	pair<int, int> cnt;
	for (int i = head[x]; i; i = e[i].next)
		if (partition_vertex_by_vertex[e[i].go] != partition_vertex_by_vertex[x]) {
			if (choose_point_bool[e[i].go])
				cnt.first++;
			cnt.second++;
		}
	return cnt;
}


void output_gene (gene p) {
	cout << p.size << endl;
	for (int i = 1; i <= p.size; i++) {
		cout << p.v[i].a.size () << " ";
		for (auto x:p.v[i].a)
			cout << x << " ";
		cout << endl;
	}
}

bool check (int color_number) {
	int stop_check = CHEAK_ITERATIONS;//迭代次数
	int best_answer = partition_size * partition_size;//最优解（最少冲突个数）
	while (stop_check--) {
		vector<int> coloring_point_set = Partition_Problem::find_point (partition_size);//得到一个待染色的点集
		if (Graph_Coloring_Problem::graph_color (coloring_point_set, color_number))
			return true;
	}
	return false;
}


int main () {
	ios_base::sync_with_stdio (false);
	freopen ("input.pcp", "r", stdin);
	//freopen ("output.txt", "w", stdout);
	input_graph_data ();//读入数据
	Partition_Problem::init_point_to_color (POPULATION_SIZE, partition_vertex_by_group, partition_size);//初始化种群
	//int l = 1, r = partition_size;
	//while (l <= r) {
	//  int mid = (l + r) >> 1;
	//color_size = mid;
	//cout << mid << endl;
	check (7);//对给定染色数进行Graph Coloring检验
	//if (check(mid))r = mid - 1;
	//else l = mid + 1;
	//}
	if (Graph_Coloring_Problem::judge (ans_p))
		output_gene (ans_p);
	return 0;
}
