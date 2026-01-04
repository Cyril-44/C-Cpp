#include <bits/stdc++.h>
using namespace std;
//#define int long long
inline void read(int &a) {
    int s = 0, w = 1;
    char ch = getchar();
    while (ch < '0' || ch > '9') {
        if (ch == '-') w = -1;
        ch = getchar();
    }
    while (ch >= '0' && ch <= '9') {
        s = s * 10 + ch - '0';
        ch = getchar();
    }
    a = s * w;
}
void write(int x) {
    if (x < 0) putchar('-'), x = -x;
    if (x > 9) write(x / 10);
    putchar(x % 10 + '0');
    return;
}
const int N = 3e7 + 5;
const int INF = 2147483647;
int siz[N], ls[N], rs[N], val[N], pri[N], root[N];
int cnt, n;
void clone(int a, int b) {
    siz[a] = siz[b];
    val[a] = val[b];
    pri[a] = pri[b];
    ls[a] = ls[b];
    rs[a] = rs[b];
}
void up(int x) {
    if (x) { siz[x] = siz[ls[x]] + siz[rs[x]] + 1; }
}
void Split(int rt, int k, int &rt1, int &rt2) {
    if (!rt) {
        rt1 = rt2 = 0;
        return;
    }
    int new_rt = ++cnt;
    clone(new_rt, rt);
    if (k < val[rt]) {
        rt2 = new_rt;
        Split(ls[rt], k, rt1, ls[rt2]);
        up(rt2);
    } else {
        rt1 = new_rt;
        Split(rs[rt], k, rs[rt1], rt2);
        up(rt1);
    }
}
int Merge(int rt1, int rt2) {
    if (!rt1 || !rt2) return rt1 | rt2;
    int new_rt = ++cnt;
    if (pri[rt1] < pri[rt2]) {
        clone(new_rt, rt1);
        rs[new_rt] = Merge(rs[new_rt], rt2);
        up(new_rt);
        return new_rt;
    } else {
        clone(new_rt, rt2);
        ls[new_rt] = Merge(rt1, ls[new_rt]);
        up(new_rt);
        return new_rt;
    }
}
int Rank(int rt, int v) {
    if (!rt) return 0;
    if (v < val[rt])
        return Rank(ls[rt], v);
    else
        return siz[ls[rt]] + Rank(rs[rt], v) + 1;
}
void insert(int &root, int v) {
    int new_rt = ++cnt;
    val[cnt] = v;
    pri[cnt] = rand();
    siz[cnt] = 1;
    ls[cnt] = rs[cnt] = 0;
    if (!root) {
        root = cnt;
        return;
    }
    int rt1, rt2;
    Split(root, v, rt1, rt2);
    root = Merge(Merge(rt1, new_rt), rt2);
}
void erase(int &root, int v) {
    int rt1, rt2, rt3;
    Split(root, v, rt1, rt3);
    Split(rt1, v - 1, rt1, rt2);
    if (rt2) {
        int new_rt = Merge(ls[rt2], rs[rt2]);
        root = Merge(rt1, Merge(new_rt, rt3));
    } else
        root = Merge(rt1, rt3);
}
int Kth(int rt, int k) {
    if (k <= siz[ls[rt]])
        return Kth(ls[rt], k);
    else if (k == siz[ls[rt]] + 1)
        return val[rt];
    else
        return Kth(rs[rt], k - siz[ls[rt]] - 1);
}
int pre(int rt, int x) {
    if (!rt) return -INF;
    if (val[rt] < x)
        return max(val[rt], pre(rs[rt], x));
    else
        return pre(ls[rt], x);
}
int nex(int rt, int x) {
    if (!rt) return INF;
    if (val[rt] > x)
        return min(val[rt], nex(ls[rt], x));
    else
        return nex(rs[rt], x);
}
signed main() {
    read(n);
    root[0] = 0;
    for (int i = 1; i <= n; i++) {
        int v, opt, x;
        read(v), read(opt), read(x);
        root[i] = root[v];
        if (opt == 1) insert(root[i], x);
        if (opt == 2) erase(root[i], x);
        if (opt == 3) write(Rank(root[i], x - 1) + 1), printf("\n");
        if (opt == 4) write(Kth(root[i], x)), printf("\n");
        if (opt == 5) write(pre(root[i], x)), printf("\n");
        if (opt == 6) write(nex(root[i], x)), printf("\n");
    }
    return 0;
}
