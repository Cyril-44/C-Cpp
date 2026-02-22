#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,ma,a[100005],t[100005];
vector<int> v[100005];
ll m;
unordered_map<ll,bool> mp;
struct node{
	int l,r;
};
bool operator <(const node &x,const node &y){
	for(int i=1;i<=ma;i++){
		int w1=lower_bound(v[i].begin(),v[i].end(),x.l)-v[i].begin();
		int w2=upper_bound(v[i].begin(),v[i].end(),x.r)-v[i].begin()-1;
		int w3=lower_bound(v[i].begin(),v[i].end(),y.l)-v[i].begin();
		int w4=upper_bound(v[i].begin(),v[i].end(),y.r)-v[i].begin()-1;
		if(w2-w1!=w4-w3){
			return w2-w1<w4-w3;
		}
	}
	return 0;
}
priority_queue<node> q;
int main(){
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		scanf("%d",&a[i]);
		ma=max(ma,a[i]);
		v[a[i]].push_back(i);
	}
	q.push({1,n});
	mp[n]=1;
	for(int i=1;i<=m;i++){
		node x=q.top();
		q.pop();
		if(x.l<x.r){
			if(!mp.count(1ll*(x.l-1)*n+x.r-1)){
				q.push({x.l,x.r-1});
				mp[1ll*(x.l-1)*n+x.r-1]=1;
			}
			if(!mp.count(1ll*x.l*n+x.r)){
				q.push({x.l+1,x.r});
				mp[1ll*x.l*n+x.r]=1;
			}
		}
	}
	node x=q.top();
	for(int i=x.l;i<=x.r;i++){
		t[a[i]]++;
	}
	for(int i=1;i<=ma;i++){
		for(int j=1;j<=t[i];j++){
			printf("%d ",i);
		}
	}
	return 0;
}
/*
1 n
1,n-1
2,n

1 2 1 3 5 1 5 1 3 7 5 

枚举每个数出现次数，

1 1 1
1 1
1
2 2 2 2
2 2 2
2 2
2
3 3
3
4 4 4
4 4
4
*/