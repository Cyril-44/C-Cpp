#include<bits/stdc++.h>
using namespace std;
int n,m,cnt[1005],nw,dis[1005],ans;
bool g[1005][1005],flag[1005][1005],vis[1005];
queue<int> q;
void bfs(int x){
	memset(vis,0,sizeof(vis));
	q.push(x);
	dis[x]=0;
	vis[x]=1;
	ans=0;
	while(!q.empty()){
		x=q.front();
		q.pop();
		for(int y=1;y<=n;y++){
			if(g[x][y]&&!vis[y]){
				vis[y]=1;
				dis[y]=dis[x]+1;
				q.push(y);
				ans+=dis[y];
			}
		}
	}
}
int main(){
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		for(int j=i+1;j<=n;j++){
			g[i][j]=1;
		}
	}
	nw=1;
	int xx=1e9;
	while(m--){
		int x,y;
		scanf("%d%d",&x,&y);
		g[x][y]^=1,g[y][x]^=1;
		if(x>y) swap(x,y);
		if(y==1) xx=min(xx,x);
		flag[x][y]^=1;
		if(flag[x][y]) cnt[x]++;
		else cnt[x]--;
		if(!cnt[x]) nw=min(nw,x);
		while(cnt[nw]) nw++;
		bfs(1);
		int anss=ans;
		bfs(nw);
		anss=min(anss,ans);
		bfs(3);
		anss=min(anss,ans);
		int nww=nw;
		for(int i=2;i<=anss-(n-1);){
			nww++;
			if(nww>n) break;
			bfs(nww);
			anss=min(anss,ans);
			if(!cnt[nww]) i++;
		}
		printf("%d\n",anss);
	}
	return 0;
}