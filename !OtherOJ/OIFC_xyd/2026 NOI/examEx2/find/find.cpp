#include <cstdio>
#include <cstring>
#include <bitset>
#include <vector>
#include <algorithm>
constexpr int N = 200001, B = 5000;
using BS = std::bitset<N>;
struct Point {
    int x, y, z;
    friend bool operator<(const Point &l, const Point &r) { return l.x < r.x; }
} a[N];
std::vector<int> zlist[N << 1];
int cnt[N], ans[N];
BS *bs[N << 1], ally;
BS getdiff(const std::vector<int>& op) {
    BS res;
    for (int y : op) if (--cnt[y] == 0) res[y] = 1;
    for (int y : op) ++cnt[y];
    return res;
}
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    BS unsetAns; unsetAns.set(); unsetAns[0] = 0;
    for (int i = 1; i <= n; i++) {
        scanf("%d%d%d", &a[i].x, &a[i].y, &a[i].z);
        a[i].z = a[i].z - a[i].y + N; // 变成颜色
        ally[a[i].y] = 1; ++cnt[a[i].y];
        zlist[a[i].z].push_back(a[i].y);
    }
    std::sort(a+1, a+1 + n);
    auto diff = [&](int z) -> BS {
        if (zlist[z].size() > B) {
            if (!bs[z]) bs[z] = new BS(getdiff(zlist[z]));
            return *bs[z];
        }
        return getdiff(zlist[z]);
    };
    for (int i = 1; i <= n; i++) { // 按 x 从小到大去更新
        BS aval = ally ^ diff(a[i].z); // 可用的所有颜色的 y 集合，满足 aj.z != ai.z
        aval = (aval >> a[i].y) & unsetAns; // 因为我们要存在 aj.y = ai.y + k，那么这样的 k 就是所有的 aj.y - ai.y
        for (size_t pos = aval._Find_first(); pos != aval.size(); pos = aval._Find_next(pos)) // 注意只需要更新未更新的位置
            ans[pos] = a[i].x;
        // auto str = aval.to_string(); std::reverse(str.begin(), str.end());
        // std::cerr << str;
        // fprintf(stderr, " %d\n", a[i].x);
        unsetAns ^= aval; // 更新过了
    }
    for (int k; q--; ) {
        scanf("%d", &k);
        printf("%d\n", unsetAns[k] ? -1 : ans[k]);
    }
    return 0;
}