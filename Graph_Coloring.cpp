//
// Created by 张淇 on 2018/9/2.
//
#include "Graph_Coloring.h"

namespace Graph_Coloring_Problem {
	void init_gen (int lim, int size) {
		for (int i = 1; i <= gene_size; i++) {//清空点集
			for (int j = 1; j <= P[i].size; j++)
				P[i].v[j].a.clear ();
		}
		bool vispoint[maxn];
		memset (vispoint, 0, sizeof (vispoint));
		gene_size = size;//种群大小
		vector<int> book_point;
		for (int i = 1; i <= size; i++) {
			book_point.insert (book_point.begin (), array_form_1_to_n + 1, array_form_1_to_n + 1 + partition_size);
			random_shuffle (book_point.begin (), book_point.end ());//生成一个乱排，代替随机数使用
			P[i].size = 0;
			for (int t = book_point.size () - 1; t >= 0; t--) {//给第t个点染色
				int x = choose_point[book_point[t]];
				book_point.erase (book_point.begin () + t);
				for (int k = head[x], y; k; k = e[k].next) {
					y = e[k].go;
					vispoint[y] = true;
				}
				int flag = 0;
				for (int k = 1; k <= P[i].size; k++) {//选择一个与该点无冲突的染色集合
					int flaggro = 0;
					for (auto q:P[i].v[k].a) {
						if (vispoint[q]) {
							flaggro = 1;
							break;
						}
					}
					if (!flaggro) {//如果找到了，就放入
						P[i].v[k].a.push_back (x);
						flag = 1;
						break;
					}
				}
				if (!flag) {//没有找到
					if (P[i].size < lim) {//染成新的颜色
						P[i].size++;
						P[i].v[P[i].size].a.push_back (x);
					} else {//随机放入一个集合中
						int to_pri = rand (1, lim);
						P[i].v[to_pri].a.push_back (x);
					}
				}
				for (int k = head[x], y; k; k = e[k].next) {
					y = e[k].go;
					vispoint[y] = false;
				}
			}
		}
	}


	void crossover (gene s1, gene s2, gene &s) {
		int book[maxn], book_point[maxn];
		memset (book_point, 0, sizeof (book_point));
		s.size = s1.size;
		for (int i = 1; i <= s1.size; i++) {
			memset (book, 0, sizeof (book));
			int maxnum = 0, maxid = 1;
			if (i % 2 == 1) {//从第一个方案中选择
				for (int j = 1; j <= s1.size; j++)
					if (s1.v[j].a.size () > maxnum) {//选择节点数最多的染色点集
						maxnum = (int) s1.v[j].a.size ();
						maxid = j;
					}
				s.v[i].a.swap (s1.v[maxid].a);//把集合放入s中
				for (auto j:s.v[i].a) {
					book[j] = 1;
					book_point[j] = 1;
				}
				gene news2;
				for (int j = 1; j <= s2.size; j++) {//从第二个集合中删去被选择的点
					for (int k = 0, y; k < s2.v[j].a.size (); k++)
						if (!book[y = s2.v[j].a[k]])
							news2.v[j].a.push_back (y);
					s2.v[j].a.swap (news2.v[j].a);
				}
			} else {//从第二个中选择
				for (int j = 1; j <= s2.size; j++)
					if (s2.v[j].a.size () > maxnum) {
						maxnum = (int) s2.v[j].a.size ();
						maxid = j;
					}
				s.v[i].a.swap (s2.v[maxid].a);
				for (auto j:s.v[i].a) {
					book[j] = 1;
					book_point[j] = 1;
				}
				gene news1;
				for (int j = 1; j <= s1.size; j++) {
					for (int k = 0, y; k < s1.v[j].a.size (); k++)
						if (!book[y = s1.v[j].a[k]])
							news1.v[j].a.push_back (y);
					s1.v[j].a.swap (news1.v[j].a);
				}
			}
		}
		for (int i = 1; i <= partition_size; i++)
			if (!book_point[choose_point[i]]) {//如果仍有未被放入的点，随机选择一个集合放入
				int x = rand (1, s.size);
				s.v[x].a.push_back (choose_point[i]);
			}
	}

