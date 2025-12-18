#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m,k,dfn[200005],dep[200005],fa[200005][20],sz[200005],hvson[200005],top[200005],rk[200005],idx;
vector<int> g[200005];
ll ans;
struct node{
	int x,id;
};
void dfs(int x){
	sz[x]=1;
    dep[x]=dep[fa[x][0]]+1;
    for(int i=1;i<=18;i++){
        fa[x][i]=fa[fa[x][i-1]][i-1];
    }
	for(int y:g[x]){
        if(!y) continue;
		dfs(y);
		sz[x]+=sz[y];
	}
}
void dfs2(int x,int tp){
	dfn[x]=++idx;
    rk[idx]=x;
	top[x]=tp;
	int mx=0,mxid=0;
	for(int y:g[x]){
        if(!y) continue;
		if(mx<sz[y]){
			mx=sz[y];
			mxid=y;
		}
	}
	hvson[x]=mxid;
	if(mx){
        ans+=mxid;
        dfs2(mxid,tp);
    }
	for(int y:g[x]){
        if(!y) continue;
		if(y!=mxid){
			dfs2(y,y);
		}
	}
}
struct segtree{
    ll c[200005];
    int lowbit(int x){
        return x&(-x);
    }
    void update(int x,int k){
        for(int i=x;i<=n;i+=lowbit(i)){
            c[i]+=k;
        }
    }
    ll query(int x){
        ll cnt=0;
        for(int i=x;i>=1;i-=lowbit(i)){
            cnt+=c[i];
        }
        return cnt;
    }
}seg;
void update(int x){
	while(x){
		seg.update(dfn[top[x]],-1);
        seg.update(dfn[x]+1,1);
		x=fa[top[x]][0];
	}
}
int main(){
	cin>>n;
    for(int i=1;i<=n;i++){
        g[i].resize(3);
    }
	for(int i=1;i<=n;i++){
		int x,y;
		scanf("%d%d",&x,&y);
        if(x){
            g[i][0]=x;
            if(y) g[i][1]=y;
        }
        else if(y) g[i][0]=y;
        fa[x][0]=fa[y][0]=i;
	}
    fa[0][0]=0;
	dfs(1);
	dfs2(1,1);
    for(int i=1;i<=n;i++){
        seg.update(dfn[i],sz[i]);
        seg.update(dfn[i]+1,-sz[i]);
    }
    cout<<ans<<endl;
    cin>>m;
    while(m--){
        int x;
        scanf("%d",&x);
        update(x);
        int rt=1;
        while(1){
            int nw=x;
            for(int i=18;i>=0;i--){
                int nww=fa[nw][i];
                if(nww&&seg.query(dfn[nww])*2<=seg.query(dfn[rt])){
                    nw=nww;
                }
            }
            if(nw==rt) break;
            if(hvson[fa[nw][0]]==nw&&(seg.query(dfn[nw])==0||seg.query(dfn[nw])*2<seg.query(dfn[fa[nw][0]])-1)){
                hvson[fa[nw][0]]=g[fa[nw][0]][0]+g[fa[nw][0]][1]-nw;
                ans+=g[fa[nw][0]][0]+g[fa[nw][0]][1]-nw-nw;
            }
            rt=nw;
        }
        if(g[fa[x][0]][0]==x){
            hvson[fa[x][0]]=g[fa[x][0]][1];
            g[fa[x][0]][0]=0;
        }
        else{
            hvson[fa[x][0]]=g[fa[x][0]][0];
            g[fa[x][0]][1]=0;
        }
        printf("%lld\n",ans);
    }
	return 0;
}