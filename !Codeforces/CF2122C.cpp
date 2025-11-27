#include <cstdio>
#include <algorithm>
#include <cstring>
#include <numeric>
#include <iomanip>
const int N = 200005;
struct Point {
    int x, y, id;
    inline bool operator<(const Point &rhs) const {
        return x < rhs.x || x == rhs.x && y < rhs.y;
    }
} p[N];
bool vis[N];
inline int len(const int &i, const int &j) {
    return abs(p[i].x - p[j].x) + abs(p[i].y - p[j].y);
}
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        memset(vis, 0, n+1);
        for (int i = 1; i <= n; i++) {
            scanf("%d%d", &p[i].x, &p[i].y);
            p[i].id = i;
        }
        std::sort(p+1, p+1 + n);
        for (int i = 1; i <= n/2; i++)
            printf("%d %d\n", p[i].id, p[i+n/2].id);
    }
    return 0 ;
}