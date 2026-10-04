#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ull = unsigned long long;

constexpr int MOD = 1000003579;
constexpr int MAXN = 100;
constexpr int MAXF = (MAXN + 1) * (MAXN + 2) / 2;
constexpr ull LIMIT = 1ULL << 63;

int n;
ll m;
vector<int> adj[MAXN + 1];

int alpha, rootB, invDelta;
int invNum[MAXN + 2];
int freq[MAXF], invA[MAXF], invB[MAXF];

int power(int a, ll b) {
    int r = 1;
    while (b) {
        if (b & 1) r = (ll)r * a % MOD;
        a = (ll)a * a % MOD;
        b >>= 1;
    }
    return r;
}

int add(int a, int b) {
    int c = a + b;
    return c >= MOD ? c - MOD : c;
}

int sub(int a, int b) {
    int c = a - b;
    return c < 0 ? c + MOD : c;
}

// q = a + b，指数为 a * alpha + (q - a) * rootB。
int id(int q, int a) {
    return q * (q + 1) / 2 + a;
}

struct Block {
    int q, a;
    vector<int> c;  // 普通多项式系数
};

struct Poly {
    int L = 0, D = 0;
    size_t terms = 0;
    vector<Block> blocks;
};

// 将临时稠密数组转换为稀疏表示。
Poly collect(int L, int D, const vector<int>& buf) {
    Poly p;
    int stride = D + 1;

    for (int q = 0; q <= L; ++q) {
        for (int a = 0; a <= q; ++a) {
            int off = id(q, a) * stride;

            int d = D;
            while (d >= 0 && buf[off + d] == 0) --d;
            if (d < 0) continue;

            Block b{q, a, vector<int>(d + 1)};
            for (int k = 0; k <= d; ++k) {
                b.c[k] = buf[off + k];
                p.terms += (b.c[k] != 0);
            }

            p.L = max(p.L, q);
            p.D = max(p.D, d);
            p.blocks.push_back(move(b));
        }
    }
    return p;
}

// 合并两个不相交的子森林。
Poly multiply(const Poly& x, const Poly& y) {
    int L = x.L + y.L;
    int D = x.D + y.D;
    int stride = D + 1;

    size_t len = (L + 1) * (L + 2) / 2 * stride;
    vector<ull> buf(len, 0);

    for (const auto& p : x.blocks) {
        for (const auto& r : y.blocks) {
            ull* dest = buf.data()
                      + id(p.q + r.q, p.a + r.a) * stride;

            for (int i = 0; i < (int)p.c.size(); ++i) {
                if (!p.c[i]) continue;
                ull v = p.c[i];

                for (int j = 0; j < (int)r.c.size(); ++j) {
                    if (!r.c[j]) continue;

                    ull& z = dest[i + j];
                    z += v * r.c[j];

                    // 累加前 < 2^63，单个乘积 < MOD^2，
                    // 因此累加后不会超过 uint64_t。
                    if (z >= LIMIT) z %= MOD;
                }
            }
        }
    }

    vector<int> out(len);
    for (size_t i = 0; i < len; ++i)
        out[i] = buf[i] % MOD;

    return collect(L, D, out);
}

// A = (alpha * I_alpha(B) - beta * I_beta(B)) / sqrt(5)。
Poly appendRoot(const Poly& b) {
    int L = max(1, b.L);
    int D = b.D + 1;
    int stride = D + 1;

    vector<int> out((L + 1) * (L + 2) / 2 * stride, 0);

    int weight[2] = {
        (int)((ll)alpha * invDelta % MOD),
        sub(0, (ll)rootB * invDelta % MOD)
    };

    for (const auto& p : b.blocks) {
        int idx = id(p.q, p.a);
        int off = idx * stride;

        for (int t = 0; t < 2; ++t) {
            int rootId = t == 0 ? id(1, 1) : id(1, 0);
            int inv = t == 0 ? invA[idx] : invB[idx];

            if (inv == 0) {
                // lambda = r：
                // I_r(e^{rx} P(x)) = e^{rx} * integral(P(x))。
                for (int k = 0; k < (int)p.c.size(); ++k) {
                    int v = (ll)weight[t] * p.c[k] % MOD
                          * invNum[k + 1] % MOD;
                    out[off + k + 1] =
                        add(out[off + k + 1], v);
                }
            } else {
                // lambda != r：
                // Q_k = (P_k - (k+1)Q_{k+1}) / (lambda-r)
                // I_r(e^{lambda*x}P) = e^{lambda*x}Q - e^{rx}Q_0。
                int next = 0;

                for (int k = (int)p.c.size() - 1; k >= 0; --k) {
                    int cur = (ll)sub(
                        p.c[k],
                        (ll)(k + 1) * next % MOD
                    ) * inv % MOD;

                    int v = (ll)weight[t] * cur % MOD;
                    out[off + k] = add(out[off + k], v);

                    if (k == 0) {
                        out[rootId * stride] =
                            sub(out[rootId * stride], v);
                    }

                    next = cur;
                }
            }
        }
    }

    return collect(L, D, out);
}

Poly dfs(int u, int parent) {
    vector<Poly> children;

    for (int v : adj[u]) {
        if (v != parent)
            children.push_back(dfs(v, u));
    }

    if (children.empty()) {
        Poly one;
        one.terms = 1;
        one.blocks.push_back({0, 0, {1}});
        return appendRoot(one);
    }

    sort(children.begin(), children.end(),
         [](const Poly& a, const Poly& b) {
             return a.terms < b.terms;
         });

    Poly b = move(children[0]);

    for (int i = 1; i < (int)children.size(); ++i) {
        b = multiply(b, children[i]);
        children[i] = Poly();
    }

    return appendRoot(b);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // MOD % 4 == 3，且 5 是模 MOD 的二次剩余。
    int delta = power(5, (MOD + 1LL) / 4);
    int inv2 = (MOD + 1) / 2;

    alpha = (ll)add(3, delta) * inv2 % MOD;
    rootB = (ll)sub(3, delta) * inv2 % MOD;
    invDelta = power(delta, MOD - 2);

    for (int k = 1; k <= n + 1; ++k)
        invNum[k] = power(k, MOD - 2);

    // 预处理各个指数以及积分变换需要的逆元。
    for (int q = 0; q <= n; ++q) {
        for (int a = 0; a <= q; ++a) {
            int idx = id(q, a);

            freq[idx] =
                ((ll)a * alpha + (ll)(q - a) * rootB) % MOD;

            int da = sub(freq[idx], alpha);
            int db = sub(freq[idx], rootB);

            invA[idx] = da == 0 ? 0 : power(da, MOD - 2);
            invB[idx] = db == 0 ? 0 : power(db, MOD - 2);
        }
    }

    Poly answer = dfs(1, 0);

    // falling[k] = m * (m-1) * ... * (m-k+1)。
    vector<int> falling(n + 1, 1);
    for (int k = 1; k <= n; ++k) {
        falling[k] = (ll)falling[k - 1]
                   * ((m - k + 1) % MOD) % MOD;
    }

    int ans = 0;

    for (const auto& p : answer.blocks) {
        int lambda = freq[id(p.q, p.a)];
        int value = power(lambda, m);
        int inv = power(lambda, MOD - 2);

        for (int k = 0; k < (int)p.c.size(); ++k) {
            int v = (ll)p.c[k] * falling[k] % MOD
                  * value % MOD;

            ans = add(ans, v);
            value = (ll)value * inv % MOD;
        }
    }

    cout << ans << '\n';
    return 0;
}