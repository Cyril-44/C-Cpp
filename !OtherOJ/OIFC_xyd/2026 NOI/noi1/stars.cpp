#include <cstdio>
#include <algorithm>
#include <cstring>
#include <cstdint>
#include <map>
#include <vector>
#include <iostream>
constexpr int N = 15;
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define roF(i, s, t) for (int i = (s); i >= (t); --i)
int p[N], len;
int perm[N];
unsigned fact[15];
inline unsigned calc(int i) {
    unsigned res = 0;
    for (int j = 0; j < len; j++)
        for (int k = j+1; k < len; k++)
            if (p[i+k] < p[i+j]) res += fact[len - j - 1];
    return res;
}
struct UpdateNode { int idx; unsigned pre, nxt; };
using Update = UpdateNode[20];
Update upds[N];

struct MoImpl {
    int cnt[N]; // 将贡献 C(x,2) 拆成 0 + 1 + 2 + ... + (x-1)，每次加入贡献的增量就是 cnt[x]-1
    int64_t ans;
    void addPerm(unsigned permCode) { ans += cnt[permCode]++; }
    void delPerm(unsigned permCode) { ans += --cnt[permCode]; }
    void patch(Update upd) { For (i, 0, 2*len-1) perm[upd[i].idx] = upd[i].nxt; }
};
int main() {
    freopen("stars.in", "r", stdin);
    freopen("stars.out", "w", stdout);
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    fact[0] = 1;
    For(i, 1, 10) fact[i] = fact[i-1] * i;
    int n, m; std::cin >> n >> m >> len;
    For(i, 1, n) std::cin >> p[i];
    For(i, 1, n-len+1) perm[i] = calc(i);
    for (int op, x, y; m--; ) {
        std::cin >> op >> x >> y;
        if (op == 1) {
            std::swap(p[x], p[y]);
            For (i, std::max(1, x-len+1), std::min(x, n-len+1)) perm[i] = calc(i);
            For (i, std::max(1, y-len+1), std::min(y, n-len+1)) perm[i] = calc(i);

        }
        else {
            std::map<unsigned, std::vector<int>> mp;
            for (int i = x; i + len - 1 <= y; i++) mp[perm[i]].push_back(i);
            unsigned ans = 0;
            for (const auto& [_, w] : mp) {
                for (int i = 0, j; i < (int)w.size(); i++) {
                    for (j = i + 1; j < (int)w.size() && w[j] - w[i] < len; j++);
                    ans += (int)w.size() - j;
                }
            }
            printf("%u\n", ans);
        }
    }
    return 0;
}