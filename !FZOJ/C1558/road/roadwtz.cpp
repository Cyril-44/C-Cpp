#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const int N=500010;
namespace seg{
    int a[N<<2];
    #define ls (u<<1)
    #define rs (u<<1|1)
    void pushup(int u){a[u]=max(a[ls],a[rs]);}
    void build(int l,int r,int u,int n){
        if(l==r){
            a[u]=n-l;
            return;
        }
        int m=l+r>>1;
        build(l,m,ls,n); build(m+1,r,rs,n);
        pushup(u);
        // printf("%d %d %d\n",l,r,a[u]);
    }
    void update(int l,int r,int t,int u,int x){
        if(l==r){
            a[u]+=x;
            return;
        }
        int m=l+r>>1;
        if(t<=m) update(l,m,t,ls,x);
        else update(m+1,r,t,rs,x);
        pushup(u);
    }
    int query(){return a[1];}
};
int n,m,nn,u,v;
map<pii,int> mp;
int main(){
    scanf("%d%d",&n,&m);
    nn=min(n,m*2+2);
    seg::build(1,nn,1,n);
    while(m--){
        scanf("%d%d",&u,&v);
        if(u>v) swap(u,v);
        if(!mp.count({u,v})) seg::update(1,nn,u,1,-1),seg::update(1,nn,v,1,1),mp[{u,v}]=1;
        else if(mp[{u,v}]==0) seg::update(1,nn,u,1,-1),seg::update(1,nn,v,1,1),mp[{u,v}]=1;
        else  seg::update(1,nn,v,1,-1),seg::update(1,nn,u,1,1),mp[{u,v}]=0;
        printf("%d\n",(n-1)*2-seg::query());
    }
}