//
// Created by 张淇 on 2018/8/31.
//

#include "Partition_Graph_coloring.h"

int book_conflict[maxn];

namespace Partition_Problem {
	void init_point_to_color (int size, vector<int> *partition_set, int num) {
		//从每个partition中选一个点
		//重复size次，构成Partition_Problem的种群
		int rec_partition_choose[maxp], i = 1;
		vector<int> random_shuffle_for_partition;
		Parition_Point_Set.size = size;//种群大小
		while (size--) {
			random_shuffle_for_partition.insert (random_shuffle_for_partition.begin (), array_form_1_to_n + 1,
			                                     array_form_1_to_n + num + 1);
			random_shuffle (random_shuffle_for_partition.begin (), random_shuffle_for_partition.end ());//生成一个1～n的乱排，为随机选择n个划分的顺序
			memset (rec_partition_choose, 0, sizeof (rec_partition_choose));
			for (int i = random_shuffle_for_partition.size ()-1; i >= 0; i--) {//按照随机顺序选择划分，从尾部开始枚举
				int k=rand (0,(int) partition_set[random_shuffle_for_partition[i]].size () -1);
				int choose_point = partition_set[random_shuffle_for_partition[i]][k];//从划分中随机选择一个点
				rec_partition_choose[random_shuffle_for_partition[i]] = choose_point;//记录该划分选择的点
				random_shuffle_for_partition.erase (random_shuffle_for_partition.begin () + i);//从尾部删去
			}
			Parition_Point_Set.a[size].insert (Parition_Point_Set.a[size].begin (), rec_partition_choose,rec_partition_choose + partition_size + 1);//将生成的点集加入种群中
		}
	}


	vector<int> crossover (vector<int> x, vector<int> y) {
		int choose_side = 0, choose_point[maxn];
		memset (choose_point, 0, sizeof (choose_point));
		bool choose[maxp], choose_poi[maxn];
		memset (choose, 0, sizeof (choose));
		memset (choose_poi, 0, sizeof (choose_poi));
		vector<int> a, tmp;
		a.clear ();
		tmp.clear ();
		tmp.insert (tmp.begin (), array_form_1_to_n + 1, array_form_1_to_n + x.size ());
		shuffle (tmp.begin (), tmp.end (), default_random_engine ((unsigned int) time (nullptr)));
		for (int i = 0; i < tmp.size (); i++) {
			int min_ind = inf, min_ind2 = inf, poi = -1;
			if (choose_side) {
				for (auto j:tmp)
					if (!choose[j]) {
						int cnt = 0, cnt2 = 0;
						for (int k = head[x[j]]; k; k = e[k].next) {
							if (choose_poi[e[k].go])
								cnt++;
							cnt2++;
						}
						if (cnt < min_ind) {
							min_ind = cnt;
							min_ind2 = cnt2;
							poi = x[j];
						} else if (cnt == min_ind && min_ind2 > cnt2) {
							min_ind2 = cnt2;
							poi = x[j];
						}
					}
			} else {
				for (auto j:tmp)
					if (!choose[j]) {
						int cnt = 0, cnt2 = 0;
						for (int k = head[y[j]]; k; k = e[k].next) {
							if (choose_poi[e[k].go])
								cnt++;
							cnt2++;
						}
						if (cnt < min_ind) {
							min_ind = cnt;
							min_ind2 = cnt2;
							poi = y[j];
						} else if (cnt == min_ind && min_ind2 > cnt2) {
							min_ind2 = cnt2;
							poi = y[j];
						}
					}
			}
			choose_point[partition_vertex_by_vertex[poi]] = poi;
			choose[partition_vertex_by_vertex[poi]] = true;
			choose_poi[poi] = true;
			choose_side ^= 1;
		}
		a.insert (a.begin (), choose_point, choose_point + x.size ());
		return a;
	}

