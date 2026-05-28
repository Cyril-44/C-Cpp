#include <cstdio>
int Mod;
struct ModInt {
    ModInt(int v=0) : val(v) {}
    ModInt operator-() const { return ModInt(!val ? 0 : Mod-val); }
    ModInt& operator+=(ModInt x) { if ((val += x.val) >= Mod) val -= Mod; return *this; }
    ModInt& operator-=(ModInt x) { if ((val -= x.val) < 0) val += Mod; return *this; }
    ModInt& operator*=(ModInt x) { val = 1ull*val*x.val % Mod; return *this; }
    ModInt operator+(ModInt x) const { return ModInt(*this) += x; }
    ModInt operator-(ModInt x) const { return ModInt(*this) -= x; }
    ModInt operator*(ModInt x) const { return ModInt(*this) *= x; }
    template<class T> constexpr explicit operator T() const { return static_cast<T>(val); }
    private: int val;
};
inline ModInt qpow(ModInt b, int n) {
    ModInt res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b; n >>= 1;
    }
    return res;
}
int main() {
    int T;
    scanf("%d%d", &T, &Mod);
    while (T--) {
        int n;
        scanf("%d", &n);
        ModInt ans = qpow(2*n*(2*n-1ll)/2 % Mod, Mod-2) * n + ModInt((2*n*(2*n-1ll)/2-n) % Mod) * qpow(2*n*(2*n-1ll) % Mod, Mod-2);
        printf("%d\n", int(ans));
    }
    return 0;
}