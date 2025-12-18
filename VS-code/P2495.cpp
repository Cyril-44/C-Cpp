#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m,k,fa[250005][20],dep[250005],mn[250005][20],dfn[250005],idx,keyp[500005];
ll f[250005][2];
bool flag[250005];
struct node{
    int y,w;
};
vector<node> g[250005];
vector<int> g2[250005];
bool cmp(int x,int y){
    return dfn[x]<dfn[y];
}
void dfs(int x,int fth,int w){
    dfn[x]=++idx;
    dep[x]=dep[fth]+1;
    fa[x][0]=fth;
    mn[x][0]=w;
    for(int i=1;i<=18;i++){
        fa[x][i]=fa[fa[x][i-1]][i-1];
        mn[x][i]=min(mn[x][i-1],mn[fa[x][i-1]][i-1]);
    }
    for(node y:g[x]){
        if(y.y!=fth){
            dfs(y.y,x,y.w);
        }
    }
}
int lca(int x,int y){
    if(dep[x]<dep[y]) swap(x,y);
    for(int i=18;i>=0;i--){
        if(dep[fa[x][i]]>=dep[y]){
            x=fa[x][i];
        }
    }
    if(x==y) return x;
    for(int i=18;i>=0;i--){
        if(fa[x][i]!=fa[y][i]){
            x=fa[x][i];
            y=fa[y][i];
        }
    }
    return fa[x][0];
}
int calc(int x,int tar){
    int cnt=1e9;
    for(int i=18;i>=0;i--){
        if(dep[fa[x][i]]>=dep[tar]){
            cnt=min(cnt,mn[x][i]);
            x=fa[x][i];
        }
    }
    return cnt;
}
void dfs2(int x){
    f[x][0]=f[x][1]=0;
    if(flag[x]) f[x][1]=1e9;
    for(int y:g2[x]){
        dfs2(y);
        int w=calc(y,x);
        ll tmp0=min(f[x][0]+min(f[y][0],f[y][1]),f[x][1]+f[y][0]);
        f[x][1]+=min(f[y][0]+w,f[y][1]);
        f[x][0]=tmp0;
    }
}
int main(){
    cin>>n;
    for(int i=1;i<n;i++){
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);
        g[x].push_back((node){y,w});
        g[y].push_back((node){x,w});
    }
    dfs(1,0,0);
    cin>>m;
    while(m--){
        scanf("%d",&k);
        for(int i=1;i<=k;i++){
            scanf("%d",&keyp[i]);
            flag[keyp[i]]=1;
        }
        keyp[++k]=1;
        sort(keyp+1,keyp+k+1,cmp);
        int nn=k;
        for(int i=1;i<k;i++){
            int lc=lca(keyp[i],keyp[i+1]);
            keyp[++nn]=lc;
        }
        sort(keyp+1,keyp+nn+1,cmp);
        nn=unique(keyp+1,keyp+nn+1)-keyp-1;
        for(int i=1;i<nn;i++){
            int lc=lca(keyp[i],keyp[i+1]);
            g2[lc].push_back(keyp[i+1]);
        }
        dfs2(1);
        printf("%lld\n",f[1][1]);
        for(int i=1;i<=nn;i++){
            flag[keyp[i]]=0;
            g2[keyp[i]].clear();
        }
    }
	return 0;
}