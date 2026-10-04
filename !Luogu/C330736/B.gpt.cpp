#include <bits/stdc++.h>
using namespace std;

class FastInput {
    static const int B = 1 << 20;
    char buf[B];
    int pos = 0, len = 0;

    int getChar() {
        if (pos == len) {
            len = (int)fread(buf, 1, B, stdin);
            pos = 0;
            if (!len) return EOF;
        }
        return buf[pos++];
    }

public:
    int read() {
        int c = getChar(), x = 0;
        while (c <= ' ' && c != EOF) c = getChar();
        while (c >= '0' && c <= '9') {
            x = x * 10 + c - '0';
            c = getChar();
        }
        return x;
    }
};

class FastOutput {
    static const int B = 1 << 20;
    char buf[B];
    int pos = 0;

public:
    ~FastOutput() {
        flush();
    }

    void flush() {
        fwrite(buf, 1, pos, stdout);
        pos = 0;
    }

    void put(char c) {
        if (pos == B) flush();
        buf[pos++] = c;
    }

    void write(const char* s) {
        while (*s) put(*s++);
    }

    void number(int x, char end) {
        char s[12];
        int k = 0;
        do {
            s[k++] = char('0' + x % 10);
            x /= 10;
        } while (x);
        while (k) put(s[--k]);
        put(end);
    }
};

struct Tree {
    int n;
    vector<int> head, to, nxt;

    explicit Tree(int n) : n(n), head(n + 1, -1) {
        to.reserve(2 * (n - 1));
        nxt.reserve(2 * (n - 1));
    }

    void add(int u, int v) {
        to.push_back(v);
        nxt.push_back(head[u]);
        head[u] = (int)to.size() - 1;
    }

    void edge(int u, int v) {
        add(u, v);
        add(v, u);
    }
};

vector<int> findCentroids(const Tree& g) {
    vector<int> par(g.n + 1), sz(g.n + 1);
    vector<int> order(1, 1), ans;
    par[1] = -1;

    for (int i = 0; i < (int)order.size(); ++i) {
        int u = order[i];
        for (int e = g.head[u]; e != -1; e = g.nxt[e]) {
            int v = g.to[e];
            if (v == par[u]) continue;
            par[v] = u;
            order.push_back(v);
        }
    }

    for (int i = g.n - 1; i >= 0; --i) {
        int u = order[i], mx = 0;
        sz[u] = 1;

        for (int e = g.head[u]; e != -1; e = g.nxt[e]) {
            int v = g.to[e];
            if (par[v] != u) continue;
            sz[u] += sz[v];
            mx = max(mx, sz[v]);
        }

        mx = max(mx, g.n - sz[u]);
        if (2 * mx <= g.n) ans.push_back(u);
    }

    return ans;
}

// 相同的“排序后的儿子类型序列”分配相同编号。
// 两棵树共用此表，完全确定性，不存在哈希碰撞问题。
struct TypeTable {
    map<vector<int>, int> ids;

    int get(vector<int> key) {
        int newId = (int)ids.size() + 1;
        auto res = ids.try_emplace(move(key), newId);
        return res.first->second;
    }
};

struct Component {
    // 单重心分量：(1, 根类型, 0)
    // 双重心分量：(2, 较小半树类型, 较大半树类型)
    array<int, 3> key;
    int entry, r1, r2;
};

struct Forest {
    vector<int> par, type;
    vector<Component> comp;

    explicit Forest(int n) : par(n + 1), type(n + 1) {}
};

