#include <bits/extc++.h>
using namespace __gnu_pbds;
constexpr int N = 300005;
int fa[N];
inline int find(int u) {
    int v = u, tp;
    while (u ^ fa[u]) u = fa[u];
    while (v ^ u) tp = fa[v], fa[v] = u, v = tp;
    return u;
}
using Pit = priority_queue<int>::point_iterator;
Pit pit[N];
struct Node {
    int addtag;
    priority_queue<int> pq;
    Pit to;
    inline int get() const { return pq.top() + addtag; }
} a[N];
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) fa[i] = i;
    priority_queue<int> allpq;
    for (int i = 1, x; i <= n; i++) {
        scanf("%d", &x);
        pit[i] = a[i].pq.push(x);
        a[i].to = allpq.push(x);
    }
    int q;
    scanf("%d", &q);
    char s[4];
    int globaddtag = 0;
    for (int x, y; q--;) {
        scanf("%s", s);
        switch (*s) {
        case 'U': {
            scanf("%d%d", &x, &y);
            x = find(x), y = find(y);
            if (x == y) continue;
            if (a[x].pq.size() > a[y].pq.size()) std::swap(x, y);
            std::vector<Pit> pits; pits.reserve(a[x].pq.size());
            for (auto it = a[x].pq.begin(); it != a[x].pq.end(); ++it)
                pits.push_back(it);
            for (auto &it : pits) a[x].pq.modify(it, *it + a[x].addtag - a[y].addtag);
            a[y].pq.join(a[x].pq);
            fa[x] = y;
            allpq.erase(a[x].to);
            allpq.modify(a[y].to, a[y].get());
            break;
        } case 'A':
            switch (s[1]) {
            case '1':
                scanf("%d%d", &x, &y);
                a[find(x)].pq.modify(pit[x], *pit[x] + y);
                allpq.modify(a[find(x)].to, a[find(x)].get());
                break;
            case '2':
                scanf("%d%d", &x, &y);
                a[find(x)].addtag += y;
                allpq.modify(a[find(x)].to, a[find(x)].get());
                break;
            case '3':
                scanf("%d", &x);
                globaddtag += x;
            } break;
        case 'F':
            switch (s[1]) {
            case '1':
                scanf("%d", &x);
                printf("%d\n", globaddtag + a[find(x)].addtag + *pit[x]);
                break;
            case '2':
                scanf("%d", &x);
                printf("%d\n", globaddtag + a[find(x)].get());
                break;
            case '3':
                printf("%d\n", globaddtag + allpq.top());
            }
        }
    }
    return 0;
}