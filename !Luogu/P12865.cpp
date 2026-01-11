#define _GLIBCXX_DEBUG
#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
constexpr int N = 500005;
int a[N], val[N];
struct PSGT {
    struct Node {
        int ls, rs, cnt;
        long long sum;
        Node() : ls(), rs(), cnt(), sum() {}
    };
    PSGT(int n, int m) : tr(1), root(n+1), n(m) { tr.reserve(m+1 << 6); }
    inline void insert(int i, int pos) {
        root[i] = root[i-1];
        P = pos, X = val[pos];
        upd(root[i], 1, n);
    }
    inline long long query(int pos, int k) { // 查 1~pos 元素 前 1~k 小的和
        return que(root[pos], 1, n, k);
    }
private:
    int n, P, X;
    std::vector<Node> tr;
    std::vector<int> root;
    inline void pushup(int u) {
        tr[u].sum = tr[tr[u].ls].sum + tr[tr[u].rs].sum;
        tr[u].cnt = tr[tr[u].ls].cnt + tr[tr[u].rs].cnt;
    }
    void upd(int &u, int l, int r) {
        tr.emplace_back(tr[u]);
        u = tr.size() - 1;
        if (l == r) {
            ++tr[u].cnt;
            tr[u].sum += X;
            return;
        }
        int mid = l + r >> 1;
        if (P <= mid) upd(tr[u].ls, l, mid);
        else upd(tr[u].rs, mid + 1, r);
        pushup(u);
    }
    long long que(int u, int l, int r, int cnt) {
        if (!u) return 0;
        if (l == r) return val[l] * 1ll * cnt;
        if (tr[u].cnt == cnt) return tr[u].sum;
        int mid = l + r >> 1;
        if (cnt <= tr[tr[u].ls].cnt) return que(tr[u].ls, l, mid, cnt);
        return tr[tr[u].ls].sum + que(tr[u].rs, mid + 1, r, cnt - tr[tr[u].ls].cnt);
    }
};
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    memcpy(val, a, sizeof a);
    std::sort(val+1, val+1 + n);
    int m = std::unique(val+1, val+1 + n) - val - 1;
    PSGT f(n, m);
    for (int i = 1; i <= n; i++)
        f.insert(i, std::lower_bound(val+1, val+1 + m, a[i]) - val);
    // 一个序列 (1-index) 排序 k 次之后 [1,p] 一定是原序列 [1,p+k] 中前 p 小的
    int q, k = 0;
    scanf("%d", &q);
    for (int opt, x, y; q--; ) {
        scanf("%d", &opt);
        if (opt == 1) ++k;
        else {
            scanf("%d%d", &x, &y);
            printf("%lld\n", f.query(std::min(n, y + k), y) - f.query(std::min(n, x - 1 + k), x - 1));
        }
    }
    return 0;
}