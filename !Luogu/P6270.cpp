#include <bits/extc++.h>
constexpr int N = 100005;
using Op = std::tuple<int,int,int>;
int s[N], t[N];
struct Card {
    int s, t;
} a[N];
struct CmpS { bool operator()(int i, int j) const { return a[i].s < a[j].s; } };
struct CmpT { bool operator()(int i, int j) const { return a[i].t < a[j].t; } };
using PqS = __gnu_pbds::priority_queue<int, CmpS>;
using PqT = __gnu_pbds::priority_queue<int, CmpT>;
int to[N];
PqS::point_iterator ps[N];
PqT::point_iterator pt[N];
int main() {
    int T, n, m;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++) {
            scanf("%d%d", s+i, t+i);
            a[i] = {s[i], t[i]};
        }
        std::sort(s+1, s+1+n), std::sort(t+1, t+1+n);
        int64_t ans = 0;
        for (int i = 1; i <= n; i++)
            ans += std::abs(t[i] - s[i]);
        PqS qs; PqT qt;
        auto push = [&qs, &qt](int v) { ps[v] = qs.push(v), pt[v] = qt.push(v); };
        for (int i = 1; i <= n; i++) push(i);
        std::vector<Op> ops1, ops2;
        auto ins = [&](std::vector<Op> &vec, bool f, int x, int y) { if (f || a[x].s != y) vec.emplace_back(f, x, y); };
        for (int _ = n; _ >= 1; _--) {
            int x = qs.top(), y = qt.top();
            qs.pop(), qt.pop();
            if (x == y) {
                ins(ops1, 0, x, a[x].t);
                continue;
            }
            qs.erase(ps[y]);
            qt.erase(pt[x]);
            auto op1 = [&] {
                ins(ops1, 0, x, a[x].t); // sx --> tx
                ins(ops2, 0, y, a[y].t); // tx --> ty
                ins(ops2, 1, x, y);
                a[y].t = a[x].t; // sy --> tx
                push(y);
            };
            auto op2 = [&] {
                ins(ops1, 0, x, a[y].t); // sx --> ty
                ins(ops1, 0, y, a[y].t); // sy --> ty
                ins(ops1, 1, x, y);
                a[x].s = a[y].t; // ty --> tx
                push(x);
            };
            auto op3 = [&] {
                ins(ops1, 0, x, a[y].s); // sx --> sy
                ins(ops1, 1, x, y);
                ins(ops1, 0, y, a[y].t); // sy --> ty
                a[x].s = a[y].s; // sy --> tx
                push(x);
            };
            if (a[x].s < a[x].t) {
                if (a[y].s <= a[x].s && a[x].t <= a[y].t) op1();
            } else if (a[y].s > a[y].t) op2();
            else if (a[x].s <= a[y].t) {
                if (a[x].t >= a[y].s) op1();
                else op3();
            } else op2();
        }
        printf("%lld %zu\n", ans, ops1.size() + ops2.size());
        std::reverse(ops2.begin(), ops2.end());
        for (const auto &t : ops1) printf("%d %d %d\n", std::get<0>(t), std::get<1>(t), std::get<2>(t));
        // fprintf(stderr, "=========\n");
        for (const auto &t : ops2) printf("%d %d %d\n", std::get<0>(t), std::get<1>(t), std::get<2>(t));
    }
    return 0;
}
