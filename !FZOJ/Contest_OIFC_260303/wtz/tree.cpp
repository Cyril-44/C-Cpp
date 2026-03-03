#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=300010;
struct Data{
    int cnt; ll lft;
    ll p[20];
    void build(){
        lft=0;
        for(int i=2;i<=cnt;i++) lft+=p[i]-p[1];
    }
    void insert(ll x){
        for(int i=1;i<=cnt;i++)
            if(p[i]>x){
                cnt++;
                for(int j=cnt;j>i;j--) p[j]=p[j-1];
                p[i]=x;
                build();
                return;
            }
        p[++cnt]=x; build();
    }
    void erase(ll x){
        for(int i=1;i<=cnt;i++)
            if(p[i]==x){
                for(int j=i;j<cnt;j++) p[j]=p[j+1];
                cnt--;
                build();
                return;
            }
    }
    ll getmin(){
        ll res=lft,now=lft;
        for(int i=2;i<=cnt;i++){
            now+=(p[i]-p[i-1])*(-cnt-2+(i<<1));
            res=min(res,now);
        }
        return res;
    }
}f[N];
void merge(const Data &x,const Data &y,Data &z){
    z.cnt=x.cnt; z.lft=x.lft+y.lft;
    for(int i=1;i<=x.cnt;i++)
        z.p[i]=x.p[i]+y.p[i];
}
#define ls (u<<1)
#define rs (u<<1|1)
int n,m,a[N],q,u,x;
void build(int u){
    if((u<<1)<=m){
        build(ls);
        build(rs);
        merge(f[ls],f[rs],f[u]);
    }
    f[u].insert(a[u]);
}
void upload(int u){
    if(!u) return;
    merge(f[ls],f[rs],f[u]);
    f[u].insert(a[u]);
    upload(u>>1);
}
void modify(int u,int cur){
    f[u].erase(a[u]);
    a[u]=cur;
    f[u].insert(cur);
    upload(u>>1);
}
int main(){
    freopen("tree.in","r",stdin);
    freopen("tree.out","w",stdout);
    scanf("%d",&n); m=(1<<n)-1;
    for(int i=1;i<(1<<n);i++)
        scanf("%d",&a[i]);
    build(1);
    scanf("%d",&q);
    while(q--){
        scanf("%d%d",&u,&x);
        modify(u,x);
        printf("%lld\n",f[1].getmin());
    }
}