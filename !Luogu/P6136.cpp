#include <cstdio>
#include <cstdlib>
#ifdef ONLINE_JUDGE
#define debug(...) 114514
#else
#define debug(...) fprintf(stderr, __VA_ARGS__)
#endif
const int N = 100005, M = 1000005;
namespace Splay {
struct Node {
    int l, r, n, mx, size, cnt;
} tr[N + M];
int top = 0;

#define Ls(u) (tr[u].l)
#define Rs(u) (tr[u].r)
inline void pushup(const int &u) {
    tr[u].size = tr[u].cnt;
    if (Ls(u))
        tr[u].size += tr[Ls(u)].size;
    if (Rs(u))
        tr[u].size += tr[Rs(u)].size, tr[u].mx = tr[Rs(u)].mx;
    else
        tr[u].mx = tr[u].n;
}
inline void rotate_left(int &u) {
    int r = Rs(u);
    Rs(u) = Ls(r), Ls(r) = u;
    pushup(u), pushup(u = r);
}
inline void rotate_right(int &u) {
    int l = Ls(u);
    Ls(u) = Rs(l), Rs(l) = u;
    pushup(u), pushup(u = l);
}

typedef __uint8_t NodeStatus;
constexpr int ROOT = 0, LEFT = 1, RIGHT = 2;
#define _Do_L(func)                                                            \
    switch (func(Ls(u), x)) {                                                  \
    case LEFT: /* LL */                                                        \
        rotate_right(u);                                                       \
        rotate_right(u);                                                       \
        return ROOT;                                                           \
    case RIGHT: /* LR */                                                       \
        rotate_left(Ls(u));                                                    \
        rotate_right(u);                                                       \
        return ROOT;                                                           \
    default:                                                                   \
        return LEFT;                                                           \
    }
#define _Do_R(func)                                                            \
    switch (func(Rs(u), x)) {                                                  \
    case LEFT: /* RL */                                                        \
        rotate_right(Rs(u));                                                   \
        rotate_left(u);                                                        \
        return ROOT;                                                           \
    case RIGHT: /* RR */                                                       \
        rotate_left(u);                                                        \
        rotate_left(u);                                                        \
        return ROOT;                                                           \
    default:                                                                   \
        return RIGHT;                                                          \
    }
#define splay(func, u, x)                                                      \
    switch (Splay::func(u, x)) {                                               \
    case Splay::LEFT:                                                          \
        Splay::rotate_right(u);                                                \
        break;                                                                 \
    case Splay::RIGHT:                                                         \
        Splay::rotate_left(u);                                                 \
        break;                                                                 \
    }

inline NodeStatus insert(int &u, const int &x) {
    if (!u) { /* 新建节点 */
        tr[u = ++top] = Node{0, 0, x, x, 1, 1};
        return ROOT;
    }
    if (x < tr[u].n)
        _Do_L(insert);
    if (x == tr[u].n) {
        ++tr[u].cnt, ++tr[u].size;
        return ROOT;
    }
    _Do_R(insert);
}
inline NodeStatus mv2x(int &u, const int &x) {
    // debug("%d %d %d %d %d %d %d\n", u, tr[u].cnt, tr[u].size, tr[u].n,
    // tr[u].mx, tr[u].l, tr[u].r);
    if (Ls(u) && x <= tr[Ls(u)].mx)
        _Do_L(mv2x);
    if (x <= tr[u].n || !Rs(u))
        return ROOT;
    _Do_R(mv2x)
}
inline NodeStatus mv2kth(int &u, int x) {
    // debug("%d %d %d %d %d %d %d\n", u, tr[u].cnt, tr[u].size, tr[u].n,
    // tr[u].mx, tr[u].l, tr[u].r);
    if (Ls(u) && x <= tr[Ls(u)].size)
        _Do_L(mv2kth);
    if (Ls(u))
        x -= tr[Ls(u)].size;
    if (x <= tr[u].cnt)
        return ROOT;
    x -= tr[u].cnt;
    _Do_R(mv2kth);
}
inline void erase(int &u, const int &x) {
    splay(mv2x, u, x);
    --tr[u].cnt;
    if (!tr[u].cnt) {
        splay(mv2kth, Rs(u), 1);
        Ls(Rs(u)) = Ls(u);
        u = Rs(u);
    }
}
#undef _Do_L
#undef _Do_R
} // namespace Splay
using Splay::tr;
inline void in(int &x) {
    register char ch = getchar();
    while (ch < '0' || ch > '9')
        ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x << 3) + (x << 1) + (ch ^ '0');
}
int main() {
#ifndef ONLINE_JUDGE
    freopen("P6136_11.in", "r", stdin);
    freopen("debug.txt", "wt", stderr);
#endif
    int cur = 0;
    // 手动添加哨兵
    tr[cur = ++Splay::top] = Splay::Node{0, 0, -1, -1, 1, 1};
    tr[cur = ++Splay::top] = Splay::Node{1, 0, 0x7fffffff, 0x7fffffff, 1, 0};
    Splay::pushup(cur);

    int n, m, ans = 0;
    in(n), in(m);
    for (int i = 1, ai; i <= n; i++) {
        in(ai);
        splay(insert, cur, ai);
    }
    for (int opt, x, last_ans = 0; m--;) {
        in(opt), in(x), x ^= last_ans;
        switch (opt) {
        case 1:
            splay(insert, cur, x);
            break;
        case 2:
            Splay::erase(cur, x);
            break;
        case 3:
            splay(mv2x, cur, x);
            ans ^= last_ans = tr[Ls(cur)].size;
            break;
        case 4:
            splay(mv2kth, cur, x + 1);
            ans ^= last_ans = tr[cur].n;
            break;
        case 5:
            splay(mv2x, cur, x);
            debug("%d %d ", cur, tr[cur].n);
            splay(mv2x, Ls(cur), 0x7fffffff);
            ans ^= last_ans = tr[Ls(cur)].n;
            break;
        case 6:
            splay(mv2x, cur, x + 1);
            ans ^= last_ans = tr[cur].n;
            break;
        }
        // debug("(%d %d): ans=%d cur=%d\n", opt, x, last_ans, cur);
    }
    printf("%d\n", ans);
    return 0;
}