#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll MOD = 1000000007;

// 快速幂 & 逆元
ll modpow(ll a, ll e) {
    ll r = 1 % MOD;
    a %= MOD;
    while (e) {
        if (e & 1) r = r * a % MOD;
        a = a * a % MOD;
        e >>= 1;
    }
    return r;
}
ll modinv(ll a) {
    return modpow((a % MOD + MOD) % MOD, MOD - 2);
}

// 多项式：c[0] + c[1] x + ... + c[d] x^d
struct Poly {
    vector<ll> c;
    Poly(int deg = 0) { c.assign(deg + 1, 0); }
    int deg() const { return (int)c.size() - 1; }
};

Poly poly_add(const Poly &A, const Poly &B) {
    int n = max(A.deg(), B.deg());
    Poly C(n);
    for (int i = 0; i <= A.deg(); ++i) C.c[i] = (C.c[i] + A.c[i]) % MOD;
    for (int i = 0; i <= B.deg(); ++i) C.c[i] = (C.c[i] + B.c[i]) % MOD;
    return C;
}

Poly poly_mul(const Poly &A, const Poly &B) {
    Poly C(A.deg() + B.deg());
    for (int i = 0; i <= A.deg(); ++i) {
        if (!A.c[i]) continue;
        for (int j = 0; j <= B.deg(); ++j) {
            C.c[i + j] = (C.c[i + j] + A.c[i] * B.c[j]) % MOD;
        }
    }
    return C;
}

// ∫_0^1 A(x) dx
ll integrate_0_1(const Poly &A, const vector<ll> &invv) {
    ll ans = 0;
    for (int i = 0; i <= A.deg(); ++i) {
        ll coef = A.c[i] * invv[i + 1] % MOD; // ∫ x^i dx = 1/(i+1)
        ans = (ans + coef) % MOD;
    }
    return ans;
}

// 树
int n;
ll p, q;
vector<vector<int>> g;

// t = p/q（实数意义上在 (0,2)），但我们只在“约束起作用的区间”上做多项式。
// 这里用的是经典做法：把所有约束统一映射到 z ∈ [0,1] 上。
ll T;

// 每个点 u 的信息：
// Pu(z)：在“约束起作用的区间”上，P_u(s) 写成关于 z 的多项式；
// ans：P_u(0)，即父亲取值为 0 时整棵子树合法的概率。
struct NodeInfo {
    Poly Pu;
    ll ans;
};

vector<ll> invv;

// DFS：返回以 u 为根的子树的多项式信息
NodeInfo dfs(int u, int fa) {
    vector<NodeInfo> children;
    for (int v : g[u]) {
        if (v == fa) continue;
        children.push_back(dfs(v, u));
    }

    if (children.empty()) {
        // 叶子：没有儿子，边约束为空，P_u(s) ≡ 1
        Poly Pu(0);
        Pu.c[0] = 1;
        return {Pu, 1};
    }

    // 先在 z∈[0,1] 上构造 Fu(z) = ∏_v P_v(z)
    Poly Fu(0);
    Fu.c[0] = 1;
    for (auto &ch : children) {
        Fu = poly_mul(Fu, ch.Pu);
    }

    // 构造 Gu(z) = ∫_z^1 Fu(u) du，z∈[0,1]
    // 先算 H(z) = ∫_0^z Fu(u) du，是一个多项式；再用 Gu(z) = H(1) - H(z)
    Poly H(Fu.deg() + 1); // 积分后次数+1
    for (int i = 0; i <= Fu.deg(); ++i) {
        ll coef = Fu.c[i] * invv[i + 1] % MOD;
        H.c[i + 1] = coef;
    }
    // H(1)
    ll H1 = 0;
    {
        ll pw = 1;
        for (int i = 0; i <= H.deg(); ++i) {
            H1 = (H1 + H.c[i] * pw) % MOD;
            pw = pw * 1 % MOD;
        }
    }
    // Gu(z) = H1 - H(z)
    Poly Gu(H.deg());
    for (int i = 0; i <= H.deg(); ++i) {
        Gu.c[i] = (MOD - H.c[i]) % MOD;
    }
    Gu.c[0] = (Gu.c[0] + H1) % MOD;

    // ans_u = P_u(0) = ∫_0^1 Fu(u) du
    ll ans_u = integrate_0_1(Fu, invv);

    return {Gu, ans_u};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> p >> q;
    g.assign(n + 1, {});
    for (int i = 1; i < n; ++i) {
        int u, v;
        cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }

    // t = p/q
    ll invq = modinv(q);
    T = p % MOD * invq % MOD;

    // 预处理 1..n+2 的逆元，用于积分
    invv.assign(n + 5, 0);
    for (int i = 1; i <= n + 4; ++i) {
        invv[i] = modinv(i);
    }

    NodeInfo root = dfs(1, 0);

    // 最终答案：整棵树是好的的概率
    // 在这套变换下，根的 ans 就是我们要的整体概率
    ll answer = root.ans % MOD;
    cout << answer << "\n";

    return 0;
}
