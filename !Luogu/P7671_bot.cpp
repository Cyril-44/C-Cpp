#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 100000 + 5;
const int MOD = 20160501;

int addmod(int a,int b){ a+=b; if(a>=MOD) a-=MOD; if(a<0) a+=MOD; return a; }
int mulmod(ll a,ll b){ return int((a*b)%MOD); }
int powmod(int a,int e=MOD-2){
    ll r=1, x=a;
    while(e){
        if(e&1) r=r*x%MOD;
        x=x*x%MOD; e>>=1;
    }
    return int(r);
}
int inv2;

int n,m;
vector<int> g[MAXN];
int a[MAXN];

// HLD arrays
int sz[MAXN], son[MAXN], fa[MAXN], dep[MAXN];
int top[MAXN], dfn[MAXN], rnk_[MAXN], timer_;
void dfs1(int u,int f){
    fa[u]=f; dep[u]= (f?dep[f]+1:0);
    sz[u]=1; son[u]=0;
    for(int v:g[u]) if(v!=f){
        dfs1(v,u);
        sz[u]+=sz[v];
        if(sz[v]>sz[son[u]]) son[u]=v;
    }
}
void dfs2(int u,int tp){
    top[u]=tp; dfn[u]=++timer_; rnk_[timer_]=u;
    if(son[u]) dfs2(son[u],tp);
    for(int v:g[u]) if(v!=fa[u] && v!=son[u]) dfs2(v,v);
}

// LCA via top/dep
int lca(int x,int y){
    while(top[x]!=top[y]){
        if(dep[top[x]]>dep[top[y]]) x=fa[top[x]];
        else y=fa[top[y]];
    }
    return dep[x]<dep[y]?x:y;
}

// Segment tree on dfn order
struct Node{
    int s0,s1,s2; // dynamic sums: sum a, sum a*dep, sum a*dep^2
    int sumDep, sumDep2; // static for this segment
    int len;
    int lazy; // add to a_i
    Node():s0(0),s1(0),s2(0),sumDep(0),sumDep2(0),len(0),lazy(0){}
} seg[4*MAXN];

int depArr[MAXN];
int dep2Arr[MAXN];

void build(int idx,int l,int r){
    seg[idx].lazy=0;
    seg[idx].len = r-l+1;
    if(l==r){
        int u = rnk_[l];
        seg[idx].sumDep = depArr[u] % MOD;
        seg[idx].sumDep2 = dep2Arr[u] % MOD;
        seg[idx].s0 = a[u] % MOD;
        seg[idx].s1 = mulmod(a[u]%MOD, seg[idx].sumDep);
        seg[idx].s2 = mulmod(a[u]%MOD, seg[idx].sumDep2);
        return;
    }
    int mid=(l+r)>>1;
    build(idx<<1,l,mid);
    build(idx<<1|1,mid+1,r);
    seg[idx].sumDep = addmod(seg[idx<<1].sumDep, seg[idx<<1|1].sumDep);
    seg[idx].sumDep2 = addmod(seg[idx<<1].sumDep2, seg[idx<<1|1].sumDep2);
    seg[idx].s0 = addmod(seg[idx<<1].s0, seg[idx<<1|1].s0);
    seg[idx].s1 = addmod(seg[idx<<1].s1, seg[idx<<1|1].s1);
    seg[idx].s2 = addmod(seg[idx<<1].s2, seg[idx<<1|1].s2);
}

void apply_add(int idx,int val){
    // add val to each a_i in this node's segment
    int v = (val%MOD+MOD)%MOD;
    seg[idx].lazy = addmod(seg[idx].lazy, v);
    seg[idx].s0 = addmod(seg[idx].s0, mulmod(v, seg[idx].len));
    seg[idx].s1 = addmod(seg[idx].s1, mulmod(v, seg[idx].sumDep));
    seg[idx].s2 = addmod(seg[idx].s2, mulmod(v, seg[idx].sumDep2));
}

