#include <bits/stdc++.h>
using namespace std;

struct State {
    long long down; // farthest distance from this node within open component
    long long diam; // diameter of open component
    long long val;  // sum of closed components' diameters + cut-edge weights inside processed part
};

struct BIT {
    int n;
    vector<long long> bit;
    BIT(int n=0): n(n), bit(n+1, LLONG_MIN) {}
    void reset(int n_) { n=n_; bit.assign(n+1, LLONG_MIN); }
    void update(int i, long long v){
        for(; i<=n; i+=i&-i) bit[i] = max(bit[i], v);
    }
    long long query(int i){
        long long res = LLONG_MIN;
        for(; i>0; i-=i&-i) res = max(res, bit[i]);
        return res;
    }
};

// Prune dominated states: keep only those not dominated in (down, diam, val).
// Dominance: A dominates B if A.down>=B.down && A.diam>=B.diam && A.val>=B.val.
static void prune_states(vector<State>& vec) {
    if (vec.empty()) return;

    // sort by down desc, diam desc, val desc
    sort(vec.begin(), vec.end(), [](const State& a, const State& b){
        if (a.down != b.down) return a.down > b.down;
        if (a.diam != b.diam) return a.diam > b.diam;
        return a.val > b.val;
    });

    // remove exact duplicates of (down, diam) keeping max val (already first due to sorting)
    vector<State> tmp;
    tmp.reserve(vec.size());
    for (auto &s : vec) {
        if (!tmp.empty() && tmp.back().down == s.down && tmp.back().diam == s.diam) continue;
        tmp.push_back(s);
    }
    vec.swap(tmp);

    // coordinate compress diam
    vector<long long> diams;
    diams.reserve(vec.size());
    for (auto &s : vec) diams.push_back(s.diam);
    sort(diams.begin(), diams.end());
    diams.erase(unique(diams.begin(), diams.end()), diams.end());
    int m = (int)diams.size();

    BIT bit(m);

    vector<State> kept;
    kept.reserve(vec.size());

    // scan in sorted order by down desc; BIT maintains max val among previous with diam >= current diam
    for (auto &s : vec) {
        int pos = (int)(lower_bound(diams.begin(), diams.end(), s.diam) - diams.begin()) + 1; // 1..m
        int rev = m - pos + 1; // larger diam -> smaller rev
        long long bestVal = bit.query(rev); // among previous with diam >= s.diam
        if (bestVal >= s.val) {
            continue; // dominated
        }
        kept.push_back(s);
        bit.update(rev, s.val);
    }
    vec.swap(kept);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;
    vector<vector<pair<int,long long>>> g(n+1);
    for (int i=0;i<n-1;i++){
        int u,v; long long w;
        cin >> u >> v >> w;
        g[u].push_back({v,w});
        g[v].push_back({u,w});
    }

    int K = min(k, n-1);

    // build rooted tree at 1
    vector<int> parent(n+1, 0), order;
    vector<long long> pw(n+1, 0); // weight to parent
    order.reserve(n);

    stack<int> st;
    st.push(1);
    parent[1]= -1;
    while(!st.empty()){
        int u=st.top(); st.pop();
        order.push_back(u);
        for (auto [v,w]: g[u]){
            if (v==parent[u]) continue;
            if (parent[v]!=0) continue;
            parent[v]=u;
            pw[v]=w;
            st.push(v);
        }
    }
    // postorder
    reverse(order.begin(), order.end());

    // dpOpen[u][c] = vector of states
    vector<vector<vector<State>>> dpOpen(n+1);
    vector<vector<long long>> bestClosed(n+1);
    vector<int> subSize(n+1, 1);

    for (int u : order) {
        // init dp for u alone
        dpOpen[u].assign(1, vector<State>{ State{0,0,0} });
        bestClosed[u].assign(1, 0);

        // process children
        for (auto [v,w] : g[u]) {
            if (parent[v] != u) continue;

            // ensure child computed; merge sizes
            int maxCu = (int)dpOpen[u].size()-1;
            int maxCv = (int)dpOpen[v].size()-1;
            int newMax = min(K, maxCu + maxCv + 1); // +1 for possibly cutting edge (u,v)

            vector<vector<State>> ndp(newMax+1);

            // precompute child closed best for each cut count in child
            // bestClosed[v][x] already computed below, but ensure its size == maxCv+1
            // merge
            for (int cu=0; cu<=maxCu; cu++){
                for (const State &su : dpOpen[u][cu]) {
                    // option: cut edge (u,v)
                    for (int cv=0; cv<=maxCv; cv++){
                        int tc = cu + cv + 1;
                        if (tc > K) break;
                        long long nval = su.val + bestClosed[v][cv] + w;
                        ndp[tc].push_back(State{su.down, su.diam, nval});
                    }
                    // option: keep edge (u,v)
                    for (int cv=0; cv<=maxCv; cv++){
                        int tc = cu + cv;
                        if (tc > K) break;
                        for (const State &sv : dpOpen[v][cv]) {
                            long long down_old = su.down;
                            long long child_down = sv.down;
                            long long candCross = down_old + w + child_down;

                            long long ndown = max(down_old, w + child_down);
                            long long ndiam = max({su.diam, sv.diam, candCross});
                            long long nval = su.val + sv.val;

                            ndp[tc].push_back(State{ndown, ndiam, nval});
                        }
                    }
                }
            }

            // prune each cut layer
            for (int c=0;c<=newMax;c++){
                prune_states(ndp[c]);
            }
            dpOpen[u].swap(ndp);

            // free child dp to save memory (optional)
            dpOpen[v].clear();
            dpOpen[v].shrink_to_fit();

            subSize[u] += subSize[v];
        }

        // compute bestClosed[u][c] = max(val + diam) among open states
        int maxC = (int)dpOpen[u].size()-1;
        bestClosed[u].assign(maxC+1, LLONG_MIN);
        for (int c=0;c<=maxC;c++){
            long long best = LLONG_MIN;
            for (auto &s : dpOpen[u][c]) {
                best = max(best, s.val + s.diam);
            }
            bestClosed[u][c] = (best==LLONG_MIN? 0: best);
        }
    }

    // exact best for root
    vector<long long> exact(K+1, 0);
    for (int t=0;t<=K;t++){
        exact[t] = bestClosed[1][t];
    }

    // prefix max (because extra ops can be wasted)
    vector<long long> ans(k+1, 0);
    long long cur = 0;
    for (int t=0;t<=k;t++){
        long long val = (t<=K? exact[t] : exact[K]);
        cur = max(cur, val);
        ans[t] = cur;
    }

    for (int i=0;i<=k;i++){
        if (i) cout << ' ';
        cout << ans[i];
    }
    cout << "\n";
    return 0;
}