	int get_conflict (vector<int> a) {
		int sum = 0;
		bool choose_point[maxn];
		memset (choose_point, 0, sizeof (choose_point));
		memset (book_conflict, 0, sizeof (book_conflict));
		for (auto i:a)
			choose_point[i] = true;//给集合中的点打标记
		for (auto j:a)
			for (int i = head[j]; i; i = e[i].next)
				if (choose_point[e[i].go]) {//如果邻接点在集合中
					book_conflict[partition_vertex_by_vertex[e[i].go]]++;//记录集合中每个点有多少个被标记点与其相连
					sum++;
				}
		return sum / 2;
	}

	vector<int> localSearch (vector<int> point_set, int iter) {
		int best_fun = get_conflict (point_set), tabutable[maxn];//最优解，tabu表
		vector<int> ans = point_set;
		bool choose_point[maxn];
		memset (tabutable, 0x3f, sizeof (tabutable));
		memset (choose_point, 0, sizeof (choose_point));
		for (auto i:point_set)
			choose_point[i] = true;//给集合中的点打标记
		while (iter--) {
			int tl, new_pri = -1, new_pri_f = -inf;//选择的点，给集合中边数带来的改变值
			tl = rand (0, A) + (int) (arf * best_fun);
			for (int i = 1; i <= n; i++)
				if (!choose_point[i] && tabutable[i] >= iter) {//枚举每个不在集合中的点，并且不是tabu操作中的禁忌点
					int cnt = 0;
					for (int j = head[i]; j; j = e[j].next)//遍历它的邻接节点
						if (choose_point[e[j].go] &&
						    partition_vertex_by_vertex[e[j].go] != partition_vertex_by_vertex[i])//如果邻接节点在集合中
							cnt++;
					if (book_conflict[partition_vertex_by_vertex[i]] - cnt > new_pri_f) {
						new_pri = i;
						new_pri_f = book_conflict[partition_vertex_by_vertex[i]] - cnt;//维护改变值
					}
				}
			if (new_pri == -1)
				return ans;
			else {
				int i = point_set[partition_vertex_by_vertex[new_pri]];//被取代的点，记为i
				for (int j = head[i]; j; j = e[j].next)
					if (choose_point[e[j].go]) {
						best_fun--;
						book_conflict[partition_vertex_by_vertex[e[j].go]]--;//与i相邻的集合中的点，连接数--
					}
				book_conflict[partition_vertex_by_vertex[i]] = 0;
				choose_point[i] = false;
				tabutable[i] = iter - tl;
				for (int j = head[new_pri]; j; j = e[j].next)
					if (choose_point[e[j].go]) {//与新点相邻的点
						best_fun++;
						book_conflict[partition_vertex_by_vertex[e[j].go]]++;
						book_conflict[partition_vertex_by_vertex[new_pri]]++;
					}
				choose_point[new_pri] = true;
				point_set[partition_vertex_by_vertex[new_pri]] = new_pri;
				if (new_pri_f > 0)
					ans = point_set;
			}
		}
		return ans;
	}

	int dis (vector<int> x, vector<int> y) {
		int cnt = 0;
		for (int i = 0; i < x.size (); i++)
			if (x[i] != y[i])//如果对于某个划分选择的点不同，则距离++
				cnt++;
		return cnt;
	}

	/*void optimize () {
		long double s_gene[Parition_Point_Set.size + 5];
		for (int i = 0; i <= Parition_Point_Set.size; i++)
			s_gene[i] = get_conflict (Parition_Point_Set.a[i]);
		int min_dis[maxn];
		memset (min_dis, 0x3f, sizeof (min_dis));
		for (int i = 1; i <= Parition_Point_Set.size; i++)
			for (int j = 1; j <= Parition_Point_Set.size; j++)
				if (i != j) {
					min_dis[i] = min (min_dis[i], dis (Parition_Point_Set.a[i], Parition_Point_Set.a[j]));
				}
		long double max_index = 0;
		int max_id = Parition_Point_Set.size;
		for (int i = 1; i <= Parition_Point_Set.size; i++) {
			s_gene[i] += pow (E, (long double) 0.05 * partition_size * (long double) partition_size / min_dis[i]);
			if (s_gene[i] > max_index) {
				max_index = s_gene[i];
				max_id = i;
			}
		}
		Parition_Point_Set.a[max_id] = Parition_Point_Set.a[Parition_Point_Set.size];
	}*/

