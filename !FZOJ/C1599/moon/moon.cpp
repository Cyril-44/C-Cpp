#include <cstdio>
#ifdef CLANGD
constexpr int N = 24, M = 2;
#else
constexpr int N = 200004, M = 640;
#endif
int Mod;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= Mod) val -= Mod; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += Mod; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    Mint(int v=0) : val(v) {}
    int val;
} f[N / 2 + M][M], ans[N + M * 2];
inline int max(int x, int y) { return x > y ? x : y; }
int main() {
    int T; scanf("%d%d", &T, &Mod);
    f[0][0] = 1;
    for (int i = 1; i <= 100640; i++) {
        int sum = 1;
        for (int j = 1; sum <= i; sum += ++j) {
            f[i][j] = f[i-j][j] + f[i-j][j-1];
            ans[i * 2 - j] += f[i][j];
        }
    }
    while (T--) {
        int n; scanf("%d", &n);
        printf("%d\n", ans[n]);
    }
    return 0;
}