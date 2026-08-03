
#include<bits/stdc++.h>
using namespace std;
int t,n,m,cl[200005];
struct node{
	int y,w;
};
vector<node> g[200005];
bool check(int x){
	int l=g[x].size();
	for(int i=0;i<l;i++){
		for(int j=i+1;j<l;j++){
			if(cl[x]==cl[g[x][i].y]&&cl[x]==cl[g[x][j].y]) return 0;
		}
		int y=g[x][i].y;
		if(cl[x]!=cl[y]) continue;
		for(node z:g[y]){
			if(cl[y]==cl[z.y]&&g[x][i].w!=z.w){
				return 0;
			}
		}
	}
	return 1;
}
bool dfs(int x){
	for(node yy:g[x]){
		int y=yy.y;
		if(!~cl[y]){
			cl[y]=cl[x]^1;
			bool ok=0;
			if(check(y)) ok=dfs(y);
			if(!ok){
				cl[y]=cl[x];
				if(check(x)&&check(y)) ok=dfs(y);
			}
			if(!ok){
				cl[y]=-1;
				return 0;
			}
		}
	}
	return 1;
}
int main(){
	cin>>t;
	while(t--){
		scanf("%d%d",&n,&m);
		for(int i=1;i<=n;i++){
			g[i].clear();
			cl[i]=-1;
		}
		for(int i=1;i<=m;i++){
			int x,y;
			scanf("%d%d",&x,&y);
			g[x].push_back({y,i});
			g[y].push_back({x,i});
		}
		bool ok=1;
		for(int i=1;i<=n;i++){
			if(!~cl[i]){
				cl[i]=0;
				if(!dfs(i)){
					ok=0;
					break;
				}
			}
		}
		if(!ok) puts("GG");
		else{
			for(int i=1;i<=n;i++){
				printf("%d ",cl[i]);
			}
			printf("\n");
		}
	}
	return 0;
}