	bool cmp (pair<int, int> a, pair<int, int> b) {
		return a.second > b.second;
	}

	vector<int> strategy_choose_point (vector<int> a, int number) {
		int tot = 0;
		pair<int, int> book[maxn];
		memset (book, 0, sizeof (book));
		bool in_vector[maxn];
		memset (in_vector, 0, sizeof (in_vector));
		for (auto x:a)
			in_vector[x] = true;
		for (auto x:a) {
			int cnt = 0;
			for (int i = head[x]; i; i = e[i].next)
				if (in_vector[e[i].go])
					cnt++;//计算相连的点的个数
			book[++tot] = make_pair (tot, cnt);
		}
		sort (book + 1, book + tot + 1, cmp);//按冲突数从大到小排列
		for (int i = 1; i <= number; i++) {//对前number个点，随机替换成对应划分中的其它点
			int up_bound = (int) partition_vertex_by_group[partition_vertex_by_vertex[a[book[i].first]]].size () - 1;
			int x = rand (0, up_bound);
			while (a[book[i].first] == partition_vertex_by_group[partition_vertex_by_vertex[a[book[i].first]]][x] &&
			       up_bound)
				x = rand (0, up_bound);
			a[book[i].first] = partition_vertex_by_group[partition_vertex_by_vertex[a[book[i].first]]][x];
		}
		return a;
	}

	vector<int> random_choose_point (vector<int> a, int number) {
		bool book[maxn];
		memset (book, 0, sizeof (book));
		for (int i = 1; i <= number; i++) {
			int x = rand (1, (int) a.size () - 1);
			while (book[x] && (a.size () - 1))
				x = rand (1, (int) a.size () - 1);
			int y = rand (0, (int) partition_vertex_by_group[partition_vertex_by_vertex[a[x]]].size () - 1);
			while (partition_vertex_by_group[partition_vertex_by_vertex[a[x]]][y] == a[x] &&
			       (partition_vertex_by_group[partition_vertex_by_vertex[a[x]]].size () - 1))
				y = rand (0, (int) partition_vertex_by_group[partition_vertex_by_vertex[a[x]]].size () - 1);
			a[x] = partition_vertex_by_group[partition_vertex_by_vertex[a[x]]][y];
			book[x] = true;
		}
		return a;
	}

	vector<int> find_point (int num) {//加上随机扰动之后的选点程序
		int iter = PCP_LOCAL_SEARCH_ITER, mini_conflict = inf;//迭代次数和最小冲突个数
		vector<int> tmp, ans, ori;
		int x = rand (0, Parition_Point_Set.size - 1), change_size = max (10, partition_size / 10);
		//从种群中随机选择一个子代，扰动时更改的点的个数
		int ori_conflict = get_conflict (Parition_Point_Set.a[x]);//得到该点集中边的个数
		tmp = Parition_Point_Set.a[x];
		while (iter--) {
			tmp = localSearch (tmp, 400);//local search缩小边的个数
			int conflict = get_conflict (tmp);//由点集构成的子图的边数是否变的更小
			if (conflict < mini_conflict) {
				ans = tmp;
				mini_conflict = conflict;
			}
			/*
			 * 如果得到的解的距离较近或者解的质量不够好时，提升换点的个数
			 */
			if (dis (tmp, ans) <= change_size*2 || abs (ori_conflict - conflict) < partition_size / 5) {
				change_size += 5;//如果当前点集与最优解的距离过近时，提升扰动幅度
			} else break;
			if (change_size >= partition_size / 2)
				break;
			tmp = strategy_choose_point (tmp, change_size);//随机换点操作，可以更换为有选择性的换点操作
		}
		Parition_Point_Set.a[x] = ans;//将点集加入种群
		return ans;
	}

	/*vector<int> find_point_old (int num) {//无换点扰动
		int x = rand (0, Parition_Point_Set.size - 1);
		Parition_Point_Set.a[Parition_Point_Set.size] = localSearch (Parition_Point_Set.a[x], 200);
		vector<int> tmp = Parition_Point_Set.a[Parition_Point_Set.size];
		optimize ();
		return tmp;
	}*/

}

