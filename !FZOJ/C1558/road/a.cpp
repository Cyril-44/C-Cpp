#include <bits/stdc++.h>
using namespace std;

struct node{
	int ld,rd,l,r;
	int d;
}tree[8500005];
int cnt,n,m;
int add(int d,int l,int r){
	tree[++cnt].d=d;
	tree[cnt].l=l,tree[cnt].r=r;
	return cnt;
}
map<pair<int,int>,int>mp;
void pushup(int w){
	int mid=(tree[w].l+tree[w].r)/2;
	if (tree[w].ld==0) tree[w].ld=add(n-tree[w].l,tree[w].l,mid);
	if (tree[w].rd==0) tree[w].rd=add(n-mid-1,mid+1,tree[w].r);
	tree[w].d=max(tree[tree[w].ld].d,tree[tree[w].rd].d);
}
void update(int w,int l,int g){
	if (tree[w].l==tree[w].r){
		tree[w].d+=g;
		return;
	}
	int mid=(tree[w].l+tree[w].r)/2;
	if (tree[w].ld==0) tree[w].ld=add(n-tree[w].l,tree[w].l,mid);
	if (tree[w].rd==0) tree[w].rd=add(n-mid-1,mid+1,tree[w].r);
	if (l<=mid) update(tree[w].ld,l,g);
	else update(tree[w].rd,l,g); 
	pushup(w);
}
int main(){
//	freopen("ex_road5.in","r",stdin);
//	freopen("ex_road.out","w",stdout);
	ios::sync_with_stdio(false);
	cin.tie(0);
	cout.tie(0);
	mp.clear();
	cin>>n>>m;
	cnt++;
	tree[cnt].l=1,tree[cnt].r=n;
	tree[cnt].d=n-1;
	for(int i=1;i<=m;i++){
		int u,v;
		cin>>u>>v;
		if (u>v) swap(u,v);
		if (mp[make_pair(u,v)]%2==0){
			update(1,u,-1);
			update(1,v,1);
		}
		else{
			update(1,u,1);
			update(1,v,-1);
		}
		mp[make_pair(u,v)]++;
		long long g=tree[1].d;
	//	cout<<g<<endl;
		cout<<n-1+n-1-g<<endl;
	}
	return 0;
}
