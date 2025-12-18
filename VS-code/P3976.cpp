#include<bits/stdc++.h>
using namespace std;
int n,m,a[50005],fa[50005],dep[50005],sz[50005],son[50005],top[50005],dfn[50005],idx,rk[50005];
vector<int> g[50005];
struct qry{
    int dif1,dif2,mn,mx;
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
    int dif1[200005],dif2[200005],mn[200005],mx[200005],lzy[200005];
    void push_up(int x){
        dif1[x]=max(max(dif1[x<<1],dif1[x<<1|1]),mx[x<<1|1]-mn[x<<1]);
        dif2[x]=max(max(dif2[x<<1],dif2[x<<1|1]),mx[x<<1]-mn[x<<1|1]);
        mn[x]=min(mn[x<<1],mn[x<<1|1]);
        mx[x]=max(mx[x<<1],mx[x<<1|1]);
    }
    void push_down(int x){
        mn[x<<1]+=lzy[x],mx[x<<1]+=lzy[x],lzy[x<<1]+=lzy[x];
        mn[x<<1|1]+=lzy[x],mx[x<<1|1]+=lzy[x],lzy[x<<1|1]+=lzy[x];
        lzy[x]=0;
    }
    void build(int x,int l,int r){
        mn[x]=1e9;
        if(l==r){
            mn[x]=mx[x]=a[rk[l]];
            return;
        }
        int mid=l+r>>1;
        build(x<<1,l,mid);
        build(x<<1|1,mid+1,r);
        push_up(x);
    }
    void update(int x,int l,int r,int L,int R,int w){
        if(l>=L&&r<=R){
            mn[x]+=w;
            mx[x]+=w;
            lzy[x]+=w;
            return;
        }
        push_down(x);
        int mid=l+r>>1;
        if(mid>=L) update(x<<1,l,mid,L,R,w);
        if(mid<R) update(x<<1|1,mid+1,r,L,R,w);
        push_up(x);
    }
    qry query(int x,int l,int r,int L,int R){
        if(l>=L&&r<=R){
            return (qry){dif1[x],dif2[x],mn[x],mx[x]};
        }
        push_down(x);
        int mid=l+r>>1;
        qry tmp;
        if(mid>=L){
            tmp=query(x<<1,l,mid,L,R);
            if(mid<R){
                qry tmpp=query(x<<1|1,mid+1,r,L,R);
                tmp.dif1=max(max(tmp.dif1,tmpp.dif1),tmpp.mx-tmp.mn);
                tmp.dif2=max(max(tmp.dif2,tmpp.dif2),tmp.mx-tmpp.mn);
                tmp.mn=min(tmp.mn,tmpp.mn);
                tmp.mx=max(tmp.mx,tmpp.mx);
            }
        }
        else if(mid<R){
            tmp=query(x<<1|1,mid+1,r,L,R);
        }
        return tmp;
    }
}seg;
void update(int x,int y,int w){
    while(top[x]!=top[y]){
        if(dep[top[x]]<dep[top[y]]) swap(x,y);
        seg.update(1,1,n,dfn[top[x]],dfn[x],w);
        x=fa[top[x]];
    }
    if(dep[x]<dep[y]) swap(x,y);
    seg.update(1,1,n,dfn[y],dfn[x],w);
}
int query(int x,int y){
    int dif=0,xmn=1e9,ymx=0;
    while(top[x]!=top[y]){
        if(dep[top[x]]>dep[top[y]]){
            qry tmp=seg.query(1,1,n,dfn[top[x]],dfn[x]);
            dif=max(max(dif,tmp.dif2),max(ymx-tmp.mn,tmp.mx-xmn));
            xmn=min(xmn,tmp.mn);
            x=fa[top[x]];
        }
        else{
            qry tmp=seg.query(1,1,n,dfn[top[y]],dfn[y]);
            dif=max(max(dif,tmp.dif1),max(tmp.mx-xmn,ymx-tmp.mn));
            ymx=max(ymx,tmp.mx);
            y=fa[top[y]];
        }
    }
    if(dep[x]>dep[y]){
        qry tmp=seg.query(1,1,n,dfn[y],dfn[x]);
        dif=max(max(dif,tmp.dif2),max(max(tmp.mx-xmn,ymx-tmp.mn),ymx-xmn));
    }
    else{
        qry tmp=seg.query(1,1,n,dfn[x],dfn[y]);
        dif=max(max(dif,tmp.dif1),max(max(tmp.mx-xmn,ymx-tmp.mn),ymx-xmn));
    }
    return dif;
}
int main(){
    cin>>n;
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
    cin>>m;
    while(m--){
        int x,y,w;
        scanf("%d%d%d",&x,&y,&w);
        printf("%d\n",query(x,y));
        update(x,y,w);
    }
    return 0;
}