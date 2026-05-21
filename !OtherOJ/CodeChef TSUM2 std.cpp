#include <bits/stdc++.h>
using namespace std;

using ll = long long;
const ll NEG = -(1LL << 60);

struct Line {
    ll m, b;
    Line(ll _m = 0, ll _b = NEG) : m(_m), b(_b) {}
    inline ll get(ll x) const {
        return (ll)((__int128)m * x + b);
    }
};

struct LiChao {
    vector<ll> xs;
    vector<Line> seg;
    vector<char> has;

    LiChao() {}
    LiChao(const vector<ll>& coords) { init(coords); }

    void init(const vector<ll>& coords) {
        xs = coords;
        int n = (int)xs.size();
        seg.assign(4 * n + 4, Line());
        has.assign(4 * n + 4, 0);
    }

    void insert(Line nw) {
        insert(1, 0, (int)xs.size() - 1, nw);
    }

    void insert(int idx, int l, int r, Line nw) {
        if (!has[idx]) {
            seg[idx] = nw;
            has[idx] = 1;
            return;
        }

        int mid = (l + r) >> 1;
        ll xl = xs[l], xm = xs[mid], xr = xs[r];

        if (nw.get(xm) > seg[idx].get(xm)) swap(seg[idx], nw);

        if (l == r) return;

        if (nw.get(xl) > seg[idx].get(xl)) insert(idx << 1, l, mid, nw);
        else if (nw.get(xr) > seg[idx].get(xr)) insert(idx << 1 | 1, mid + 1, r, nw);
    }

    ll query(int pos) const {
        return query(1, 0, (int)xs.size() - 1, pos);
    }

    ll query(int idx, int l, int r, int pos) const {
        ll res = has[idx] ? seg[idx].get(xs[pos]) : NEG;
        if (l == r) return res;
        int mid = (l + r) >> 1;
        if (pos <= mid) return max(res, query(idx << 1, l, mid, pos));
        else return max(res, query(idx << 1 | 1, mid + 1, r, pos));
    }
};

struct State {
    int len;
    ll sum, val;
    int idx;
};

struct Frame {
    int u, p, len;
    ll sum, val;
};

int n;
vector<vector<int>> g;
vector<ll> w;
vector<char> dead;
vector<int> parentTmp, sz;
ll ans;

void collect_subtree(int start, int parent, ll wc, vector<State>& vec) {
    stack<Frame> st;
    st.push({start, parent, 2, wc + w[start], wc + 2LL * w[start]});

    while (!st.empty()) {
        auto cur = st.top();
        st.pop();

        vec.push_back({cur.len, cur.sum, cur.val, -1});

        for (int v : g[cur.u]) {
            if (dead[v] || v == cur.p) continue;
            st.push({v, cur.u, cur.len + 1,
                     cur.sum + w[v],
                     cur.val + 1LL * (cur.len + 1) * w[v]});
        }
    }
}

int get_centroid(int start) {
    vector<int> comp;
    comp.reserve(1024);

    stack<int> st;
    st.push(start);
    parentTmp[start] = 0;

    while (!st.empty()) {
        int u = st.top();
        st.pop();
        comp.push_back(u);

        for (int v : g[u]) {
            if (dead[v] || v == parentTmp[u]) continue;
            parentTmp[v] = u;
            st.push(v);
        }
    }

    for (int i = (int)comp.size() - 1; i >= 0; --i) {
        int u = comp[i];
        sz[u] = 1;
        for (int v : g[u]) {
            if (!dead[v] && parentTmp[v] == u) sz[u] += sz[v];
        }
    }

    int total = (int)comp.size();
    int centroid = start;
    int best = total;

    for (int u : comp) {
        int mx = total - sz[u];
        for (int v : g[u]) {
            if (!dead[v] && parentTmp[v] == u) mx = max(mx, sz[v]);
        }
        if (mx < best) {
            best = mx;
            centroid = u;
        }
    }

    return centroid;
}

void solve(int start) {
    int c = get_centroid(start);
    dead[c] = 1;

    ll wc = w[c];

    vector<vector<State>> subs;
    vector<ll> coords;
    coords.push_back(0); // centroid query needs x = 0

    for (int v : g[c]) {
        if (dead[v]) continue;
        vector<State> vec;
        collect_subtree(v, c, wc, vec);
        for (auto &st : vec) coords.push_back(st.sum - wc);
        subs.push_back(move(vec));
    }

    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    int zero_idx = (int)(lower_bound(coords.begin(), coords.end(), 0) - coords.begin());

    for (auto &vec : subs) {
        for (auto &st : vec) {
            st.idx = (int)(lower_bound(coords.begin(), coords.end(), st.sum - wc) - coords.begin());
        }
    }

    auto run_pass = [&](const vector<int>& ord) {
        LiChao lc(coords);
        lc.insert(Line(1, wc)); // centroid itself: c -> x

        for (int id : ord) {
            auto &vec = subs[id];

            // query with previous subtrees only
            for (auto &st : vec) {
                ll best = lc.query(st.idx) + (st.val - st.sum);
                if (best > ans) ans = best;
            }

            // insert current subtree
            for (auto &st : vec) {
                ll b = 1LL * (st.len + 1) * st.sum - st.val;
                lc.insert(Line(st.len, b));
            }
        }

        // x -> c
        ans = max(ans, lc.query(zero_idx));
    };

    vector<int> ord(subs.size());
    iota(ord.begin(), ord.end(), 0);

    run_pass(ord);
    reverse(ord.begin(), ord.end());
    run_pass(ord);

    for (int v : g[c]) {
        if (!dead[v]) solve(v);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        cin >> n;
        w.assign(n + 1, 0);
        for (int i = 1; i <= n; ++i) cin >> w[i];

        g.assign(n + 1, {});
        for (int i = 1; i <= n - 1; ++i) {
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }

        dead.assign(n + 1, 0);
        parentTmp.assign(n + 1, 0);
        sz.assign(n + 1, 0);

        ans = *max_element(w.begin() + 1, w.end());

        solve(1);

        cout << ans << '\n';
    }

    return 0;
}