void pushdown(int idx){
    if(seg[idx].lazy){
        apply_add(idx<<1, seg[idx].lazy);
        apply_add(idx<<1|1, seg[idx].lazy);
        seg[idx].lazy=0;
    }
}

void pullup(int idx){
    seg[idx].s0 = addmod(seg[idx<<1].s0, seg[idx<<1|1].s0);
    seg[idx].s1 = addmod(seg[idx<<1].s1, seg[idx<<1|1].s1);
    seg[idx].s2 = addmod(seg[idx<<1].s2, seg[idx<<1|1].s2);
}

void range_add(int idx,int l,int r,int ql,int qr,int val){
    if(ql>r || qr<l) return;
    if(ql<=l && r<=qr){
        apply_add(idx, val);
        return;
    }
    pushdown(idx);
    int mid=(l+r)>>1;
    range_add(idx<<1,l,mid,ql,qr,val);
    range_add(idx<<1|1,mid+1,r,ql,qr,val);
    pullup(idx);
}

struct Sum3 { int s0,s1,s2; };
Sum3 range_query(int idx,int l,int r,int ql,int qr){
    if(ql>r || qr<l) return {0,0,0};
    if(ql<=l && r<=qr) return {seg[idx].s0, seg[idx].s1, seg[idx].s2};
    pushdown(idx);
    int mid=(l+r)>>1;
    Sum3 L = range_query(idx<<1,l,mid,ql,qr);
    Sum3 R = range_query(idx<<1|1,mid+1,r,ql,qr);
    return { addmod(L.s0,R.s0), addmod(L.s1,R.s1), addmod(L.s2,R.s2) };
}

// apply path add (u->v) by HLD
void path_add(int u,int v,int w){
    while(top[u]!=top[v]){
        if(dep[top[u]] < dep[top[v]]) swap(u,v);
        range_add(1,1,n, dfn[top[u]], dfn[u], w);
        u = fa[top[u]];
    }
    if(dep[u] > dep[v]) swap(u,v);
    // now u is lca on this chain
    range_add(1,1,n, dfn[u], dfn[v], w);
}

// query path segments but we need to collect segments separately for two sides
// We'll provide two helpers: query_up(u, anc) returns sum on path u->anc (inclusive) where anc is ancestor of u
Sum3 query_up(int u,int anc){
    Sum3 res{0,0,0};
    while(top[u]!=top[anc]){
        Sum3 t = range_query(1,1,n, dfn[top[u]], dfn[u]);
        res.s0 = addmod(res.s0, t.s0);
        res.s1 = addmod(res.s1, t.s1);
        res.s2 = addmod(res.s2, t.s2);
        u = fa[top[u]];
    }
    Sum3 t = range_query(1,1,n, dfn[anc], dfn[u]);
    res.s0 = addmod(res.s0, t.s0);
    res.s1 = addmod(res.s1, t.s1);
    res.s2 = addmod(res.s2, t.s2);
    return res;
}

