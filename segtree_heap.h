//
// Created by 张淇 on 2018/11/8.
//

#ifndef HYBIRD_SEGTREE_HEAP_H
#define HYBIRD_SEGTREE_HEAP_H

int pre_array[maxp];

segtree f[maxp*4];

priority_queue<int> h[maxp];

namespace segtree{
	void pushup(int i){
		if(f[i*2+1].w.first>f[i*2].w.first){
			f[i].w=f[i*2+1].w;
		}
		else{
			f[i].w=f[i*2].w;
		}
	}

	void update(int i,int x)
	{
		f[i].w.first=x;
		return;
	}

	void build(int i,int left,int right){
		int mid=(left+right)/2;
		f[i].w.first=0;f[i].w.second=0;f[i].l=left;f[i].r=right;
		if(left==right){
			f[i].w.first=pre_array[left];
			f[i].w.second=left;
			return;
		}
		build(i*2,left,mid);
		build(i*2+1,mid+1,right);
		pushup(i);
	}

	void change(int i,int left,int right,int v){
		int mid=(f[i].l+f[i].r)/2;
		if(f[i].l==left&&f[i].r==right){
			update(i,v);
			return;
		}
		if(mid>=right)change(i*2,left,right,v);
		else if(mid<left)change(i*2+1,left,right,v);
		else change(i*2,left,mid,v),change(i*2+1,mid+1,right,v);
		pushup(i);
	}

	pair<int,int> query(int i,int left,int right){
		int mid=(f[i].l+f[i].r)/2;
		if(f[i].l==left&&f[i].r==right) return f[i].w;
		pushdown(i);
		if(mid>=right) return query(i*2,left,right);
		if(mid<left) return query(i*2+1,left,right);
		pair<int,int> left_ans= query(i*2,left,mid),right_ans=query(i*2+1,mid+1,right);
		return left_ans.w>right_ans.w?left_ans:right_ans;
	}

}

#endif //HYBIRD_SEGTREE_HEAP_H
