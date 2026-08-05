#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <cmath>
constexpr int N = 100005, M = 320;
#define For(i, s, t) for (int i = (s); i <= (t); i++)
struct Blk {
    int l, r, tag, modified;
} blk[M];
int n, B, Btop, p2id[N];
struct Array {
    int a[N];
    int& operator[](int p) { return a[p]; }
    int operator()(int p) const { return std::max(1, a[p] - blk[p2id[p]].tag); }
} a, b;
/* 切入点：设 b[i] 为最深的 i 的祖先，使得 b[i] 和 i 不在同一块。
这样，跳 b[i] 就相当于倍增地去跳，只不过最多跳 O(sqrt n) 次，大块+小块 
*/
inline void initBlk() {
    B = sqrt(n);
    int cntdwn = 0;
    Btop = 0;
    For(i, 1, n) {
        if (!cntdwn) {
            blk[Btop].r = i-1;
            cntdwn = B, ++Btop;
            blk[Btop].l = i;
        }
        p2id[i] = Btop;
        --cntdwn;
    }
    if (cntdwn != B) blk[Btop].r = n;
    else --Btop;
}
inline void rebuild(int id) {
    if (blk[id].modified >= blk[id].r - blk[id].l + 1) return;
    For(i, blk[id].l, blk[id].r) {
        a[i] -= blk[id].tag;
        if (a[i] < 1) a[i] = 1;
    }
    blk[id].tag = 0;
    For(i, blk[id].l, blk[id].r)
        if (a[i] >= blk[id].l)
            b[i] = b[a[i]];
        else b[i] = a[i];
    ++blk[id].modified;
}
int main() {
    int q;
    scanf("%d%d", &n, &q);
    initBlk();
    For(i, 2, n)
        scanf("%d", &a[i]);
    For(i, 1, Btop) rebuild(i);
    For(_q, 1, q) {
        int t, l, r, x;
        scanf("%d%d%d", &t, &l, &r);
        if (t == 1) {
            scanf("%d", &x);
            if (p2id[l] == p2id[r]) {
                For(i, l, r) a[i] -= x;
                rebuild(p2id[l]);
            } else {
                For(i, l, blk[p2id[l]].r) a[i] -= x;
                rebuild(p2id[l]);
                For(i, blk[p2id[r]].l, r) a[i] -= x;
                rebuild(p2id[r]);
                For(i, p2id[l]+1, p2id[r]-1)
                    blk[i].tag += x, rebuild(i);
            }
        }
        else {
            while (p2id[l] != p2id[r]) {
                if (l > r) std::swap(l, r);
                r = b(r);
            }
            while (b(l) != b(r)) l = b(l), r = b(r);
            while (a(l) != a(r)) {
                if (l > r) std::swap(l, r);
                r = a(r);
            }
            if (l == r) printf("%d\n", l);
            else printf("%d\n", a(l));
        }
    }
    return 0;
}