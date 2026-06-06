#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int mod=1e9+7;
int n,sz[200005],dep[200005],fa[200005],son[200005],mxdep[105],a[105];
ll f[105][105][105],C[105][105];
vector<int> g[200005];
void dfs(int x,int fth){
	sz[x]=1;
	fa[x]=fth;
	dep[x]=dep[fth]+1;
	for(int y:g[x]){
		if(y!=fth){
			dfs(y,x);
			sz[x]+=sz[y];
		}
	}
}
int dfs2(int x){
	int mx=dep[x];
	for(int y:g[x]){
		if(y==fa[x]) continue;
		mx=max(mx,dfs2(y));
	}
	return mx;
}
int main(){
	C[0][0]=1;
	for(int i=1;i<=100;i++){
		C[i][0]=C[i][i]=1;
		for(int j=1;j<i;j++){
			C[i][j]=(C[i-1][j-1]+C[i-1][j])%mod;
		}
	}
	cin>>n;
	for(int i=1;i<n;i++){
		int x,y;
		scanf("%d%d",&x,&y);
		g[x].push_back(y);
		g[y].push_back(x);
	}
	dfs(1,0);
	int nn=0,mxid=0;
	for(int i=1;i<=n;i++){
		if(nn<dep[i]){
			nn=dep[i];
			mxid=i;
		}
	}
	for(int x=mxid,i=nn;x;x=fa[x],i--){
		son[fa[x]]=x;
		for(int y:g[x]){
			if(y!=fa[x]&&y!=son[x]){
				mxdep[i]=max(mxdep[i],dfs2(y));
			}
		}
	}
	int ma=0;
	for(int i=1;i<=nn;i++){
		a[i]=max(0,ma-i+1);
		ma=max(ma,mxdep[i]);
		for(int p=a[i];p<=nn;p++){
			f[i][i][p]=1;
		}
	}
	for(int l=2;l<=nn;l++){
		for(int i=1;i+l-1<=nn;i++){
			int j=i+l-1;
			for(int p=a[i];p<=nn;p++){
				f[i][j][p]=f[i][j-1][p+1];
				for(int k=i+1;k<=j;k++){
					if(a[k]>p) continue;
					if(k==j) (f[i][j][p]+=f[i][k-1][p])%=mod;
					else (f[i][j][p]+=f[i][k-1][p]*f[k][j-1][p+1]%mod*C[j-i][k-i])%=mod;
				}
			}
		}
	}
	cout<<nn<<" "<<f[1][nn][0]<<endl;
	return 0;
}