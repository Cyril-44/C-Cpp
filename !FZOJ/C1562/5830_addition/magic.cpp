#include <cstdio>
constexpr int N = 100005, MOD = 998244353;
using ULL = unsigned long long;
struct ModInt {
    inline void fixPos() { if (val >= MOD) val -= MOD; }
    inline void fixNeg() { if (val < 0) val += MOD; }
    inline ModInt(int x = 0) : val(x) {}
    inline explicit operator int() const { return val; }
    inline ModInt operator+=(ModInt x) { val += x.val; fixPos(); return *this; }
    inline ModInt operator-=(ModInt x) { val -= x.val; fixNeg(); return *this; }
    inline ModInt operator*=(ModInt x) { val = (ULL)val * x.val % MOD; return *this; }
    inline ModInt operator+(ModInt x) const { return ModInt(*this) += x; }
    inline ModInt operator-(ModInt x) const { return ModInt(*this) -= x; }
    inline ModInt operator*(ModInt x) const { return ModInt(*this) *= x; }
    inline ModInt operator-() const { return val ? ModInt(MOD - val) : *this; }
    inline ModInt& operator++() { if (++val == MOD) val = 0; return *this; }
    inline ModInt& operator--() { if (val-- == 0) val = MOD - 1; return *this; }
    private: int val;
};
template<class T>
inline T qpow(T b, int n, T res) {
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}

struct Matrix {
    ModInt a00, a01, a10, a11;
    friend Matrix operator*(const Matrix&, const Matrix&);
    Matrix& operator*=(const Matrix& o) { return *this = *this * o; }
};
inline Matrix operator*(const Matrix &x, const Matrix &y) {
    return {
        x.a00 * y.a00 + x.a01 * y.a10,
        x.a00 * y.a01 + x.a01 * y.a11,
        x.a10 * y.a00 + x.a11 * y.a10,
        x.a10 * y.a01 + x.a11 * y.a11,
    };
}
static int n;
struct SegTr {
    struct Node {
        ModInt sum{0}, mul{1}, add{0};
        inline void pull(ModInt m, ModInt a, int len) {
            sum = sum * m + a * len;
            mul *= m, add = add * m + a;
        }
    } tr[N << 2];
    int L, R;
    ModInt X, Y;
    inline void pushdown(int u, int ll, int rl) {
        if (int(tr[u].mul) == 1 && int(tr[u].add) == 0) return;
        tr[u<<1].pull(tr[u].mul, tr[u].add, ll);
        tr[u<<1|1].pull(tr[u].mul, tr[u].add, rl);
        tr[u].mul = 1, tr[u].add = 0;
    }
    inline void pushup(int u) { tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum; }
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return void(tr[u].pull(X, Y, r - l + 1));
        int mid = l + r >> 1;
        pushdown(u, mid - l + 1, r - mid);
        if (L <= mid) upd(u << 1, l, mid);
        if (mid < R) upd(u << 1 | 1, mid + 1, r);
        pushup(u);
    }
    ModInt que(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        ModInt res;
        int mid = l + r >> 1;
        pushdown(u, mid - l + 1, r - mid);
        if (L <= mid) res += que(u << 1, l, mid);
        if (mid < R) res += que(u << 1 | 1, mid + 1, r);
        return res;
    }
    inline ModInt inquire(int l, int r) { return L = l, R = r, que(1, 1, n); }
    inline void update(int l, int r, ModInt mul, ModInt add) { L = l, R = r, X = mul, Y = add; upd(1, 1, n); }
} f;

int main() {
    int n, m;
    scanf("%*d%d%d", &n, &m);
    ::n = m;
    ModInt expf = 1;
    for (int l, r, c; n--; ) {
        scanf("%d%d%d", &l, &r, &c);
        int j = r - l + 1;
        // A, S = 2A - 1/k * S, 1/k * A + (1-1/k) * S
        ModInt invj = qpow<ModInt>(j, MOD-2, 1), rinvj = ModInt(1) - invj;
        ModInt exps = f.inquire(l, r);
        Matrix x = qpow<Matrix>({2, -invj, 1, rinvj}, c, {1,0,0,1}) * Matrix{expf, 0, exps, 0};
        ModInt k = qpow<ModInt>(rinvj, c, 1);
        ModInt b = invj * (x.a10 - k * exps);
        f.update(l, r, k, b);
        expf = x.a00;
    }
    printf("%d\n", int(expf));
    return 0;
}