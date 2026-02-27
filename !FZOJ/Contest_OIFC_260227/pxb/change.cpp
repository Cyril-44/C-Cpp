#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int oo=1e9;
int n,m,s,t,cur[1605];
ll dis[1605],cnt;
bool vis[1605];
struct node{
	int y,c,w,id;
};
vector<node> g[1605];
queue<int> q;
int spfa(){
	memset(dis,0x3f,sizeof(dis));
	memset(cur,0,sizeof(cur));
	q.push(s);
	dis[s]=0;
	while(!q.empty()){
		int x=q.front();
		q.pop();
		int l=g[x].size();
		for(int i=0;i<l;i++){
			if(g[x][i].c&&dis[x]+g[x][i].w<dis[g[x][i].y]){
				q.push(g[x][i].y);
				dis[g[x][i].y]=dis[x]+g[x][i].w;
			}
		}
	}
	return dis[t]<oo;
}
ll dfs(int x,int lim){
	if(x==t) return lim;
	int l=g[x].size(),sum=0;
	vis[x]=1;
	for(int i=cur[x];i<l;i++){
		cur[x]=i;
		int y=g[x][i].y;
		if(dis[y]==dis[x]+g[x][i].w&&g[x][i].c&&!vis[y]){
			int tmp=dfs(y,min(g[x][i].c,lim));
			g[x][i].c-=tmp;
			g[y][g[x][i].id].c+=tmp;
			cnt+=tmp? g[x][i].w:0;
			lim-=tmp;
			sum+=tmp;
			if(!lim) break;
		}
	}
	vis[x]=0;
	return sum;
}
int dinic(){
	cnt=0;
	int sum=0,fl;
	while(spfa()){
		while(fl=dfs(s,oo)){
			sum+=fl;
		}
	}
	return sum;
}
void add(int x,int y,int c,int w){
	int id1=g[x].size(),id2=g[y].size();
	g[x].push_back((node){y,c,w,id2});
	g[y].push_back((node){x,0,-w,id1});
}
int main(){
	cin>>n;
	s=0,t=n+1;
	for(int i=1;i<=n;i++){
		int x;
		scanf("%d",&x);
		if(x>=0) add(s,i,x,0);
		else add(i,t,-x,0);
		if(i>1) add(i,i-1,oo,1);
		if(i<n) add(i,i+1,oo,1);
	}
	dinic();
	cout<<cnt;
	return 0;
}