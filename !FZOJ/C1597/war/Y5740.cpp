#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int mod=998244353;
int tid,t,n,m,fa[305],a[305];
ll f[305][3005],tmp[3005],sz[305];//f[i][j]表示前i个用了j个的方案数
vector<int> g[305];
void work0(){
	ll ans=1;
	for(int i=2;i<=n;i++){
		ans=ans*(a[i]+1)%mod;
	}
	printf("%lld\n",ans);
}
void dfs(int x){
	f[x][0]=1;
	sz[x]=0;
	for(int y:g[x]){
		dfs(y);
		memset(tmp,0,sizeof(tmp));
		for(int i=0;i<=sz[x];i++){
			for(int j=0;j<=sz[y];j++){
				tmp[i+j]=(tmp[i+j]+f[x][i]*f[y][j])%mod;
			}
		}
		memcpy(f[x],tmp,sizeof(tmp));
		sz[x]+=sz[y];
	}
	sz[x]+=a[x];
	for(int i=1;i<=sz[x];i++){
		f[x][i]=(f[x][i]+f[x][i-1])%mod;
	}
}
void work1(){
	memset(f,0,sizeof(f));
	dfs(1);
	printf("%lld\n",f[1][sz[1]]);
}
int main(){
	cin>>tid>>t;
	while(t--){
		cin>>n>>m;
		for(int i=1;i<=n;i++) g[i].clear();
		for(int i=2;i<=n;i++){
			scanf("%d",&fa[i]);
			g[fa[i]].push_back(i);
		}
		for(int i=1;i<=n;i++){
			scanf("%d",&a[i]);
		}
		if(tid==4||tid==11||tid==19||tid==20) work0();
		else if(tid<=2||tid==6||tid==7||tid==14||tid==15) work1();
	}
	return 0;
}