Forest buildForest(const Tree& g, int center, TypeTable& table) {
    Forest f(g.n);
    vector<int> sz(g.n + 1), order;

    for (int ec = g.head[center]; ec != -1; ec = g.nxt[ec]) {
        int entry = g.to[ec];
        order.clear();
        order.push_back(entry);
        f.par[entry] = center;

        // 先以原入口为根遍历当前分量。
        for (int i = 0; i < (int)order.size(); ++i) {
            int u = order[i];
            for (int e = g.head[u]; e != -1; e = g.nxt[e]) {
                int v = g.to[e];
                if (v == f.par[u]) continue;
                f.par[v] = u;
                order.push_back(v);
            }
        }

        int m = (int)order.size();
        vector<int> centers;

        // 找当前分量自身的重心。
        for (int i = m - 1; i >= 0; --i) {
            int u = order[i], mx = 0;
            sz[u] = 1;

            for (int e = g.head[u]; e != -1; e = g.nxt[e]) {
                int v = g.to[e];
                if (f.par[v] != u) continue;
                sz[u] += sz[v];
                mx = max(mx, sz[v]);
            }

            mx = max(mx, m - sz[u]);
            if (2 * mx <= m) centers.push_back(u);
        }

        int r1 = centers[0];
        int r2 = centers.size() == 2 ? centers[1] : 0;

        order.clear();
        order.push_back(r1);
        f.par[r1] = 0;
        if (r2) {
            order.push_back(r2);
            f.par[r2] = 0;
        }

        // 单重心：以重心为根。
        // 双重心：删除中心边，两端分别作为根。
        for (int i = 0; i < (int)order.size(); ++i) {
            int u = order[i];
            for (int e = g.head[u]; e != -1; e = g.nxt[e]) {
                int v = g.to[e];
                if (v == center || v == f.par[u]) continue;
                if ((u == r1 && v == r2) ||
                    (u == r2 && v == r1)) continue;

                f.par[v] = u;
                order.push_back(v);
            }
        }

        // 自底向上计算有根树类型。
        for (int i = m - 1; i >= 0; --i) {
            int u = order[i];
            vector<int> key;

            for (int e = g.head[u]; e != -1; e = g.nxt[e]) {
                int v = g.to[e];
                if (f.par[v] == u) key.push_back(f.type[v]);
            }

            sort(key.begin(), key.end());
            f.type[u] = table.get(move(key));
        }

        if (r2 && f.type[r1] > f.type[r2]) swap(r1, r2);

        f.comp.push_back({
            {r2 ? 2 : 1, f.type[r1], r2 ? f.type[r2] : 0},
            entry, r1, r2
        });
    }

    sort(f.comp.begin(), f.comp.end(),
         [](const Component& a, const Component& b) {
             return a.key < b.key;
         });

    return f;
}

// 分量类型已经相同，按儿子类型排序后逐一对应，生成同构映射。
void matchForests(const Tree& s, const Tree& t,
                  const Forest& fs, const Forest& ft,
                  vector<int>& p, vector<int>& inv) {
    vector<pair<int, int>> stk, a, b;

    for (int i = 0; i < (int)fs.comp.size(); ++i) {
        stk.emplace_back(fs.comp[i].r1, ft.comp[i].r1);
        if (fs.comp[i].r2) {
            stk.emplace_back(fs.comp[i].r2, ft.comp[i].r2);
        }
    }

    while (!stk.empty()) {
        auto [u, v] = stk.back();
        stk.pop_back();

        p[u] = v;
        inv[v] = u;

        a.clear();
        b.clear();

        for (int e = s.head[u]; e != -1; e = s.nxt[e]) {
            int x = s.to[e];
            if (fs.par[x] == u) a.emplace_back(fs.type[x], x);
        }

        for (int e = t.head[v]; e != -1; e = t.nxt[e]) {
            int x = t.to[e];
            if (ft.par[x] == v) b.emplace_back(ft.type[x], x);
        }

        sort(a.begin(), a.end());
        sort(b.begin(), b.end());

        for (int i = 0; i < (int)a.size(); ++i) {
            stk.emplace_back(a[i].second, b[i].second);
        }
    }
}

