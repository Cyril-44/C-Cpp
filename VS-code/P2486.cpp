#include<bits/stdc++.h>
using namespace std;
int n,m,a[100005],fa[100005],dep[100005],sz[100005],son[100005],top[100005],dfn[100005],idx,rk[100005];
vector<int> g[100005];
struct qry{
    int sum,cl,cr;
};
void dfs(int x,int fth){
    fa[x]=fth;
    dep[x]=dep[fth]+1;
    sz[x]=1;
    int mx=0;
    for(int y:g[x]){
        if(y==fth) continue;
        dfs(y,x);
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
        if(y==fa[x]||y==son[x]) continue;
        dfs2(y,y);
    }
}
struct segtree{
    int sum[400005],cl[400005],cr[400005],lzy[400005];
    void push_up(int x){
        sum[x]=sum[x<<1]+sum[x<<1|1]-(cr[x<<1]==cl[x<<1|1]);
        cl[x]=cl[x<<1],cr[x]=cr[x<<1|1];
    }
    void push_down(int x){
        if(!lzy[x]) return;
        sum[x<<1]=sum[x<<1|1]=1;
        cl[x<<1]=cr[x<<1]=lzy[x<<1]=lzy[x];
        cl[x<<1|1]=cr[x<<1|1]=lzy[x<<1|1]=lzy[x];
        lzy[x]=0;
    }
    void build(int x,int l,int r){
        if(l==r){
            sum[x]=1;
            cl[x]=cr[x]=a[rk[l]];
            return;
        }
        int mid=l+r>>1;
        build(x<<1,l,mid);
        build(x<<1|1,mid+1,r);
        push_up(x);
    }
    void update(int x,int l,int r,int L,int R,int c){
        if(l>=L&&r<=R){
            sum[x]=1;
            cl[x]=cr[x]=lzy[x]=c;
            return;
        }
        push_down(x);
        int mid=l+r>>1;
        if(mid>=L) update(x<<1,l,mid,L,R,c);
        if(mid<R) update(x<<1|1,mid+1,r,L,R,c);
        push_up(x);
    }
    qry query(int x,int l,int r,int L,int R){
        if(l>=L&&r<=R){
            return (qry){sum[x],cl[x],cr[x]};
        }
        push_down(x);
        int mid=l+r>>1;
        qry tmp;
        if(mid>=L){
            tmp=query(x<<1,l,mid,L,R);
            if(mid<R){
                qry tmpp=query(x<<1|1,mid+1,r,L,R);
                tmp.sum+=tmpp.sum-(tmp.cr==tmpp.cl);
                tmp.cr=tmpp.cr;
            }
        }
        else if(mid<R){
            tmp=query(x<<1|1,mid+1,r,L,R);
        }
        return tmp;
    }
}seg;
void update(int x,int y,int c){
    while(top[x]!=top[y]){
        if(dep[top[x]]<dep[top[y]]) swap(x,y);
        seg.update(1,1,n,dfn[top[x]],dfn[x],c);
        x=fa[top[x]];
    }
    if(dep[x]<dep[y]) swap(x,y);
    seg.update(1,1,n,dfn[y],dfn[x],c);
}
int query(int x,int y){
    int sum=0,xc=0,yc=0;
    while(top[x]!=top[y]){
        if(dep[top[x]]>dep[top[y]]){
            qry tmp=seg.query(1,1,n,dfn[top[x]],dfn[x]);
            sum+=tmp.sum-(tmp.cr==xc);
            xc=tmp.cl;
            x=fa[top[x]];
        }
        else{
            qry tmp=seg.query(1,1,n,dfn[top[y]],dfn[y]);
            sum+=tmp.sum-(tmp.cr==yc);
            yc=tmp.cl;
            y=fa[top[y]];
        }
    }
    if(dep[x]>dep[y]){
        qry tmp=seg.query(1,1,n,dfn[y],dfn[x]);
        sum+=tmp.sum-(tmp.cl==yc)-(tmp.cr==xc);
    }
    else{
        qry tmp=seg.query(1,1,n,dfn[x],dfn[y]);
        sum+=tmp.sum-(tmp.cl==xc)-(tmp.cr==yc);
    }
    return sum;
}
int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    for(int i=1;i<n;i++){
        int x,y;
        scanf("%d%d",&x,&y);
        g[x].push_back(y);
        g[y].push_back(x);
    }
    dfs(1,0);
    dfs2(1,1);
    seg.build(1,1,n);
    while(m--){
        char op[2];
        scanf("%s",op);
        if(op[0]=='C'){
            int x,y,c;
            scanf("%d%d%d",&x,&y,&c);
            update(x,y,c);
        }
        else{
            int x,y;
            scanf("%d%d",&x,&y);
            printf("%d\n",query(x,y));
        }
    }
    return 0;
}