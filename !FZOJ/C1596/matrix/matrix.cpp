#include <cstdio>
#include <vector>
#include <queue>
#include <cassert>
constexpr int N = 2004;
std::vector<int> pos[N];
int f[N], a[N][N];
int main() {
    int taskid, T, n;
    scanf("%d%d", &taskid, &T);
    while (T--) {
        scanf("%d", &n);
        pos[0].clear();
        for (int i = 1; i <= n; i++)
            scanf("%d", &f[i]), pos[i].clear();
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                a[i][j] = 0;
        for (int i = 1; i <= n; i++) {
            pos[f[i]].push_back(i);
            std::priority_queue<int, std::vector<int>, std::greater<int>> vals;
            for (int j = 0; j < f[i]; j++) vals.push(j);
            for (int k = 1; !vals.empty() && k <= i; k++) {
                bool miss = false;
                for (int j : pos[k]) {
                    if (vals.top() == k) miss = true, vals.pop();
                    if (vals.empty()) break;
                    a[i][j] = vals.top(), vals.pop();
                    if (vals.empty()) break;
                }
                if (miss) vals.push(k);
            }
        }
        for (int i = 1; i <= n; i++)
            for (int j = 1; j < i; j++)
                a[j][i] = a[i][j];
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                printf("%d%c", a[i][j], " \n"[j==n]);
    }
    return 0;
}