#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const int MAXN = 100000 + 5;
const int MOD = 998244353;

int parent_[MAXN], parity_[MAXN], sz_[MAXN];

// find with path compression; along the way, accumulate parity to the root
int find_set(int x) {
    if (parent_[x] == x) return x;
    int p = parent_[x];
    parent_[x] = find_set(p);
    parity_[x] ^= parity_[p];
    return parent_[x];
}

// merge two sets, enforcing that f[u] ^ f[v] = w
// returns true if merged successfully (or redundant but consistent), false if contradiction
bool union_set(int u, int v, int w) {
    int ru = find_set(u);
    int rv = find_set(v);
    int pu = parity_[u];
    int pv = parity_[v];
    if (ru == rv) {
        // already in same component: check consistency
        return ((pu ^ pv) == w);
    }
    // union by size
    if (sz_[ru] > sz_[rv]) {
        swap(ru, rv);
        swap(pu, pv);
    }
    // attach ru under rv
    parent_[ru] = rv;
    // we need parity_[ru] such that pu ^ parity_[ru] ^ pv = w
    // so parity_[ru] = pu ^ pv ^ w
    parity_[ru] = pu ^ pv ^ w;
    sz_[rv] += sz_[ru];
    return true;
}

// fast exponentiation mod
ll mod_pow(ll a, ll e) {
    ll res = 1 % MOD;
    a %= MOD;
    while (e) {
        if (e & 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
        e >>= 1;
    }
    return res;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int N, Q;
    cin >> N >> Q;
    // read the tree structure, but we actually don't need the explicit edges
    // since constraints reduce to a system on the tree's vertex potentials.
    //
    // skip reading the next N-1 edges
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
    }

    // initialize DSU
    for (int i = 1; i <= N; i++) {
        parent_[i] = i;
        parity_[i] = 0;
        sz_[i] = 1;
    }

    int rank = 0;
    bool ok = true;
    for (int i = 0; i < Q; i++) {
        int u, v, k;
        cin >> u >> v >> k;
        if (!ok) continue;
        if (union_set(u, v, k ? 1 : 0)) {
            // merged without contradiction
            // only if it actually connected two different components do we increase rank
            // we can detect that by checking if they were in different sets before union
            // but union_set doesn't tell us directly; so we check again:
            // actually, we can compare roots before calling union_set.
            // To avoid double find, we can instrument union_set to return a flag.
            // For simplicity, do it here by re-finding:
            // (slight overhead, still amortized O(alpha(N)))
            // Instead, we can detect rank increment inside union_set:
            // but we've done it externally: see implementation below.
            //
            // I'll simplify: if they were in different roots, that call did union.
            // So we do:
            // (We can store a bool)
        } else {
            ok = false;
        }
    }

    // The above approach didn't count rank properly.
    // Let's re-process constraints, but tracking rank inside union_set.

    // Reset DSU
    for (int i = 1; i <= N; i++) {
        parent_[i] = i;
        parity_[i] = 0;
        sz_[i] = 1;
    }

    cin.clear();
    cin.seekg(0);
    cin >> N >> Q;
    for (int i = 0; i < N - 1; i++) {
        int u, v;
        cin >> u >> v;
    }

    rank = 0;
    ok = true;
    for (int i = 0; i < Q; i++) {
        int u, v, k;
        cin >> u >> v >> k;
        if (!ok) continue;

        int ru = find_set(u), rv = find_set(v);
        int pu = parity_[u], pv = parity_[v];
        if (ru == rv) {
            // must satisfy existing parity
            if ((pu ^ pv) != (k & 1)) {
                ok = false;
            }
        } else {
            // union by size
            if (sz_[ru] > sz_[rv]) {
                swap(ru, rv);
                swap(pu, pv);
            }
            parent_[ru] = rv;
            parity_[ru] = pu ^ pv ^ (k & 1);
            sz_[rv] += sz_[ru];
            rank++;
        }
    }

    if (!ok) {
        cout << 0 << "\n";
    } else {
        // number of free edge-labelings = 2^{(N-1) - rank}
        ll expo = (ll)(N - 1 - rank);
        cout << mod_pow(2LL, expo) << "\n";
    }
    return 0;
}
