#include <stdio.h>
#include <algorithm>
#include <vector>
#include <functional>
using std::min;
const int N = 200005;
std::vector<int> h[N];
int main() {
    int t, n, x, ai, ans;
    scanf("%d", &t);
    while (t--) {
        scanf("%d %d", &n, &x);
        for (int i = 0; i < min(n, x); i++)
            h[i].clear();
        for (int i = 0; i < n; i++) {
            scanf("%d", &ai);
            if (ai < n) h[ai % x].push_back(ai);
        }
        for (int i = 0; i < min(n, x); i++)
            std::sort(h[i].begin(), h[i].end(), std::greater<int>());
        ans = 0;
        for (int i = 0; i < n; i++) {
            if (!h[i % x].empty() && h[i % x].back() <= i)
                ++ans, h[i % x].pop_back();
            else break;
        }
        printf("%d\n", ans);
    }
    return 0;
}