#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <cassert>
constexpr int N = 1004, M = 400004, Q = 100004;
int64_t a[N][N], alla[M];
int top;
struct Query {
    int x, y; int64_t z;
    bool typ;
} qry[Q];
inline int id(int64_t v) { return std::lower_bound(alla+1, alla+1+top, v) - alla; }
struct BIT {
    int tr[M];
    void upd(int p, int x) { for (; p <= top; p += p & -p) tr[p] += x; }
    int sum(int p) { int res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
} sv, sf, se;
int main() {
    int taskid, n, m, q;
    scanf("%d%d%d%d", &taskid, &n, &m, &q);
    alla[++top] = 0;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            scanf("%lld", &a[i][j]), alla[++top] = a[i][j];
    int64_t addtag = 0;
    for (int i = 1; i <= q; i++) {
        char s[5]; int x, y; int64_t z;
        scanf(" %s%d", s, &x);
        if (*s == 'a') {
            addtag += x;
            qry[i] = {0,0,-addtag,0};
            alla[++top] = -addtag;
        } else {
            scanf("%d%lld", &y, &z);
            z -= addtag;
            qry[i] = {x,y,z,1};
            alla[++top] = z;
        }
    }
    std::sort(alla + 1, alla + 1 + top);
    top = std::unique(alla + 1, alla + 1 + top) - alla - 1;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++)
            sv.upd(id(a[i][j]), 1);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j < m; j++)
            se.upd(id(std::min(a[i][j], a[i][j+1])), 1);
    for (int i = 1; i < n; i++)
        for (int j = 1; j <= m; j++)
            se.upd(id(std::min(a[i][j], a[i+1][j])), 1);
    for (int i = 1; i < n; i++)
        for (int j = 1; j < m; j++)
            sf.upd(id(std::min({a[i][j], a[i+1][j], a[i][j+1], a[i+1][j+1]})), 1);
    int64_t baseline = 0;
    for (int i = 1; i <= q; i++) {
        if (qry[i].typ) { // set
            auto change = [&](int x, int y, int v) {
                sv.upd(id(a[x][y]), v);
                for (auto [dx, dy] : {std::pair<int,int>{-1,0},{0,1},{1,0},{0,-1}}) {
                    int tx = x + dx, ty = y + dy;
                    if (1 <= tx && tx <= n && 1 <= ty && ty <= m)
                        se.upd(id(std::min(a[x][y], a[tx][ty])), v);
                }
                for (auto [dx, dy] : {std::pair<int,int>{-1,-1},{-1,0},{0,-1},{0,0}}) {
                    int tx = x + dx, ty = y + dy;
                    if (1 <= tx && tx < n && 1 <= ty && ty < m)
                        sf.upd(id(std::min({a[tx][ty], a[tx+1][ty], a[tx][ty+1], a[tx+1][ty+1]})), v);
                }
            };
            change(qry[i].x, qry[i].y, -1);
            a[qry[i].x][qry[i].y] = qry[i].z;
            change(qry[i].x, qry[i].y, 1);
        } else { // add
            baseline = qry[i].z;
        }
        printf("%d\n", (n*m - sv.sum(id(baseline))) - (n*(m-1) + m*(n-1) - se.sum(id(baseline))) + ((n-1)*(m-1) - sf.sum(id(baseline))));
    }
    return 0;
}