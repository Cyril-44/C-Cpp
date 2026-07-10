甜筒有一个无向连通图，边有边权且无重边自环。它包含一条含有全部 $n$ 个点的链，链上的点依次编号为 $1\sim n$。

此外有 $m-n+1$ 条边，保证这些边不会交叉。交叉的定义为：如果两条边 $(u_i,v_i)$ 和 $(u_j,v_j)$ 满足 $\min(u_i,v_i)<\min(u_j,v_j)<\max(u_i,v_i)<\max(u_j,v_j)$，则称这两条发生交叉。

有 $q$ 组询问，每次给定两个点 $x$ 和 $y$ ，求它们之间的最短路。

## 子任务 $1$，$n\le300$

Floyd 预处理即可。

## 子任务 $2$，$n\le5\times10^4,q\le150$

每次跑 Dijkstra 即可，SPFA 会跑的很慢被卡掉，~~关于SPFA，它死了~~。

## 子任务 $3$，数据随机

看到边没有交叉关系，容易想到建出一棵包含关系树，即每条边建一个点，将包含这条边的跨越点数最少的边作为父亲。可以将每条边按 $u$ 为第一关键字，$-v$ 为第二关键字从小到大排序（$u<v$），用单调栈维护当前森林的每个根即可完成建树。为了方便可以加一条 $(1,n,\infty)$ 的边。

由于数据随机，树高是 $O(\log n)$ 的，考虑从一个点不停向父亲跳，那么我们该记录哪些信息呢？可以发现一个点要走出一条边覆盖的范围，一定会经过边的一个端点，因此我们只需要记录起点到当前边的两个端点的最短路。

预处理每条边左右端点间的最短路（只经过子树中的边），对每条边的所有儿子的这个距离记前缀和，我们就可以向父亲跳了。

那么我们将两个点分别跳到 LCA 的儿子的位置，此时可以从这两个儿子较近的两个端点间走，否则一定经过 LCA 到根链上的一条边，枚举经过哪一条，依次更新答案即可。

复杂度 $O((n+q)\log n)$。

## 子任务 $4$，$n\le5\times 10^4$

留给常数大的正解或者多 $\log$ 或根号的神秘做法。

## 子任务 $5$，$w=1$

延续子任务 $3$ 的做法，发现向父亲跳的转移是一个 $(\min,+)$ 矩阵的形式，倍增预处理矩阵乘法即可。又因为 $w=1$，不可能经过 LCA 到根链上 LCA 以外的边，无需继续向上跳。

复杂度 $O((n+q)\log n)$。

## 子任务 $6$，正解

我们的问题在于处理经过 LCA 的祖先的情况，直接从上到下将每条边的边权与绕一圈经过其父亲的距离取最小值即可，这样只需考虑经过 LCA 的情况。

如果被卡常的话可以优化矩阵乘法的计算，大抵会跑得快很多。

复杂度 $O((n+q)\log n)$。

