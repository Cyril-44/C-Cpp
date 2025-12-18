#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int mod=998244353;
int n,m,cnte,bfa[100005],keyp[100005],nn,dfn[100005],idx,fa[100005][20],dep[100005],sz[100005];
ll ff[100005][2],f[100005][2],k[100005][2][2],ans;
vector<int> g[100005],g2[100005];
struct edge{
    int x,y;
}e[20];
int find(int x){
    if(x==bfa[x]) return x;
    return bfa[x]=find(bfa[x]);
}
bool cmp(int x,int y){
    return dfn[x]<dfn[y];
}
void dfs(int x,int fth){
    dfn[x]=++idx;
    dep[x]=dep[fth]+1;
    fa[x][0]=fth;
    for(int i=1;i<=18;i++){
        fa[x][i]=fa[fa[x][i-1]][i-1];
    }
    for(int y:g[x]){
        if(y!=fth){
            dfs(y,x);
            sz[x]+=sz[y];
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
void init(int x){
    ff[x][0]=ff[x][1]=1;
    for(int y:g[x]){
        if(y==fa[x][0]||sz[y]) continue;
        init(y);
        ff[x][0]=ff[x][0]*(ff[y][0]+ff[y][1])%mod;
        ff[x][1]=ff[x][1]*ff[y][0]%mod;
    }
}
void work(int x,int son){
    f[x][0]=f[x][1]=1;
    for(int y:g[x]){
        if(y==fa[x][0]||y==son) continue;
        work(y,son);
        f[x][0]=f[x][0]*(f[y][0]+f[y][1])%mod;
        f[x][1]=f[x][1]*f[y][0]%mod;
    }
}
void calc(int x,int fth){
    int xx=x;
    k[x][0][0]=k[x][0][1]=k[x][1][0]=1;
    while(fa[xx][0]!=fth){
        work(fa[xx][0],xx);
        ll k00=(f[fa[xx][0]][0]*k[x][0][0]+f[fa[xx][0]][1]*k[x][1][0])%mod;
        ll k01=(f[fa[xx][0]][0]*k[x][0][1]+f[fa[xx][0]][1]*k[x][1][1])%mod;
        k[x][1][0]=f[fa[xx][0]][0]*k[x][0][0]%mod;
        k[x][1][1]=f[fa[xx][0]][0]*k[x][0][1]%mod;
        k[x][0][0]=k00,k[x][0][1]=k01;
        xx=fa[xx][0];
    }
}
void dfs2(int x,int fth){
    init(x);
    if(x!=fth) calc(x,fth);
    for(int y:g2[x]){
        dfs2(y,x);
    }
}
void work2(int x){
    for(int y:g2[x]){
        work2(y);
        f[x][0]=f[x][0]*(k[y][0][0]*f[y][0]%mod+k[y][0][1]*f[y][1]%mod)%mod;
        f[x][1]=f[x][1]*(k[y][1][0]*f[y][0]%mod+k[y][1][1]*f[y][1]%mod)%mod;
    }
}
int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        bfa[i]=i;
    }
    for(int i=1;i<=m;i++){
        int x,y;
        scanf("%d%d",&x,&y);
        int fx=find(x),fy=find(y);
        if(fx!=fy){
            bfa[fy]=fx;
            g[x].push_back(y);
            g[y].push_back(x);
        }
        else{
            e[++cnte]=(edge){x,y};
            keyp[++nn]=x;
            keyp[++nn]=y;
            sz[x]=sz[y]=1;
        }
    }
    sz[1]=1;
    dfs(1,0);
    keyp[++nn]=1;
    sort(keyp+1,keyp+nn+1,cmp);
    nn=unique(keyp+1,keyp+nn+1)-keyp-1;
    int n2=nn;
    for(int i=1;i<nn;i++){
        int lc=lca(keyp[i],keyp[i+1]);
        keyp[++n2]=lc;
    }
    sort(keyp+1,keyp+n2+1,cmp);
    nn=unique(keyp+1,keyp+n2+1)-keyp-1;
    for(int i=1;i<nn;i++){
        int lc=lca(keyp[i],keyp[i+1]);
        g2[lc].push_back(keyp[i+1]);
    }
    dfs2(1,1);
    for(int i=0;i<(1<<cnte);i++){
        for(int j=1;j<=nn;j++){
            f[keyp[j]][0]=ff[keyp[j]][0],f[keyp[j]][1]=ff[keyp[j]][1];
        }
        for(int j=1;j<=cnte;j++){
            if(i&(1<<j-1)){
                f[e[j].x][0]=f[e[j].y][1]=0;
            }
            else{
                f[e[j].x][1]=0;
            }
        }
        work2(1);
        ans=(ans+f[1][0]+f[1][1])%mod;
    }
    cout<<ans;
    return 0;
}
/*
g[3][0]=f[3][0],g[3][1]=f[3][1]
f[3][0]=g[3][0]*(f[2][0]+f[2][1])
f[2][0]+f[2][1]=g[2][0]*(k00*f[1][0]+k01*f[1][1])+g[2][1]*(k10*f[1][0]+k11*f[1][1])
f[3][0]=(k00*g[2][0]+k10*g[2][1])*f[1][0]+(k01*g[2][0]+k11*g[2][1])*f[1][1]

f[3][1]=g[3][1]*f[2][0]
f[2][0]=g[2][0]*(k00*f[1][0]+k01*f[1][1])
f[3][0]=(k00*g[2][0])*f[1][0]+(k01*g[2][0])*f[1][1]
*/