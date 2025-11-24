#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <queue>
constexpr int N = 150005;
struct Node {
    int a, b;
    inline bool operator< (const Node &rhs) const {
        return b < rhs.b || b == rhs.b && a < rhs.a;
    }
} d[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d%d", &d[i].a, &d[i].b);
    std::sort(d+1, d+1 + n);
    
    int ans = 0, cnt = 0;
    long long x = 0;
    std::priority_queue<int> q;
    for (int i = 1; i <= n; i++) {
        if (d[i].a > d[i].b) continue;
        q.push(d[i].a);
        x += d[i].a; ++cnt;
        while (x > d[i].b) x -= q.top(), q.pop(), --cnt;
        ans = std::max(ans, cnt);
    }
    printf("%d\n", ans);
    return 0;
}