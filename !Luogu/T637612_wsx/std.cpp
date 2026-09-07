#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

int n, g;
const int MAXV = 10002; // cover values 0..10000, mex up to 10001
int L; // number of 64-bit blocks per bitset

vector<vector<int>> adj;
vector<int> parent_, depth_, heavy, sz;
vector<int> head, pos;
int curPos;

void dfs1(int u, int p){
    parent_[u]=p;
    depth_[u]= (p==-1?0:depth_[p]+1);
    sz[u]=1;
    int maxsz=0;
    heavy[u]=-1;
    for(int v: adj[u]){
        if(v==p) continue;
        dfs1(v,u);
        sz[u]+=sz[v];
        if(sz[v]>maxsz){
            maxsz=sz[v];
            heavy[u]=v;
        }
    }
}

void dfs2(int u, int h){
    head[u]=h;
    pos[u]=curPos++;
    if(heavy[u]!=-1) dfs2(heavy[u], h);
    for(int v: adj[u]){
        if(v==parent_[u] || v==heavy[u]) continue;
        dfs2(v, v);
    }
}

int segSize;
vector<ull> seg; // flat array: node i has block offset i*L .. i*L+L-1

inline ull* seg_ptr(int idx){
    return &seg[(size_t)idx * L];
}

void seg_build(const vector<vector<ull>>& base){
    // segSize = n, nodes indexed [1..segSize-1] internal, leaves at [segSize .. segSize+n-1] if using 1-based
    // We'll use iterative segtree with base index = segSize
    // seg vector length = 2*segSize * L
    seg.assign((size_t)2*segSize * L, 0ULL);
    // copy leaves
    for(int i=0;i<n;i++){
        ull* dst = seg_ptr(segSize + i);
        const vector<ull>& src = base[i];
        for(int j=0;j<L;j++) dst[j]=src[j];
    }
    // build internal
    for(int i=segSize-1;i>0;i--){
        ull* dst = seg_ptr(i);
        ull* lch = seg_ptr(i<<1);
        ull* rch = seg_ptr(i<<1|1);
        for(int j=0;j<L;j++) dst[j] = lch[j] | rch[j];
    }
}

// OR src into dst (both length L)
inline void or_into(ull* dst, const ull* src){
    for(int i=0;i<L;i++) dst[i] |= src[i];
}

// query interval [l,r] inclusive on base positions 0..n-1
void seg_query_range(int l, int r, vector<ull>& out){
    if(l>r) return;
    int Lidx = l + segSize;
    int Ridx = r + segSize + 1;
    // temporary buffers for left and right accumulations to avoid repeated OR into out
    // but we can OR directly into out
    while(Lidx < Ridx){
        if(Lidx & 1){
            or_into(out.data(), seg_ptr(Lidx));
            Lidx++;
        }
        if(Ridx & 1){
            --Ridx;
            or_into(out.data(), seg_ptr(Ridx));
        }
        Lidx >>= 1;
        Ridx >>= 1;
    }
}

// query path u-v, OR result into out
void query_path(int u, int v, vector<ull>& out){
    while(head[u] != head[v]){
        if(depth_[head[u]] < depth_[head[v]]) swap(u,v);
        int h = head[u];
        seg_query_range(pos[h], pos[u], out);
        u = parent_[h];
    }
    if(depth_[u] > depth_[v]) swap(u,v);
    seg_query_range(pos[u], pos[v], out);
}

// find mex from bitset out
int find_mex(const vector<ull>& out){
    for(int i=0;i<L;i++){
        ull word = out[i];
        // mask off bits beyond MAXV in last word
        if(i == L-1){
            int rem = MAXV % 64;
            if(rem != 0){
                ull mask = (rem==64?~0ULL:((1ULL<<rem)-1ULL));
                word |= (~mask); // set beyond bits to 1 so they are ignored
            }
        }
        if(~word){
            ull inv = ~word;
            if(inv){
                int bit = __builtin_ctzll(inv);
                int res = i*64 + bit;
                if(res < MAXV) return res;
            }
        }
    }
    return MAXV; // should not happen
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> g;
    adj.assign(n, {});
    for(int i=0;i<n-1;i++){
        int u,v; cin >> u >> v; --u; --v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    // read node values
    vector<vector<int>> vals(n);
    for(int i=0;i<n;i++){
        int k; cin >> k;
        vals[i].resize(k);
        for(int j=0;j<k;j++) cin >> vals[i][j];
    }
    // HLD init
    parent_.assign(n, -1);
    depth_.assign(n, 0);
    heavy.assign(n, -1);
    sz.assign(n, 0);
    head.assign(n, 0);
    pos.assign(n, 0);
    curPos = 0;
    dfs1(0, -1);
    dfs2(0, 0);
    // prepare base bitsets in HLD order
    L = (MAXV + 63) / 64;
    vector<vector<ull>> base(n, vector<ull>(L, 0ULL));
    for(int u=0;u<n;u++){
        int p = pos[u];
        for(int v: vals[u]){
            if(v < 0 || v >= MAXV) continue;
            int idx = v >> 6;
            int off = v & 63;
            base[p][idx] |= (1ULL << off);
        }
    }
    // build iterative segtree with segSize = n
    segSize = 1;
    // we can use segSize = n (iterative segtree supports non-power-of-two)
    segSize = n;
    seg_build(base);
    // queries
    int q; cin >> q;
    int lastans = 0;
    while(q--){
        long long s1,t1;
        int p;
        cin >> s1 >> t1 >> p;
        int s = (int)(( (s1 - 1 + 1LL * lastans * g) % p ) + 1) - 1;
        int t = (int)(( (t1 - 1 + 1LL * lastans * g) % p ) + 1) - 1;
        // prepare out bitset
        vector<ull> out(L, 0ULL);
        query_path(s, t, out);
        int ans = find_mex(out);
        cout << ans << '\n';
        lastans = ans;
    }
    return 0;
}
