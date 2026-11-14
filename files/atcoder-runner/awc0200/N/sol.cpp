#include <bits/stdc++.h>
using namespace std;

static constexpr long long MOD = 998244353;

struct Affine {
    long long a = 1, b = 0;
};

// Apply x and then y.
Affine join(Affine x, Affine y) {
    return {(y.a * x.a) % MOD, (y.a * x.b + y.b) % MOD};
}

struct Node {
    int ch[2] = {0, 0};
    int p = 0;
    bool rev = false;
    bool flipped = false;
    Affine valF, valR, aggF, aggR;
};

struct LinkCutTree {
    vector<Node> t;
    explicit LinkCutTree(int n) : t(n + 1) {}

    bool isRoot(int x) const {
        int p = t[x].p;
        return !p || (t[p].ch[0] != x && t[p].ch[1] != x);
    }
    void pull(int x) {
        int l = t[x].ch[0], r = t[x].ch[1];
        Affine lf = l ? t[l].aggF : Affine{};
        Affine rf = r ? t[r].aggF : Affine{};
        Affine lr = l ? t[l].aggR : Affine{};
        Affine rr = r ? t[r].aggR : Affine{};
        t[x].aggF = join(join(lf, t[x].valF), rf);
        t[x].aggR = join(join(rr, t[x].valR), lr);
    }
    void applyRev(int x) {
        if (!x) return;
        swap(t[x].ch[0], t[x].ch[1]);
        swap(t[x].valF, t[x].valR);
        swap(t[x].aggF, t[x].aggR);
        t[x].rev = !t[x].rev;
        t[x].flipped = !t[x].flipped;
    }
    void push(int x) {
        if (t[x].rev) {
            applyRev(t[x].ch[0]);
            applyRev(t[x].ch[1]);
            t[x].rev = false;
        }
    }
    void rotate(int x) {
        int p = t[x].p, g = t[p].p;
        int d = (t[p].ch[1] == x), b = t[x].ch[d ^ 1];
        if (!isRoot(p)) t[g].ch[t[g].ch[1] == p] = x;
        t[x].p = g;
        t[x].ch[d ^ 1] = p; t[p].p = x;
        t[p].ch[d] = b; if (b) t[b].p = p;
        pull(p); pull(x);
    }
    void splay(int x) {
        vector<int> st{x};
        for (int y = x; !isRoot(y); ) { y = t[y].p; st.push_back(y); }
        for (int i = (int)st.size() - 1; i >= 0; --i) push(st[i]);
        while (!isRoot(x)) {
            int p = t[x].p, g = t[p].p;
            if (!isRoot(p)) {
                bool zigzig = (t[p].ch[0] == x) == (t[g].ch[0] == p);
                rotate(zigzig ? p : x);
            }
            rotate(x);
        }
    }
    void access(int x) {
        int last = 0;
        for (int y = x; y; y = t[y].p) {
            splay(y);
            t[y].ch[1] = last;
            if (last) t[last].p = y;
            pull(y);
            last = y;
        }
        splay(x);
    }
    void makeRoot(int x) { access(x); applyRev(x); }
    int findRoot(int x) {
        access(x);
        while (true) {
            push(x);
            if (!t[x].ch[0]) break;
            x = t[x].ch[0];
        }
        splay(x);
        return x;
    }
    bool connected(int x, int y) { return x == y || findRoot(x) == findRoot(y); }
    void link(int x, int y) { makeRoot(x); if (findRoot(y) != x) t[x].p = y; }
    void cut(int x, int y) {
        makeRoot(x); access(y);
        if (t[y].ch[0] == x) {
            t[y].ch[0] = 0; t[x].p = 0; pull(y);
        }
    }
    void setValue(int x, Affine f, Affine r) {
        access(x);
        if (t[x].flipped) swap(f, r);
        t[x].valF = f; t[x].valR = r; pull(x);
    }
    Affine path(int x, int y) { makeRoot(x); access(y); return t[y].aggF; }
};

struct EdgeInfo { int id, from, to; };
long long keyOf(int u, int v) {
    if (u > v) swap(u, v);
    return (static_cast<long long>(u) << 32) | (unsigned)v;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int N, Q; if (!(cin >> N >> Q)) return 0;
    LinkCutTree lct(N + Q + 2);
    unordered_map<long long, EdgeInfo> edges;
    int nextId = N;
    auto get = [](long long a, long long b) { return Affine{a, b}; };
    for (int z = 0; z < Q; ++z) {
        char op; cin >> op;
        if (op == 'L') {
            int u, v; long long a,b,c,d; cin >> u >> v >> a >> b >> c >> d;
            int id = ++nextId;
            lct.setValue(id, get(a,b), get(c,d));
            lct.link(u, id); lct.link(id, v);
            edges[keyOf(u,v)] = {id,u,v};
        } else if (op == 'C') {
            int u,v; cin >> u >> v;
            auto e = edges.at(keyOf(u,v));
            lct.cut(u,e.id); lct.cut(v,e.id);
            edges.erase(keyOf(u,v));
        } else if (op == 'U') {
            int u,v; long long a,b,c,d; cin >> u >> v >> a >> b >> c >> d;
            auto e = edges.at(keyOf(u,v));
            // Update coefficients use the edge's original opening direction,
            // regardless of the order of u and v in this operation.
            lct.setValue(e.id, get(a,b), get(c,d));
        } else {
            int x,y; long long s; cin >> x >> y >> s;
            if (!lct.connected(x,y)) cout << -1 << '\n';
            else { auto f = lct.path(x,y); cout << (f.a * s + f.b) % MOD << '\n'; }
        }
    }
}