	bool judge (gene p) {
		int color[maxn];
		for (int i = 1; i <= p.size; i++)
			for (int j = 0; j < p.v[i].a.size (); j++)
				color[p.v[i].a[j]] = i;
		for (int i = 1; i <= partition_size; i++)
			for (int j = head[choose_point[i]]; j; j = e[j].next)
				if (color[e[j].go] == color[choose_point[i]])
					return false;
		return true;
	}

	int f (gene p) {//得到p的冲突数
		int ans = 0;
		int color[maxn];
		for (int i = 1; i <= p.size; i++)
			for (int j = 0; j < p.v[i].a.size (); j++)
				color[p.v[i].a[j]] = i;
		for (int i = 1; i <= partition_size; i++)
			for (int j = head[choose_point[i]]; j; j = e[j].next)
				if (color[e[j].go] == color[choose_point[i]])
					ans++;
		return ans / 2;
	}

	void find (gene p) {//得到冲突个数，并处理出每个邻接点的染色情况
		int point_color[maxn];
		nb_CFL = 0;
		memset (conflict_color, 0, sizeof (conflict_color));
		for (int i = 1; i <= p.size; i++)
			for (int j = 0; j < p.v[i].a.size (); j++) {
				point_color[p.v[i].a[j]] = i;
				point_tot_conflicts[p.v[i].a[j]].color = i;
			}
		for (int i = 1; i <= partition_size; i++) {
			int x = choose_point[i];
			point_tot_conflicts[x].sum = 0;
			for (int j = head[x]; j; j = e[j].next) {
				conflict_color[x][point_color[e[j].go]]++;//维护点i的邻接点中染成该种颜色的个数
				if (point_color[e[j].go] == point_color[x])//如果邻接点所染颜色相同
					point_tot_conflicts[x].sum++;
			}
			nb_CFL += point_tot_conflicts[x].sum;
		}
		nb_CFL /= 2;
	}

