#include <stdio.h>
#include <algorithm>
constexpr int N = 5005, MOD = 998244353;
int pw2[N], C[N][N];
struct Node {
    int price, id;
    explicit operator int() const { return price; }
} a[N];
inline bool operator< (const Node &x, const Node &y) {
    return (int)x > (int)y || int(x) == int(y) && x.id < y.id;
}
inline void add(int &x, int y) { if ((x += y) >= MOD) x -= MOD; }
int main() {
    pw2[0] = 1;
    int Tid, T, n = 5000, m;
    for (int i = 1; i <= n; i++) add(pw2[i] = pw2[i-1], pw2[i-1]);
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            add(C[i][j] = C[i-1][j-1], C[i-1][j]);
    }
    scanf("%d%d", &Tid, &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i].price), a[i].id = i;
        std::sort(a+1, a+1+n);
        int ans = 0; // 不合法的方案数
        for (int i = 1; i < n; i++)
            for (int j = i+1; j <= n; j++) {
                int L = std::upper_bound(a+1, a+1+n, 2 * int(a[i]), [](int x, const Node&y){return x > (int)y;}) - a;
                int R = std::lower_bound(a+1, a+1+n, int(a[i]) + int(a[j]), [](const Node&x, int y){return int(x) > y;}) - a;
                for (int k = L; k < R; k++) { // k 就是当前选定的 ai+aj < ak
                    int force1Cnt = k - L;
                    // 现在要选 k(2), ...(cnt个1)..., i(1), j(1)，前面的剩余的 (i - 2 - cnt) 个随便选
                    if (i - 2 - force1Cnt >= 0 && m - i >= 0) add(ans, 1ull* C[i - 2 - force1Cnt][m - i] * pw2[n - j]);
                }
            }
        for (int i = 1; i < n; i++) {
            int L = std::upper_bound(a+1, a+1+n, 2 * int(a[i]), [](int x, const Node&y){return x > (int)y;}) - a;
            // L~k 都要是 1，这样才有 Corner Case 1 2...2 0，而且后面也没有自由选择的空间
            for (int k = L; k < i; k++) {
                int force1Cnt = k - L;
                if (i - 2 - force1Cnt >= 0 && m - i >= 0) add(ans, C[i - 2 - force1Cnt][m - i]);
            }
        }
        printf("%d\n", (pw2[n] - ans + MOD) % MOD);
    }
    return 0;
}