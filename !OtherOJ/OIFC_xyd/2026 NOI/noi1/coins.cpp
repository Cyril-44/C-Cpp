#include <cstdio>
#include <iostream>
#include <algorithm>
constexpr int N = 1000005;
int n;
/* 首先将原问题转化为经典的 Nim 博弈，于是得到 SG 值的异或和不为 0 ==> 先手必胜
 * 维护区间 SG 函数值的异或和，最后求解 YESNO(XorSumOfSG[1..n] != 0) 即可
 * 输出最小，我们显然需要让下一个状态的 SG 函数异或和为 0 使得后手必败，然后我们就可以发现
 * 有的时候我们可以选取两个数，比如 x, y；设当前 SG 函数异或和为 f0，我们要让 f0 ^ x ^ y = 0，且 y < x。
 * 我们立刻想到，x 必须包含 f0 的最高位，否则肯定无法结果是 0。我们只需要取包含这样的最高位中最小的一个数就可以了。
 */
class SegTr {
    struct Node {
        unsigned xorSum, mask, revMask, rangeXorSum;
        bool rev;
        void reverse() {
            xorSum = rangeXorSum ^ xorSum;
            std::swap(mask, revMask);
            rev = !rev;
        }
    } tr[N << 2];
#define ls (u<<1)
#define rs (u<<1|1)
    void pushup(int u) {
        tr[u].xorSum = tr[ls].xorSum ^ tr[rs].xorSum;
        tr[u].mask = tr[ls].mask | tr[rs].mask;
        tr[u].revMask = tr[ls].revMask | tr[rs].revMask;
    }
    void pushdown(int u) {
        if (tr[u].rev) {
            tr[ls].reverse(), tr[rs].reverse();
            tr[u].rev = false;
        }
    }
    unsigned L, R;
    void upd(int u, unsigned l, unsigned r) {
        if (L <= l && r <= R) return tr[u].reverse();
        unsigned mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(ls, l, mid);
        if (mid < R) upd(rs, mid+1, r);
        pushup(u);
    }
public:
    void build(int u=1, unsigned l=1, unsigned r=n) {
        if (l == r) return void(tr[u] = {l, l, 0, l, false});
        unsigned mid = l + r >> 1;
        build(ls, l, mid); build(rs, mid+1, r);
        pushup(u); tr[u].rangeXorSum = tr[ls].rangeXorSum ^ tr[rs].rangeXorSum;
    }
    void reverse(int l, int r) { L=l, R=r; upd(1,1,n); }
    int sg() { return tr[1].xorSum; }
    int inquire(int bit) {
        if (!(tr[1].mask & 1u << bit)) return -1; 
        int u = 1, l = 1, r = n;
        while (l < r) {
            int mid = l + r >> 1;
            pushdown(u);
            if (tr[ls].mask & 1u << bit) r = mid, u = ls;
            else l = mid+1, u = rs;
        }
        return l;
    }
#undef ls
#undef rs
} fsg;
int main() {
    freopen("coins.in", "r", stdin);
    freopen("coins.out", "w", stdout);
    std::cin.tie(nullptr)->sync_with_stdio(false);
    int m; std::cin >> n >> m;
    fsg.build();
    for (int l, r; m--; ) {
        std::cin >> l >> r;
        fsg.reverse(l, r);
        int sg = fsg.sg();
        if (!sg) std::cout << "-1\n";
        else std::cout << fsg.inquire(31 - __builtin_clz(sg)) << '\n';
    }
    return 0;
}