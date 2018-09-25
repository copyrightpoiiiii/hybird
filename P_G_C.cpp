//
// Created by 张淇 on 2018/8/31.
//

#include "P_G_C.h"

int book_conflict[maxn];

namespace P_P {
    void init_point(int size, vector<int> *b, int num) {
        PP.size = size;
        bool record_point[maxp];
        int rec_poi[maxn];
        vector<int> poi;
        int i = 1;
        while (size--) {
            poi.insert(poi.begin(),array_form_1_to_n+1,array_form_1_to_n+num+1);
            memset(record_point, 0, sizeof(record_point));
            while (!poi.empty()) {
                i = rand(0, (int) poi.size() - 1);
                int po = -1, rec_num = inf;
                for (auto j:b[poi[i]]) {
                    int rec_num_tmp = 0;
                    for (int k = head[j]; k; k = e[k].next)
                        if (record_point[e[k].go])
                            rec_num_tmp++;
                    if (rec_num_tmp < rec_num) {
                        rec_num = rec_num_tmp;
                        po = j;
                    }
                }
                record_point[po] = true;
                rec_poi[poi[i]] = po;
                poi.erase(poi.begin() + i);
            }
            PP.a[size].insert(PP.a[size].begin(), rec_poi, rec_poi + p + 1);
        }
    }


    vector<int> crossover(vector<int> x, vector<int> y) {
        int choose_side = 0,choose_point[maxn];
        memset(choose_point, 0, sizeof(choose_point));
        bool choose[maxp],choose_poi[maxn];
        memset(choose,0,sizeof(choose));
        memset(choose_poi,0,sizeof(choose_poi));
        vector<int> a,tmp;
        a.clear();tmp.clear();
        tmp.insert(tmp.begin(),array_form_1_to_n+1,array_form_1_to_n+x.size());
        shuffle(tmp.begin(),tmp.end(),default_random_engine((unsigned int)time(nullptr)));
        for(int i=0;i<tmp.size();i++){
            int min_ind=inf,min_ind2=inf,poi=-1;
            if(choose_side){
                for(auto j:tmp)
                    if(!choose[j]){
                        int cnt=0,cnt2=0;
                        for(int k=head[x[j]];k;k=e[k].next) {
                            if (choose_poi[e[k].go])
                                cnt++;
                            cnt2++;
                        }
                        if(cnt<min_ind){
                            min_ind=cnt;
                            min_ind2=cnt2;
                            poi=x[j];
                        }
                        else if(cnt==min_ind&&min_ind2>cnt2){
                            min_ind2=cnt2;
                            poi=x[j];
                        }
                }
            }
            else{
                for(auto j:tmp)
                    if(!choose[j]){
                        int cnt=0,cnt2=0;
                        for(int k=head[y[j]];k;k=e[k].next) {
                            if (choose_poi[e[k].go])
                                cnt++;
                            cnt2++;
                        }
                        if(cnt<min_ind){
                            min_ind=cnt;
                            min_ind2=cnt2;
                            poi=y[j];
                        }
                        else if(cnt==min_ind&&min_ind2>cnt2){
                            min_ind2=cnt2;
                            poi=y[j];
                        }
                    }
            }
            choose_point[pro[poi]]=poi;
            choose[pro[poi]]=true;
            choose_poi[poi]=true;
            choose_side^=1;
        }
        a.insert(a.begin(),choose_point,choose_point+x.size());
        return a;
    }

    int find(vector<int> a) {
        int sum = 0;
        bool choose_point[maxn];
        memset(choose_point, 0, sizeof(choose_point));
        memset(book_conflict, 0, sizeof(book_conflict));
        for (auto i:a)
            choose_point[i] = true;
        for (auto j:a)
            for (int i = head[j]; i; i = e[i].next)
                if (choose_point[e[i].go]) {
                    book_conflict[pro[e[i].go]]++;
                    sum++;
                }
        return sum / 2;
    }

    vector<int> localSearch(vector<int> a, int iter) {
        int best_fun = find(a);
        int tabutable[maxn];
        memset(tabutable, 0x3f, sizeof(tabutable));
        bool choose_point[maxn];
        memset(choose_point, 0, sizeof(choose_point));
        for (auto i:a)
            choose_point[i] = true;
        while (iter--) {
            int tl, new_pri = -1, new_pri_f = 0;
            tl = rand(0, A) + (int) (arf * best_fun);
            for (int i = 1; i <= n; i++)
                if (!choose_point[i] && tabutable[i] >= iter) {
                    int cnt = 0;
                    for (int j = head[i]; j; j = e[j].next)
                        if (choose_point[e[j].go] && pro[e[j].go] != pro[i])
                            cnt++;
                    if (book_conflict[pro[i]] - cnt >= new_pri_f) {
                        new_pri = i;
                        new_pri_f = book_conflict[pro[i]] - cnt;
                    }
                }
            if (new_pri == -1)
                return a;
            else {
                int i = a[pro[new_pri]];
                for (int j = head[i]; j; j = e[j].next)
                    if (choose_point[e[j].go]) {
                        best_fun--;
                        book_conflict[pro[e[j].go]]--;
                    }
                book_conflict[pro[i]] = 0;
                choose_point[i] = false;
                tabutable[i] = iter - tl;
                for (int j = head[new_pri]; j; j = e[j].next)
                    if (choose_point[e[j].go]) {
                        best_fun++;
                        book_conflict[pro[e[j].go]]++;
                        book_conflict[pro[new_pri]]++;
                    }
                choose_point[new_pri] = true;
                a[pro[new_pri]] = new_pri;
            }
        }
        return a;
    }

    int dis(vector<int> x, vector<int> y) {
        int cnt = 0;
        for (int i = 0; i < x.size(); i++)
            if (x[i] != y[i])
                cnt++;
        return cnt;
    }

    void optimize() {
        long double s_gene[PP.size + 5];
        for (int i = 0; i <= PP.size; i++)
            s_gene[i] = find(PP.a[i]);
        int min_dis[maxn];
        memset(min_dis, 0x3f, sizeof(min_dis));
        for (int i = 0; i < PP.size; i++)
            for (int j = 0; j < PP.size; j++)
                if (i != j)
                    min_dis[i] = min(min_dis[i], dis(PP.a[i], PP.a[j]));
        long double max_index = 0;
        int max_id = PP.size;
        for (int i = 1; i < PP.size; i++) {
            s_gene[i] += (double)min_dis[i] *p/2;
            if (s_gene[i] > max_index) {
                max_index = s_gene[i];
                max_id = i;
            }
        }
        PP.a[max_id] = PP.a[PP.size];
    }

    vector<int> find_point(int num) {
       // for (int i = 0; i < PP.size; i++)
       //     cout << "init_connect: " << find(PP.a[i]) << endl;
        cout<<p<<endl;
        for(int i=0;i<PP.size;i++)
            for(int j=0;j<PP.size;j++)
                if(i!=j)
                    cout<<i<<" "<<j<<" "<<dis(PP.a[i],PP.a[j])<<endl;
        int x = rand(0, PP.size - 1), y = rand(0, PP.size - 1);
        while (y == x)
            y = rand(0, PP.size - 1);
        PP.a[PP.size] = localSearch(PP.a[x],1000);
        cout << "connect: " << find(PP.a[PP.size]) << endl;
        vector<int> tmp = PP.a[PP.size];
        optimize();
        return tmp;
    }

}
