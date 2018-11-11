//
// Created by 张淇 on 2018/8/31.
//

#include "Partition_Graph_coloring.h"

int book_conflict[maxn];

namespace Partition_Problem {
	void init_point_to_color (int size, vector<int> *b, int num) {
		//从每个partition中选一个点
		//重复size次，构成Partition_Problem的种群
		int rec_partition_choose[maxp], i = 1;
		vector<int> random_shuffle_for_partition;
		Parition_Point_Set.size = size;
		while (size--) {
			random_shuffle_for_partition.insert (random_shuffle_for_partition.begin (), array_form_1_to_n + 1,
			                                     array_form_1_to_n + num + 1);
			random_shuffle (random_shuffle_for_partition.begin (), random_shuffle_for_partition.end ());
			memset (rec_partition_choose, 0, sizeof (rec_partition_choose));
			for (int i = random_shuffle_for_partition.size ()-1; i >= 0; i--) {
				int choose_point = b[random_shuffle_for_partition[i]][rand (0,
				                                                            (int) b[random_shuffle_for_partition[i]].size () -
				                                                            1)];
				rec_partition_choose[random_shuffle_for_partition[i]] = choose_point;
				random_shuffle_for_partition.erase (random_shuffle_for_partition.begin () + i);
			}
			Parition_Point_Set.a[size].insert (Parition_Point_Set.a[size].begin (), rec_partition_choose,
			                                   rec_partition_choose + partition_size + 1);
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
			choose_point[i] = true;
		for (auto j:a)
			for (int i = head[j]; i; i = e[i].next)
				if (choose_point[e[i].go]) {
					book_conflict[partition_vertex_by_vertex[e[i].go]]++;
					sum++;
				}
		return sum / 2;
	}

	vector<int> localSearch (vector<int> point_set, int iter) {
		int best_fun = get_conflict (point_set), tabutable[maxn];
		vector<int> ans = point_set;
		bool choose_point[maxn];
		memset (tabutable, 0x3f, sizeof (tabutable));
		memset (choose_point, 0, sizeof (choose_point));
		for (auto i:point_set)
			choose_point[i] = true;
		while (iter--) {
			int tl, new_pri = -1, new_pri_f = -inf;
			tl = rand (0, A) + (int) (arf * best_fun);
			//pair<int,int> change_point_id=segtree::query(1,1+partition_size);
			for (int i = 1; i <= n; i++)
				if (!choose_point[i] && tabutable[i] >= iter) {
					int cnt = 0;
					for (int j = head[i]; j; j = e[j].next)
						if (choose_point[e[j].go] &&
						    partition_vertex_by_vertex[e[j].go] != partition_vertex_by_vertex[i])
							cnt++;
					if (book_conflict[partition_vertex_by_vertex[i]] - cnt > new_pri_f) {
						new_pri = i;
						new_pri_f = book_conflict[partition_vertex_by_vertex[i]] - cnt;
					}
				}
			/*
			 * 这里可以用线段树维护全局最小值
			 * 初始化建树是NlogN
			 * 区间查询，单点修改，时间复杂度分别为logN
			 * 再用懒更新的思路维护p个堆，总体维护仍然是logN
			 */
			if (new_pri == -1)
				return ans;
			else {
				//cout<<best_fun-new_pri_f<<" skr"<<endl;
				int i = point_set[partition_vertex_by_vertex[new_pri]];
				for (int j = head[i]; j; j = e[j].next)
					if (choose_point[e[j].go]) {
						best_fun--;
						book_conflict[partition_vertex_by_vertex[e[j].go]]--;
					}
				book_conflict[partition_vertex_by_vertex[i]] = 0;
				choose_point[i] = false;
				tabutable[i] = iter - tl;
				for (int j = head[new_pri]; j; j = e[j].next)
					if (choose_point[e[j].go]) {
						best_fun++;
						book_conflict[partition_vertex_by_vertex[e[j].go]]++;
						book_conflict[partition_vertex_by_vertex[new_pri]]++;
					}
				choose_point[new_pri] = true;
				point_set[partition_vertex_by_vertex[new_pri]] = new_pri;
				if (new_pri_f > 0)
					ans = point_set;
				cout<<best_fun<<endl;
			}
		}
		return ans;
	}

	int dis (vector<int> x, vector<int> y) {
		int cnt = 0;
		for (int i = 0; i < x.size (); i++)
			if (x[i] != y[i])
				cnt++;
		return cnt;
	}

	void optimize () {
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
	}

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
					cnt++;
			book[++tot] = make_pair (tot, cnt);
		}
		sort (book + 1, book + tot + 1, cmp);
		for (int i = 1; i <= number; i++) {
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
		int iter = PCP_LOCAL_SEARCH_ITER, mini_conflict = inf;
		vector<int> tmp, ans, ori;
		int x = rand (0, Parition_Point_Set.size - 1), change_size = max (10, partition_size / 10);
		int ori_conflict = get_conflict (Parition_Point_Set.a[x]);
		tmp = Parition_Point_Set.a[x];
		while (iter--) {
			tmp = localSearch (tmp, 100);
			int conflict = get_conflict (tmp);
			if (conflict < mini_conflict) {
				ans = tmp;
				mini_conflict = conflict;
			}
			/*
			 * 如果得到的解的距离较近或者解的质量不够好时，提升换点的个数
			 */
			if (dis (tmp, ans) <= min (100, partition_size / 3) || abs (ori_conflict - conflict) < partition_size / 5) {
				change_size += 5;
			} else break;
			if (change_size >= partition_size / 2)
				break;
			tmp = random_choose_point (tmp, change_size);//随机换点操作，可以更换为有选择性的换点操作
		}
		Parition_Point_Set.a[x] = ans;
		cout << "fin_connect : " << get_conflict (ans) << endl;
		return ans;
	}

	vector<int> find_point_old (int num) {//无换点扰动
		int x = rand (0, Parition_Point_Set.size - 1);
		Parition_Point_Set.a[Parition_Point_Set.size] = localSearch (Parition_Point_Set.a[x], 200);
		vector<int> tmp = Parition_Point_Set.a[Parition_Point_Set.size];
		optimize ();
		return tmp;
	}

}

