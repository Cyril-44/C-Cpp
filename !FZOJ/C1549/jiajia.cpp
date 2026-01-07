#include <stdio.h>
#include <algorithm>
constexpr int N = 40005, M = 100005;
struct Oper {
    int l, r;
    inline bool operator<(const Oper &rv) const {
        return l < rv.l || l == rv.l && r < rv.r;
    }
} op[M];
int c[N], sum[N], red[N];
std::vector<int> ops[N];
int main() {
    int n, m, k;
    Initialization: {
        scanf("%d%d%d", &n, &m, &k);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &c[i]);
            ops[i].reserve(k * 2);
        }
        for (int i = 1; i <= m; i++) {
            scanf("%d%d", &op[i].l, &op[i].r);
            ++sum[op[i].l], --sum[op[i].r+1];
        }
        int n0 = 0;
        for (int i = 1; i <= n; i++) {
            sum[i] += sum[i-1];
            red[i] = red[i-1];
            if (sum[i] > k*2) ++red[i];
            else c[++n0] = c[i];
        }
        int m0 = 0;
        for (int i = 1; i <= m; i++) {
            op[i].l -= red[op[i].l - 1];
            op[i].r -= red[op[i].r];
            if (op[i].l <= op[i].r) op[++m0] = op[i];
        }
        m = m0, n = n0;
        std::sort(op+1, op+1 + m);
        for (int i = 1; i <= m; i++)
            for (int j = op[i].l; j <= op[i].r; ++j)
                ops[j].push_back(i);
    }
    Solving: {
        printf("m=%d,n=%d\n", m, n);
        for (int i = 1; i <= m; i++)
            printf("[%d, %d]\n", op[i].l, op[i].r);
        for (int i = 1; i <= n; i++) {
            if (ops[i].empty()) { putchar('\n'); continue; }
            printf("%d", ops[i].front());
            for (auto it = std::next(ops[i].begin()); it != ops[i].end(); ++it)
                printf(" +%d", *it - ops[i].front());
            putchar('\n');
        }
    }
    Representing: {

    }
    return 0;
}