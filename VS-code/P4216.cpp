#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> p;
int n,m,rt,fa[200005],dep[200005],sz[200005],son[200005],top[200005],dfn[200005],idx,rk[200005];
bool flag[200005];
vector<int> g[200005];
struct qry{
    int id,op,l,r,ti,cnt,ans;
}q[200005];
bool cmpti(qry x,qry y){
    if(x.ti==y.ti) return x.op>y.op;
    return x.ti<y.ti;
}
bool cmpid(qry x,qry y){
    return x.id<y.id;
}
void dfs(int x){
    dep[x]=dep[fa[x]]+1;
    sz[x]=1;
    int mx=0;
    for(int y:g[x]){
        dfs(y);
        if(mx<sz[y]){
            mx=sz[y];
            son[x]=y;
        }
        sz[x]+=sz[y];
    }
}
void dfs2(int x,int tp){
    top[x]=tp;
    dfn[x]=++idx;
    rk[idx]=x;
    if(son[x]) dfs2(son[x],tp);
    for(int y:g[x]){
        if(y==son[x]) continue;
        dfs2(y,y);
    }
}
int lowbit(int x){
    return x&(-x);
}
struct segtree{
    int c[200005];
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
p query(int x,int y){
    int cnt=0,sum=0;
    while(top[x]!=top[y]){
        if(dep[top[x]]<dep[top[y]]) swap(x,y);
        cnt+=seg.query(dfn[x])-seg.query(dfn[top[x]]-1);
        sum+=dep[x]-dep[top[x]]+1;
        x=fa[top[x]];
    }
    if(dep[x]<dep[y]) swap(x,y);
    cnt+=seg.query(dfn[x])-seg.query(dfn[y]-1);
    sum+=dep[x]-dep[y]+1;
    return make_pair(sum,cnt);
}
int main(){
    cin>>n;
    for(int i=1;i<=n;i++){
        scanf("%d",&fa[i]);
        if(fa[i]==0) rt=i;
        else g[fa[i]].push_back(i);
    }
    dfs(rt);
    dfs2(rt,rt);
    cin>>m;
    for(int i=1;i<=m;i++){
        scanf("%d",&q[i].op);
        if(q[i].op==1){
            scanf("%d%d%d",&q[i].l,&q[i].r,&q[i].ti);
            q[i].ti=max(i-q[i].ti-1,0);
        }
        else{
            scanf("%d",&q[i].l);
            q[i].ti=i;
        }
        q[i].id=i;
    }
    sort(q+1,q+m+1,cmpti);
    for(int i=0,j=1;i<=m;i++){
        while(j<=m&&q[j].ti==i&&q[j].op==2){
            if(!flag[q[j].l]){
                seg.update(dfn[q[j].l],1);
                flag[q[j].l]=1;
            }
            j++;
        }
        while(j<=m&&q[j].ti==i&&q[j].op==1){
            p tmp=query(q[j].l,q[j].r);
            q[j].cnt=tmp.first,q[j].ans=tmp.second;
            j++;
        }
    }
    sort(q+1,q+m+1,cmpid);
    for(int i=1;i<=m;i++){
        if(q[i].op==1){
            printf("%d %d\n",q[i].cnt,q[i].ans);
        }
    }
    return 0;
}