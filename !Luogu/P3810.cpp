#include <cstdio>
#include <algorithm>
#include <cstring>
constexpr int N = 200005;
struct Node {
    int a, b, c, cnt, res;
    inline bool operator<(const Node &x) const {
        return a < x.a || a == x.a && (b < x.b || b == x.b && c < x.c);
    }
    inline bool operator!=(const Node &x) const {
        return a != x.a || b != x.b || c != x.c;
    }
} e[N];
int k;
long long f[N];
inline void dfs(const int &l, const int &r) {
    static int bitr[N];
    static auto add = [&] (int p, const int &l) -> void {
        while (p <= k) {
            bitr[p] += l;
            p += p & -p;
        }
    };
    static auto query = [&] (int p) -> int {
        int res = 0;
        while (p > 0) {
            res += bitr[p];
            p -= p & -p;
        }
        return res;
    };
    static auto cmp = [] (const Node &x, const Node &y) -> bool {
        return x.b < y.b || x.b == y.b && x.c < y.c;
    };
    if (l >= r) return;
    int mid = l + r >> 1;
    dfs(l, mid); dfs(mid+1, r);
    std::sort(e + l, e + mid + 1, cmp);
    std::sort(e + mid + 1, e + r + 1, cmp);
    int i = l;
    for (int j = mid + 1; j <= r; j++) { // 类似于归并，左边只对右边产生影响
        while (i <= mid && e[i].b <= e[j].b) // 确保 现在的 e[j].b >= e[i].b 
            add(e[i].c, e[i].cnt), ++i;
        e[j].res += query(e[j].c); // 直接加上 e[i].c <= e[j].c 的个数
    }
    while (i > l)
        --i, add(e[i].c, -e[i].cnt);
}
int main() {
    int n;
    scanf("%d%d", &n, &k);
    for (int i = 0; i < n; i++)
        scanf("%d%d%d", &e[i].a, &e[i].b, &e[i].c);
    std::sort(e, e + n);
    int m(0), tp(0);
    for (int i = 0; i < n; i++) {
        ++tp;
        if (e[i] != e[i+1]) {
            e[m] = e[i];
            e[m].cnt = tp;
            ++m;
            tp = 0;
        }
    }
    dfs(0, m-1);
    for (int i = 0; i < m; i++)
        f[e[i].res + e[i].cnt - 1] += e[i].cnt;
    for (int i = 0; i < n; i++)
        printf("%d\n", f[i]);
    return 0;
}