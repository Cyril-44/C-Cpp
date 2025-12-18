#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m,k,fa[500005][20],dep[500005],dfn[500005],idx,keyp[1000005],sz[500005],cntp;
int t[500005],c[500005],ans[500005];
vector<int> g[500005],g2[500005],v[500005],keys[500005];
struct node{
    int x,l,r,op;
}p[10000005];
struct qry{
    int x,y,id;
}q[1000005];
void read(int &x){
    x=0;
    char ch=getchar();
    while(ch<'0'||ch>'9'){
        ch=getchar();
    }
    while(ch>='0'&&ch<='9'){
        x=(x<<3)+(x<<1)+ch-48;
        ch=getchar();
    }
}
void print(int x){
    if(!x) return;
    print(x/10);
    putchar(x%10+48);
}
bool cmp(int x,int y){
    return dfn[x]<dfn[y];
}
bool cmpp(node x,node y){
    return x.x<y.x;
}
bool cmpq(qry x,qry y){
    return dfn[x.x]<dfn[y.x];
}
void dfs(int x,int fth){
    dfn[x]=++idx;
    dep[x]=dep[fth]+1;
    fa[x][0]=fth;
    sz[x]=1;
    for(int i=1;i<=19;i++){
        fa[x][i]=fa[fa[x][i-1]][i-1];
    }
    for(int y:g[x]){
        if(y!=fth){
            dfs(y,x);
            sz[x]+=sz[y];
        }
    }
}
int lcson(int x,int y){
    if(dep[x]<dep[y]) swap(x,y);
    for(int i=19;i>=0;i--){
        if(dep[fa[x][i]]>dep[y]){
            x=fa[x][i];
        }
    }
    if(fa[x][0]==y) return x;
    if(dep[x]>dep[y]) x=fa[x][0];
    for(int i=19;i>=0;i--){
        if(fa[x][i]!=fa[y][i]){
            x=fa[x][i];
            y=fa[y][i];
        }
    }
    return x;
}
int lca(int x,int y){
    return fa[lcson(x,y)][0];
}
void dfs2(int rt,int x,int fth,int cnt,int cl){
    cnt+=(c[x]==cl)*(3-t[x]*2);
    if(cnt<0) return;
    if(cnt==0){
        int lcs=lcson(rt,x);
        int lc=fa[lcs][0];
        if(lc==rt){
            p[++cntp]=(node){1,dfn[x],dfn[x]+sz[x]-1,1};
            p[++cntp]=(node){dfn[lcs],dfn[x],dfn[x]+sz[x]-1,-1};
            if(dfn[lcs]+sz[lcs]<=n){
                p[++cntp]=(node){dfn[lcs]+sz[lcs],dfn[x],dfn[x]+sz[x]-1,1};
                p[++cntp]=(node){n+1,dfn[x],dfn[x]+sz[x]-1,-1};
            }
        }
        else if(lc==x){
            p[++cntp]=(node){dfn[rt],1,dfn[lcs]-1,1};
            p[++cntp]=(node){dfn[rt]+sz[rt],1,dfn[lcs]-1,-1};
            if(dfn[lcs]+sz[lcs]<=n){
                p[++cntp]=(node){dfn[rt],dfn[lcs]+sz[lcs],n,1};
                p[++cntp]=(node){dfn[rt]+sz[rt],dfn[lcs]+sz[lcs],n,-1};
            }
        }
        else{
            p[++cntp]=(node){dfn[rt],dfn[x],dfn[x]+sz[x]-1,1};
            p[++cntp]=(node){dfn[rt]+sz[rt],dfn[x],dfn[x]+sz[x]-1,-1};
        }
        return;
    }
    for(int y:g2[x]){
        if(y!=fth){
            dfs2(rt,y,x,cnt,cl);
        }
    }
}
int lowbit(int x){
    return x&(-x);
}
struct segtree{
    int c[500005];
    void update(int x,int k){
        for(int i=x;i<=n;i+=lowbit(i)){
            c[i]+=k;
        }
    }
    int query(int x){
        int cnt=0;
        for(int i=x;i>=1;i-=lowbit(i)){
            cnt+=c[i];
        }
        return cnt;
    }
}seg;
int main(){
    read(n),read(m);
    for(int i=1;i<=n;i++){
        read(t[i]),read(c[i]);
        v[c[i]].push_back(i);
        if(t[i]==1) keys[c[i]].push_back(i);
    }
    for(int i=1;i<n;i++){
        int x,y;
        read(x),read(y);
        g[x].push_back(y);
        g[y].push_back(x);
    }
    dfs(1,0);
    for(int i=1;i<=n;i++){
        if(!v[i].size()) continue;
        keyp[k=1]=1;
        for(int j:v[i]){
            keyp[++k]=j;
        }
        sort(keyp+1,keyp+k+1,cmp);
        int nn=k;
        for(int j=1;j<k;j++){
            int lc=lca(keyp[j],keyp[j+1]);
            keyp[++nn]=lc;
        }
        sort(keyp+1,keyp+nn+1,cmp);
        nn=unique(keyp+1,keyp+nn+1)-keyp-1;
        for(int j=1;j<nn;j++){
            int lc=lca(keyp[j],keyp[j+1]);
            g2[lc].push_back(keyp[j+1]);
            g2[keyp[j+1]].push_back(lc);
        }
        for(int j:keys[i]){
            dfs2(j,j,0,0,i);
        }
        for(int j=1;j<=nn;j++){
            g2[keyp[j]].clear();
        }
    }
    for(int i=1;i<=m;i++){
        read(q[i].x),read(q[i].y);
        q[i].id=i;
    }
    sort(p+1,p+cntp+1,cmpp);
    sort(q+1,q+m+1,cmpq);
    for(int i=1,j=1;i<=m;i++){
        while(j<=cntp&&p[j].x<=dfn[q[i].x]){
            seg.update(p[j].l,p[j].op);
            seg.update(p[j].r+1,-p[j].op);
            j++;
        }
        ans[q[i].id]=seg.query(dfn[q[i].y]);
    }
    for(int i=1;i<=m;i++){
        if(ans[i]) print(ans[i]);
        else putchar('0');
        putchar('\n');
    }
	return 0;
}