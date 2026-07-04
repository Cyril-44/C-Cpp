#include <cstdio>
#include <algorithm>
#include <map>
#include <tuple>
constexpr int N = 3005;
std::map<int, std::pair<int, std::map<int,int>>> mp;
struct Point { int x, y, z; } a[N];
int main() {
    int n, q;
    scanf("%d%d", &n, &q);
    for (int i = 1, x, y, z; i <= n; i++) {
        scanf("%d%d%d", &x, &y, &z);
        a[i] = {x, y, z};
        ++mp[y].first;
        ++mp[y].second[z];
    }
    for (int k; q--; ) {
        scanf("%d", &k);
        int mn = 1e9;
        for (int i = 1; i <= n; i++) {
            auto it = mp.find(a[i].y + k);
            if (it != mp.end()) {
                auto zcnt = it->second.second.find(a[i].z + k);
                if (it->second.first - (zcnt != it->second.second.end() ? zcnt->second : 0))
                    mn = std::min(mn, a[i].x);
            }
        }
        printf("%d\n", mn == (int)1e9 ? -1 : mn);
    }
    return 0;
}