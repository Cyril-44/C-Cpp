#include <cstdio>
constexpr int N = 100005, MOD = (int)1e9 + 7;
struct Mint {
    Mint& operator+=(Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(Mint o) { val = 1ull * val * o.val % MOD; return *this; }
    friend Mint operator+(Mint x, Mint y) { return x += y; }
    friend Mint operator-(Mint x, Mint y) { return x -= y; }
    friend Mint operator*(Mint x, Mint y) { return x *= y; }
    Mint(int v=0) : val(v) {}
private: int val;
} sum[N], cnt[N];
int op[N][3];
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= q; i++)
        scanf("%d%d%d", &op[i][0], &op[i][1], &op[i][2]);
    cnt[n+1] = 1;
    for (int i = q; i >= 1; i--) {
        cnt[i] += cnt[i+1];
        if (op[i][0] == 1) sum[op[i][1]] += cnt[i], sum[op[i][2] + 1] -= cnt[i];
        else cnt[op[i][2]] += cnt[i], cnt[op[i][1] - 1] -= cnt[i];
    }
    for (int i = 1; i <= n; i++)
        printf("%d ", sum[i] += sum[i-1]);
    return 0;
}