// query on path x->y as required by problem
int query_xy(int x,int y){
    int L = lca(x,y);
    int D = dep[y];
    // part A: nodes on path L..y (inclusive) -> dist = D - h
    Sum3 partA = query_up(y, L); // this collects nodes from y up to L
    // coefficients for T(D - h) = (h^2 * A + h * B + C)
    // T = ( (D-h)*(D-h+1) )/2 = (h^2 - (2D+1)h + (D^2+D))/2
    int Acoef = inv2 % MOD; // 1/2
    int Bcoef = mulmod( (MOD - ( (2LL*D+1)%MOD ) )%MOD, inv2 ); // -(2D+1)/2
    int Ccoef = mulmod( ( (1LL*D*D + D) % MOD ), inv2 ); // (D^2+D)/2

    int ans = 0;
    ans = addmod(ans, mulmod(Acoef, partA.s2));
    ans = addmod(ans, mulmod(Bcoef, partA.s1));
    ans = addmod(ans, mulmod(Ccoef, partA.s0));

    // part B: nodes on path x..L excluding L -> dist = h + D - 2*dep[L] = h + C
    if(x!=L){
        Sum3 partB = query_up(x, L==x?x: (fa[L]==0?L: (L)) ); 
        // But query_up(x,L) includes L; we need exclude L.
        // So we query x->L and then subtract L's contribution.
        // Simpler: query_up(x, L) then subtract node L.
        Sum3 tmp = query_up(x, L);
        // subtract node L
        Sum3 nodeL = range_query(1,1,n, dfn[L], dfn[L]);
        tmp.s0 = addmod(tmp.s0, MOD - nodeL.s0);
        tmp.s1 = addmod(tmp.s1, MOD - nodeL.s1);
        tmp.s2 = addmod(tmp.s2, MOD - nodeL.s2);
        // now tmp is sum over x..L excluding L
        int C = (D - 2*dep[L]);
        C %= MOD; if(C<0) C+=MOD;
        // T(h + C) = ( (h+C)*(h+C+1) )/2 = (h^2 + h*(2C+1) + C*(C+1))/2
        int A2 = inv2;
        int B2 = mulmod( ( (2LL*C + 1) % MOD ), inv2 );
        int C2 = mulmod( ( (1LL*C*(C+1))%MOD ), inv2 );
        ans = addmod(ans, mulmod(A2, tmp.s2));
        ans = addmod(ans, mulmod(B2, tmp.s1));
        ans = addmod(ans, mulmod(C2, tmp.s0));
    }
    return ans;
}

// But above we called query_up(x,L) twice; optimize by using once. For clarity and correctness we keep as is.

struct Update{
    int x,y,w;
    Update(){}
    Update(int _x,int _y,int _w):x(_x),y(_y),w(_w){}
};
vector<Update> updates; // 1-based index for type1 ops
int curT = 0; // current applied count of type1 ops

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    inv2 = (MOD+1)/2;

    cin>>n>>m;
    for(int i=1;i<=n-1;i++){
        int u,v; cin>>u>>v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    for(int i=1;i<=n;i++) cin>>a[i];

    // prepare HLD
    dfs1(1,0);
    timer_=0;
    dfs2(1,1);
    // prepare dep arrays for segment tree static sums
    for(int i=1;i<=n;i++){
        depArr[i] = dep[i] % MOD;
        dep2Arr[i] = (1LL*dep[i]*dep[i])%MOD;
    }
    // build segment tree on initial a[]
    build(1,1,n);

    updates.push_back(Update()); // dummy so updates[1] is first
    curT = 0;
    int lastans = 0;

    for(int iop=0;iop<m;iop++){
        int typ; cin>>typ;
        if(typ==1){
            int Xp,Yp,Delta; cin>>Xp>>Yp>>Delta;
            int X = Xp ^ lastans;
            int Y = Yp ^ lastans;
            int W = Delta ^ lastans;
            updates.push_back(Update(X,Y,W));
            curT = (int)updates.size() - 1;
            path_add(X,Y,W);
        }else if(typ==2){
            int Xp,Yp; cin>>Xp>>Yp;
            int X = Xp ^ lastans;
            int Y = Yp ^ lastans;
            int ans = query_xy(X,Y);
            cout<<ans<<"\n";
            lastans = ans % MOD;
        }else if(typ==3){
            int Xp; cin>>Xp;
            int X = Xp ^ lastans;
            // restore to after X-th type1 op
            // X can be 0 (meaning initial)
            // move curT to X by applying or undoing updates
            while(curT < X){
                curT++;
                Update &u = updates[curT];
                path_add(u.x, u.y, u.w);
            }
            while(curT > X){
                Update &u = updates[curT];
                path_add(u.x, u.y, (MOD - (u.w%MOD))%MOD); // add -w
                curT--;
            }
        }
    }
    return 0;
}