```cpp
#include<iostream>
#include<algorithm>
#include<vector>
#include<cstring>
#define rep(i,l,r) for(int i=(l);i<=(r);i++)
#define per(i,l,r) for(int i=(l);i>=(r);i--)
#define lowbit(x)  ((x)&(-(x)))
using namespace std;
typedef long long ll;
const int maxn=5e5+5,maxm=1e6+5;
const ll inf=0x3f3f3f3f3f3f3f3f;
int st[maxm],jum[maxm][24],dep[maxm],lnk[maxn],tp,in,im,iq;
struct edge{
    int l,r;
    ll w;
    inline friend bool operator<(edge a,edge b){
        if(a.r==b.r){
            if(a.l==b.l)return a.w<b.w;
            return a.l>b.l;
        }
        return a.r<b.r;
    }
}ae[maxm];
struct matrix{
    ll a[2][2];
    inline ll* operator[](int x){return a[x];}
    inline matrix operator*(matrix b){
        matrix res;
        rep(v1,0,1)rep(v2,0,1){
            res[v1][v2]=min(a[v1][0]+b[0][v2],a[v1][1]+b[1][v2]);
        }
        return res;
    }
    inline void print(){
        rep(v1,0,1){
            rep(v2,0,1)cout<<a[v1][v2]<<' ';
            cout<<endl;
        }
    }
}trs[maxm][24];
ll dis[maxm],pre[maxm],suf[maxm],nd[maxm];
vector<int> T[maxm];
inline void build(int x){
    rep(v1,1,22)jum[x][v1]=jum[jum[x][v1-1]][v1-1];
    dis[x]=ae[x].w;
    if(ae[x].l+1==ae[x].r)return;
    for(int v:T[x]){
        dep[v]=dep[x]+1;
        jum[v][0]=x;
        build(v);
    }
    int sz=T[x].size();
    rep(v1,1,sz-1)pre[T[x][v1]]=pre[T[x][v1-1]]+dis[T[x][v1-1]];
    per(v1,sz-2,0)suf[T[x][v1]]=suf[T[x][v1+1]]+dis[T[x][v1+1]];
    dis[x]=min(dis[x],suf[T[x][0]]+dis[T[x][0]]);
    for(int v:T[x]){
        matrix &m=trs[v][0];
        m[0][0]=min(pre[v],dis[v]+suf[v]+dis[x]);
        m[1][0]=min(pre[v]+dis[v],suf[v]+dis[x]);
        m[0][1]=min(pre[v]+dis[x],dis[v]+suf[v]);
        m[1][1]=min(suf[v],pre[v]+dis[v]+dis[x]);
    }
}
inline void pushdown(int x){
    for(int v:T[x]){
        nd[v]=min(dis[v],pre[v]+suf[v]+nd[x]);
        pushdown(v);
    }
}
int main(){
    freopen("chain.in","r",stdin);
    freopen("chain.out","w",stdout);
    cin>>in>>im>>iq;
    rep(v1,1,im){
        scanf("%d %d %lld",&ae[v1].l,&ae[v1].r,&ae[v1].w);
        if(ae[v1].l>ae[v1].r)swap(ae[v1].l,ae[v1].r);
    }
    ae[++im]={1,in,inf};
    sort(ae+1,ae+im+1);
    int tt=0;
    rep(v1,1,im)if(ae[v1].r!=ae[v1-1].r||ae[v1].l!=ae[v1-1].l){
        ae[++tt]=ae[v1];
        while(tp&&ae[st[tp]].l>=ae[v1].l)T[tt].push_back(st[tp--]);
        reverse(T[tt].begin(),T[tt].end());
        st[++tp]=tt;
        if(ae[v1].l+1==ae[v1].r)lnk[ae[v1].l]=lnk[ae[v1].r]=tt;
    }
    dep[st[1]]=1;
    build(st[1]);
    nd[st[1]]=dis[st[1]];
    pushdown(st[1]);
    rep(v1,1,22)rep(v2,1,tt)trs[v2][v1]=trs[v2][v1-1]*trs[jum[v2][v1-1]][v1-1];
    while(iq--){
        int ix,iy;
        scanf("%d %d",&ix,&iy);
        int x=lnk[ix],y=lnk[iy];
        if(dep[x]>dep[y]){
            swap(x,y);
            swap(ix,iy);
        }
        matrix mx,my;
        mx[0][0]=my[0][0]=0;
        mx[0][1]=nd[x];
        my[0][1]=nd[y];
        if(ae[x].r==ix)swap(mx[0][0],mx[0][1]);
        if(ae[y].r==iy)swap(my[0][0],my[0][1]);
        per(v1,22,0)if(dep[jum[y][v1]]>=dep[x]){
            my=my*trs[y][v1];
            y=jum[y][v1];
        }
        if(x==y){
            ll ans=min(mx[0][0]+my[0][0],mx[0][1]+my[0][1]);
            ans=min(ans,mx[0][0]+my[0][1]+nd[x]);
            ans=min(ans,mx[0][1]+my[0][0]+nd[x]);
            printf("%lld\n",ans);
            continue;
        }
        per(v1,22,0)if(jum[x][v1]!=jum[y][v1]){
            mx=mx*trs[x][v1];
            my=my*trs[y][v1];
            x=jum[x][v1];
            y=jum[y][v1];
        }
        if(ae[x].l>ae[y].l){
            swap(x,y);
            swap(mx,my);
        }
        matrix ma=mx*trs[x][0],mb=my*trs[y][0];
        ll ans=min(ma[0][0]+mb[0][0],ma[0][1]+mb[0][1]);
        ans=min(ans,mx[0][1]+my[0][0]+pre[y]-pre[x]-dis[x]);
        ans=min(ans,mx[0][0]+my[0][1]+pre[x]+suf[y]+nd[jum[x][0]]);
        printf("%lld\n",ans);
    }
    return 0;
}

```