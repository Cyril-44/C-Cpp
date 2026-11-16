#include <bits/stdc++.h>
using namespace std;
const int N=1000025;
struct T{long long f,w;int g,e,l,c;};
T tr[4*(N+1)];
int a[N+1];
long long ws(int l,int r){return 1ll*(l+r-2)*(r-l+1)/2;}
void bd(int p,int l,int r){
    tr[p].e=r-l+1;tr[p].w=ws(l,r);
    if(l==r)return;
    int m=(l+r)/2;
    bd(p*2,l,m);bd(p*2+1,m+1,r);
}
void pu(int p){
    tr[p].f=tr[p*2].f+tr[p*2+1].f;
    tr[p].g=tr[p*2].g+tr[p*2+1].g;
    tr[p].e=tr[p*2].e+tr[p*2+1].e;
    tr[p].w=tr[p*2].w+tr[p*2+1].w;
}
void ap(int p,int l,int r,int d){
    int n=r-l+1,o=n-tr[p].e,h=d>=0?d/2:(d-1)/2;
    long long w=ws(l,r);
    tr[p].g+=(int)(1ll*h*n);tr[p].f+=1ll*h*w;
    if(d%2){tr[p].g+=tr[p].e;tr[p].f+=tr[p].w+tr[p].e-o;tr[p].e=o;tr[p].w=w-tr[p].w;}
    tr[p].l+=d;
    if(l==r)tr[p].c+=d;
}
void pd(int p,int l,int r){
    int d=tr[p].l;
    if(!d||l==r)return;
    int m=(l+r)/2;
    ap(p*2,l,m,d);ap(p*2+1,m+1,r,d);tr[p].l=0;
}
void ad(int p,int l,int r,int x,int y,int d){
    if(x<=l&&r<=y){ap(p,l,r,d);return;}
    pd(p,l,r);
    int m=(l+r)/2;
    if(x<=m)ad(p*2,l,m,x,y,d);
    if(y>m)ad(p*2+1,m+1,r,x,y,d);
    pu(p);
}
void sc(int p,int l,int r,int x,int v){
    if(l==r){
        tr[p].c=v;tr[p].l=0;tr[p].e=(v%2==0);tr[p].w=tr[p].e?l-1:0;
        tr[p].g=(v+1)/2;tr[p].f=1ll*tr[p].g*(l-1)+(v&1);return;
    }
    pd(p,l,r);
    int m=(l+r)/2;
    if(x<=m)sc(p*2,l,m,x,v);else sc(p*2+1,m+1,r,x,v);
    pu(p);
}
int gc(int p,int l,int r,int x){
    if(l==r)return tr[p].c;
    pd(p,l,r);
    int m=(l+r)/2;
    if(x<=m)return gc(p*2,l,m,x);
    return gc(p*2+1,m+1,r,x);
}
int fb(int p,int l,int r,int x,int o){
    if(r<x)return N+1;
    if(l>=x&&(o?tr[p].e==0:tr[p].e==r-l+1))return N+1;
    if(l==r)return l;
    pd(p,l,r);
    int m=(l+r)/2,z=fb(p*2,l,m,x,o);
    if(z<=N)return z;
    return fb(p*2+1,m+1,r,x,o);
}
void ch(int x,int y){
    int d=y-a[x];a[x]=y;
    if(!d)return;
    int c=gc(1,2,N,x);
    sc(1,2,N,x,c+d);
    int z=(c+d)/2-c/2,i=x+1;
    while(z&&i<=N){
        if(z==1||z==-1){
            int b=fb(1,2,N,i,z==1),e=min(N,b);
            if(i<=e)ad(1,2,N,i,e,z);
            break;
        }
        c=gc(1,2,N,i);sc(1,2,N,i,c+z);
        z=(c+z)/2-c/2;++i;
    }
}
long long qu(int k,int c){
    if(k<=c)return k;
    int z=k-c;
    if(tr[1].g<=z)return c+tr[1].f+1ll*(z-tr[1].g)*N;
    int p=1,l=2,r=N,gg=0;
    long long ff=0;
    while(l<r){
        pd(p,l,r);
        int m=(l+r)/2;
        if(gg+tr[p*2].g>z){p*=2;r=m;}
        else{ff+=tr[p*2].f;gg+=tr[p*2].g;p=p*2+1;l=m+1;}
    }
    return c+ff+1ll*(z-gg)*(l-1);
}
int main(){
    freopen("game.in","r",stdin);freopen("game.out","w",stdout);
    int id,q;if(scanf("%d%d",&id,&q)!=2)return 0;
    bd(1,2,N);
    int c=0;
    while(q--){
        int t,x;scanf("%d%d",&t,&x);
        if(t==1){
            int y;scanf("%d",&y);
            if(x==1)c=y;else ch(x,y);
        }else printf("%lld\n",qu(x,c));
    }
    return 0;
}
