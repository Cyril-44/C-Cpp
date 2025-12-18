#include<bits/stdc++.h>
#define ll unsigned long long
using namespace std;
ll MAX;
int n,m,k,id[100005],fa[100005],dep[100005],sz[100005],son[100005],top[100005],dfn[100005],idx,rk[100005];
ll a[100005];
vector<int> g[100005];
struct qry{
    ll lf0,lf1,rf0,rf1;
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
ll op(ll x,ll y,int opid){
    if(opid==1) return x&y;
    if(opid==2) return x|y;
    return x^y;
}
struct segtree{
    ll lf0[400005],lf1[400005],rf0[400005],rf1[400005];
    void push_up(int x){
        lf0[x]=(lf0[x<<1]&lf1[x<<1|1])|(~lf0[x<<1]&lf0[x<<1|1]);
        lf1[x]=(lf1[x<<1]&lf1[x<<1|1])|(~lf1[x<<1]&lf0[x<<1|1]);
        rf0[x]=(rf0[x<<1|1]&rf1[x<<1])|(~rf0[x<<1|1]&rf0[x<<1]);
        rf1[x]=(rf1[x<<1|1]&rf1[x<<1])|(~rf1[x<<1|1]&rf0[x<<1]);
    }
    void build(int x,int l,int r){
        if(l==r){
            lf0[x]=rf0[x]=op(0,a[rk[l]],id[rk[l]]);
            lf1[x]=rf1[x]=op(MAX,a[rk[l]],id[rk[l]]);
            return;
        }
        int mid=l+r>>1;
        build(x<<1,l,mid);
        build(x<<1|1,mid+1,r);
        push_up(x);
    }
    void update(int x,int l,int r,int w,int opid,ll z){
        if(l==r){
            lf0[x]=rf0[x]=op(0,z,opid);
            lf1[x]=rf1[x]=op(MAX,z,opid);
            return;
        }
        int mid=l+r>>1;
        if(mid>=w) update(x<<1,l,mid,w,opid,z);
        else update(x<<1|1,mid+1,r,w,opid,z);
        push_up(x);
    }
    qry query(int x,int l,int r,int L,int R){
        if(l>=L&&r<=R){
            return (qry){lf0[x],lf1[x],rf0[x],rf1[x]};
        }
        int mid=l+r>>1;
        qry tmp;
        if(mid>=L){
            tmp=query(x<<1,l,mid,L,R);
            if(mid<R){
                qry tmpp=query(x<<1|1,mid+1,r,L,R),tmp2;
                tmp2.lf0=(tmp.lf0&tmpp.lf1)|(~tmp.lf0&tmpp.lf0);
                tmp2.lf1=(tmp.lf1&tmpp.lf1)|(~tmp.lf1&tmpp.lf0);
                tmp2.rf0=(tmpp.rf0&tmp.rf1)|(~tmpp.rf0&tmp.rf0);
                tmp2.rf1=(tmpp.rf1&tmp.rf1)|(~tmpp.rf1&tmp.rf0);
                tmp=tmp2;
            }
        }
        else if(mid<R){
            tmp=query(x<<1|1,mid+1,r,L,R);
        }
        return tmp;
    }
}seg;
void work(int y,int lc,ll &f0,ll &f1){
    if(top[y]!=top[lc]){
        work(fa[top[y]],lc,f0,f1);
        qry tmp=seg.query(1,1,n,dfn[top[y]],dfn[y]);
        f0=(f0&tmp.lf1)|(~f0&tmp.lf0);
        f1=(f1&tmp.lf1)|(~f1&tmp.lf0);
    }
    else{
        qry tmp=seg.query(1,1,n,dfn[lc],dfn[y]);
        f0=(f0&tmp.lf1)|(~f0&tmp.lf0);
        f1=(f1&tmp.lf1)|(~f1&tmp.lf0);
    }
}
ll query(int x,int y,ll z){
    int xx=x,yy=y,lc;
    while(top[x]!=top[y]){
        if(dep[top[x]]>dep[top[y]]){
            x=fa[top[x]];
        }
        else{
            y=fa[top[y]];
        }
    }
    if(dep[x]>dep[y]){
        lc=y;
    }
    else{
        lc=x;
    }
    x=xx,y=yy;
    ll f0=0,f1=MAX;
    while(top[x]!=top[lc]){
        qry tmp=seg.query(1,1,n,dfn[top[x]],dfn[x]);
        f0=(f0&tmp.rf1)|(~f0&tmp.rf0);
        f1=(f1&tmp.rf1)|(~f1&tmp.rf0);
        x=fa[top[x]];
    }
    if(x!=lc){
        qry tmp=seg.query(1,1,n,dfn[son[lc]],dfn[x]);
        f0=(f0&tmp.rf1)|(~f0&tmp.rf0);
        f1=(f1&tmp.rf1)|(~f1&tmp.rf0);
    }
    work(y,lc,f0,f1);
    ll cnt=0,ans=0;
    for(int i=k-1;i>=0;i--){
        if((f0>>i)&1){
            ans|=1llu<<i;
        }
        else if(((f1>>i)&1)&&(cnt|(1llu<<i))<=z){
            cnt|=1llu<<i;
            ans|=1llu<<i;
        }
    }
    return ans;
}
int main(){
    MAX--;
    cin>>n>>m>>k;
    for(int i=1;i<=n;i++){
        scanf("%d%llu",&id[i],&a[i]);
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
        int op,x,y;
        ll z;
        scanf("%d%d%d%llu",&op,&x,&y,&z);
        if(op==1){
            printf("%llu\n",query(x,y,z));
        }
        else{
            seg.update(1,1,n,dfn[x],y,z);
        }
    }
    return 0;
}