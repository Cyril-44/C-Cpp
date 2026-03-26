#include <cstdio>
#include <map>
#include <cstdint>
constexpr int MOD = 998244353;
struct ModInt {
    inline ModInt(int x = 0) : val(x) {}
    inline explicit operator int() const { return val; }
    inline ModInt& operator+=(const ModInt &x) { if ((val += x.val) >= MOD) val -= MOD; return *this; }
    inline ModInt operator+(const ModInt &x) const { return ModInt(*this) += x; }
    inline ModInt& operator-=(const ModInt &x) { if ((val -= x.val) < 0) val += MOD; return *this; }
    inline ModInt operator-(const ModInt &x) const { return ModInt(*this) -= x; }
    private: int val;
};
int main() {
    ModInt f = 1;
    std::map<int64_t, ModInt> g;
    int n;
    scanf("%d", &n);
    int64_t as = 0;
    for (int i = 1, x; i < n; i++) {
        scanf("%d", &x);
        as += x;
        ModInt tmp = f;
        f = f + f - g[as];
        g[as] = tmp;
    }
    printf("%d\n", int(f));
    return 0;
}