// 删除中心边后，遍历一个半边，得到父先子后的顺序。
vector<int> rootHalf(const Tree& g, int root, int blocked,
                     vector<int>& par) {
    vector<int> order(1, root);
    par[root] = 0;

    for (int i = 0; i < (int)order.size(); ++i) {
        int u = order[i];
        for (int e = g.head[u]; e != -1; e = g.nxt[e]) {
            int v = g.to[e];
            if (v == blocked || v == par[u]) continue;
            par[v] = u;
            order.push_back(v);
        }
    }

    return order;
}

void changeHalf(int root, int other,
                const vector<int>& targetOrder,
                const vector<int>& ps,
                const vector<int>& pt,
                const vector<int>& inv,
                vector<array<int, 3>>& ops) {
    int current = root;

    for (int i = 1; i < (int)targetOrder.size(); ++i) {
        int y = targetOrder[i];
        int u = inv[y], w = inv[pt[y]];

        if (ps[u] == w) continue;

        // 移动中心边，使 u 成为重心。
        if (current != u) {
            ops.push_back({other, current, u});
            current = u;
        }

        // u 尚未处理，它的原父边仍然存在。
        ops.push_back({u, ps[u], w});
    }

    // 恢复本半的中心端点。
    if (current != root) {
        ops.push_back({other, current, root});
    }
}

static FastInput in;
static FastOutput out;

int main() {
    int tests = in.read();

    while (tests--) {
        int n = in.read();
        Tree s(n), t(n);

        for (int i = 1; i < n; ++i) {
            int u = in.read(), v = in.read();
            s.edge(u, v);
        }
        for (int i = 1; i < n; ++i) {
            int u = in.read(), v = in.read();
            t.edge(u, v);
        }

        auto cs = findCentroids(s);
        auto ct = findCentroids(t);

        if (cs.size() != ct.size()) {
            out.write("No\n");
            continue;
        }

        vector<int> p(n + 1), inv(n + 1);
        vector<array<int, 3>> ops;

        if (cs.size() == 1) {
            TypeTable table;
            auto fs = buildForest(s, cs[0], table);
            auto ft = buildForest(t, ct[0], table);

            bool ok = fs.comp.size() == ft.comp.size();
            if (ok) {
                for (int i = 0; i < (int)fs.comp.size(); ++i) {
                    if (fs.comp[i].key != ft.comp[i].key) {
                        ok = false;
                    }
                }
            }

            if (!ok) {
                out.write("No\n");
                continue;
            }

            p[cs[0]] = ct[0];
            inv[ct[0]] = cs[0];
            matchForests(s, t, fs, ft, p, inv);

            for (int i = 0; i < (int)fs.comp.size(); ++i) {
                int v = fs.comp[i].entry;
                int w = inv[ft.comp[i].entry];
                if (v != w) ops.push_back({cs[0], v, w});
            }
        } else {
            int a = cs[0], b = cs[1];
            int x = ct[0], y = ct[1];

            vector<int> ps(n + 1), pt(n + 1);
            auto sa = rootHalf(s, a, b, ps);
            auto sb = rootHalf(s, b, a, ps);
            auto ta = rootHalf(t, x, y, pt);
            auto tb = rootHalf(t, y, x, pt);

            // 两半分别任意配对，且根对应根。
            for (int i = 0; i < (int)sa.size(); ++i) {
                p[sa[i]] = ta[i];
                inv[ta[i]] = sa[i];

                p[sb[i]] = tb[i];
                inv[tb[i]] = sb[i];
            }

            changeHalf(a, b, ta, ps, pt, inv, ops);
            changeHalf(b, a, tb, ps, pt, inv, ops);
        }

        out.write("Yes\n");
        out.number((int)ops.size(), '\n');

        for (auto op : ops) {
            out.number(op[0], ' ');
            out.number(op[1], ' ');
            out.number(op[2], '\n');
        }

        for (int i = 1; i <= n; ++i) {
            out.number(p[i], i == n ? '\n' : ' ');
        }
    }

    return 0;
}