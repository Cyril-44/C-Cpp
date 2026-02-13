#include <bits/stdc++.h>
constexpr int N = 105;
bool map[N][N];
inline void in(bool &x) {
    char ch = getchar();
    while (ch != '0' && ch != '1') ch = getchar();
    x = ch & 1;
}
std::vector<std::pair<int,int>> g[N*N];
int dij(int S, int T) {
    static int dis[N*N];
    static bool vis[N*N];
    std::priority_queue<std::pair<int,int>, std::vector<std::pair<int,int>>, std::greater<std::pair<int,int>>> pq;
    memset(dis, 0x3f, sizeof dis);
    dis[S] = 0, pq.emplace(0, S);
    while (!pq.empty()) {
        auto [dist, u] = pq.top(); pq.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (const auto &[v, w] : g[u])
            if (!vis[v] && dis[v] > dist + w) {
                dis[v] = dist + w;
                pq.emplace(dis[v], v);
            }
    }
    return dis[T];
}
int main() {
    int n, k, a, b, c;
    scanf("%d%d%d%d%d", &n, &k, &a, &b, &c);
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            in(map[i][j]);
    auto getid = [n](int x, int y) { return (x - 1) * n + y; };
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++) {
            static int dis[N][N], pun[N][N];
            static std::pair<int,int> que[N];
            for (int x = std::max(1, i - k), upx = std::min(n, i + k); x <= upx; x++)
                for (int y = std::max(1, j - k), upy = std::min(n, j + k); y <= upy; y++)
                    dis[x][y] = -1;
            int hd=0, tl=0;
            dis[i][j] = pun[i][j] = 0, que[tl++] = {i,j};
            while (hd ^ tl) {
                auto [x, y] = que[hd++];
                if (dis[x][y] >= k) continue;
                for (const auto &[tx, ty] : {std::pair<int,int>{x-1,y},{x+1,y},{x,y-1},{x,y+1}})
                    if (1 <= tx && tx <= n && 1 <= ty && ty <= n && dis[tx][ty] == -1) {
                        dis[tx][ty] = dis[x][y] + 1;
                        pun[tx][ty] = pun[x][y] + (tx < x || ty < y);
                        if (!map[tx][ty]) que[tl++] = {tx, ty};
                    }
            }
            for (int x = std::max(1, i - k), upx = std::min(n, i + k); x <= upx; x++)
                for (int y = std::max(1, j - k), upy = std::min(n, j + k); y <= upy; y++)
                    if (~dis[x][y]) {
                        int baseval = i == 1 && j == 1 ? 0 : map[i][j] ? a : a + c;
                        g[getid(i, j)].emplace_back(getid(x, y), baseval + pun[x][y] * b);
                    }
        }
    printf("%d\n", dij(getid(1, 1), getid(n, n)));
    return 0;
}