	void localSearch (gene &p, int iter) {
		gene best_res = p;
		find (p);
		int best_fun = nb_CFL;
		memset (tabutable, 0x3f, sizeof (tabutable));
		while (iter--) {
			int tl, new_pri = -1, new_pri_f = n * n;
			tl = rand (0, A) + (int) (arf * nb_CFL);
			int color_ori, rec_id = 0;
			for (int i = 1; i <= partition_size; i++)//枚举每个点
				if (point_tot_conflicts[choose_point[i]].sum > 0) {
					for (int j = 1; j <= p.size; j++)//枚举把一个点放入一种新的颜色中带来的变化
						if (j != point_tot_conflicts[choose_point[i]].color && tabutable[choose_point[i]][j] >= iter) {
							if (point_tot_conflicts[choose_point[i]].sum - conflict_color[choose_point[i]][j] >
							    point_tot_conflicts[rec_id].sum - new_pri_f) {//如果带来的提升幅度更大就记录下来
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
				color_ori = point_tot_conflicts[rec_id].color;//改变之前的颜色
				for (int j = 0; j < p.v[color_ori].a.size (); j++)
					if (p.v[color_ori].a[j] == rec_id) {
						p.v[color_ori].a[j] = p.v[color_ori].a[p.v[color_ori].a.size () - 1];//将这个点放到集合尾部
						p.v[color_ori].a.erase (p.v[color_ori].a.begin () + p.v[color_ori].a.size () - 1);
						//删掉集合尾部的点
					}
				nb_CFL -= point_tot_conflicts[rec_id].sum;//维护冲突数
				nb_CFL += new_pri_f;
				for (int j = head[rec_id]; j; j = e[j].next) {
					if (point_tot_conflicts[e[j].go].color == color_ori)//邻接点中与旧颜色相同的点的冲突数--
						point_tot_conflicts[e[j].go].sum--;
					else if (point_tot_conflicts[e[j].go].color == new_pri)//邻接点中与新颜色相同的点的冲突数--
						point_tot_conflicts[e[j].go].sum++;
					conflict_color[e[j].go][color_ori]--;
					conflict_color[e[j].go][new_pri]++;
				}
				if (nb_CFL <= best_fun) {//维护最优解
					best_fun = nb_CFL;
					best_res = p;
				}
				tabutable[rec_id][color_ori] = iter - tl;
				point_tot_conflicts[rec_id].sum = new_pri_f;
				point_tot_conflicts[rec_id].color = new_pri;
				p.v[new_pri].a.push_back (rec_id);
				if (!nb_CFL)
					return;
			}
		}
		p = best_res;
	}

	int dis (gene s, gene t) {//定义s与t的距离为：将s转化成t最少需要修改点的个数
		int color[maxn];
		int sum = 0;
		for (int i = 1; i <= t.size; i++)
			for (int j = 0; j < t.v[i].a.size (); j++)
				color[t.v[i].a[j]] = i;
		for (int i = 1; i <= s.size; i++) {//使用贪心法求近似解
			int rec_num[maxn], max_rec = 0;
			memset (rec_num, 0, sizeof (rec_num));
			for (auto j:s.v[i].a) {//找出s的一个颜色与t中某种颜色对应，使得交集最大
				rec_num[color[j]]++;
				max_rec = max (max_rec, rec_num[color[j]]);
			}
			sum += max_rec;
		}
		return partition_size - sum;
	}

	void optimize () {
		long double max_index = 0;
		int max_id = gene_size;
		for (int i = 1; i <= gene_size; i++) {//给每个染色方案估价
			s_gene[i] += pow (E, (long double) 0.08 * partition_size * (long double) partition_size / min_dis[i]);
			if (s_gene[i] > max_index) {
				max_index = s_gene[i];
				max_id = i;
			}
		}
		P[max_id] = P[gene_size];
		s_gene[max_id]=s_gene[gene_size];
		for (int i = 1; i < gene_size; i++)
			if(i!=max_id)//选择最差的解删掉
				min_dis[max_id] = min (min_dis[max_id], dis (P[max_id], P[i]));
		gene_size--;
	}

	bool graph_color (vector<int> point_set, int color_number) {
		choose_point = point_set;
		Graph_Coloring_Problem::init_gen (color_number, POPULATION_SIZE);//初始化混合进化算法种群
		int stop_cond = 10;//CHEAK_ITERATIONS;
		for (int i = 1; i <= POPULATION_SIZE; i++)
			if (Graph_Coloring_Problem::judge (P[i])) {//检查种群中是否存在合法的染色方案
				ans_p = P[i];
				return true;
			}
		for (int i = 1; i < gene_size; i++)
			s_gene[i] = f (P[i]);//计算出每个解的冲突数
		while (stop_cond--) {
			int p1 = rand (1, POPULATION_SIZE), p2 = rand (1, POPULATION_SIZE);
			while (p1 == p2)
				p2 = rand (1, POPULATION_SIZE);//随机选择种群中的两个染色方案进行croosover
			gene ps;
			Graph_Coloring_Problem::crossover (P[p1], P[p2], ps);
			Graph_Coloring_Problem::localSearch (ps, GCP_LOCAL_SEARCH_ITER);
			if (Graph_Coloring_Problem::judge (ps)) {//如果冲突数为0
				ans_p = ps;
				cout << "I got it!" << endl;
				cout << CHEAK_ITERATIONS - stop_cond << endl;
				return true;
			}
			P[++gene_size] = ps;
			s_gene[gene_size] = f (ps);
			min_dis[gene_size]=inf;
			for (int j = 1; j < gene_size; j++)
					min_dis[gene_size] = min (min_dis[gene_size], dis (ps, P[j]));
			Graph_Coloring_Problem::optimize ();//将新方案放入种群，进行种群的更新
		}
		return false;
	}
}


