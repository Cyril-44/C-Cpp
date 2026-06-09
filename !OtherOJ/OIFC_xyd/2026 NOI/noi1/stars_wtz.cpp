#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N = 100010, B = 5000;
int n, m, k, op, x, y, p[N];
int fac[12], ori[N], t[N];
int idx(int *a) {
    int res = 0;
    for (int i = 0; i < k; i++)
        for (int j = i + 1; j < k; j++)
            if (a[i] > a[j]) res += fac[k - i - 1];
    return res;
}
struct Data {
    int cur, pre, now;
};
vector<Data> u[N];
struct Query {
    int l, lid, r, rid, c, id;
    bool operator<(const Query &x) { return lid == x.lid ? (rid == x.rid ? (c < x.c) : rid < x.rid) : lid < x.lid; }
} q[N];
struct MoQueue {
    int a[3628805], l = 1, r = 0, c = 0;
    ll ans = 0;
    void addval(int x) {
        ans += a[t[x]];
        a[t[x]]++;
    }
    void delval(int x) {
        a[t[x]]--;
        ans -= a[t[x]];
    }
    void addck() {
        c++;
        for (const Data &x : u[c]) {
            if (l <= x.cur && x.cur <= r) delval(x.cur);
            t[x.cur] = x.now;
            if (l <= x.cur && x.cur <= r) addval(x.cur);
        }
    }
    void delck() {
        for (const Data &x : u[c]) {
            if (l <= x.cur && x.cur <= r) delval(x.cur);
            t[x.cur] = x.pre;
            if (l <= x.cur && x.cur <= r) addval(x.cur);
        }
        c--;
    }
    void move(int nl, int nr, int nc) {
        while (l > nl) addval(--l);
        while (r < nr) addval(++r);
        while (l < nl) delval(l++);
        while (r > nr) delval(r--);
        while (c < nc) addck();
        while (c > nc) delck();
    }
} d;
struct BIT {
    int a[N], v[N];
    int lowbit(int x) { return x & -x; }
    void update(int x, int y) {
        for (; x <= n; x += lowbit(x)) a[x] += y;
    }
    void modify(int x, int y) {
        // printf("MODIFY %d ori %d new %d det %d\n",x,v[x],y,-v[x]+y);
        update(x, -v[x] + y);
        v[x] = y;
    }
    int query(int x) {
        int res = 0;
        for (; x; x -= lowbit(x)) res += a[x];
        return res;
    }
} bit;
int ck, qc;
int calcPerm(int x) {
    int res = 0;
    for (int i = 1; i < k && x + i <= n - k + 1; i++) res += (t[x] == t[x + i]);
    return res;
}
int mend(int f, int x) {
    int res = 0;
    for (int i = x; i >= max(x - k, f); i--)
        for (int j = x - i + 1; j < k && i + j <= n - k + 1; j++) res += (t[i] == t[i + j]);
    return res;
}
void upd(int c) {
    int tmp = idx(p + c);
    u[ck].push_back((Data){c, t[c], tmp});
    t[c] = tmp;
}
void upd2(int c) {
    bit.modify(c, calcPerm(c));
}
ll ans[N];
int main() {
    freopen("stars.in", "r", stdin);
    freopen("stars.out", "w", stdout);
    scanf("%d%d%d", &n, &m, &k);
    fac[0] = 1;
    for (int i = 1; i <= k; i++) fac[i] = fac[i - 1] * i;
    for (int i = 1; i <= n; i++) scanf("%d", &p[i]);
    for (int i = n - k + 1; i >= 1; i--) ori[i] = t[i] = idx(p + i), bit.modify(i, calcPerm(i));
    int tmp, qid = 0;
    for (int i = 1; i <= m; i++) {
        scanf("%d%d%d", &op, &x, &y);
        if (op == 1) {
            swap(p[x], p[y]);
            ck++;
            //[x-k+1,x] & [y-k+1,y]
            if (x >= y - k + 1) {
                for (int j = min(y, n - k + 1); j >= max(1, x - k + 1); j--) upd(j);
                for (int j = min(y, n - k + 1); j >= max(1, x - 2 * k + 1); j--) upd2(j);
            } else {
                for (int j = min(y, n - k + 1); j >= y - k + 1; j--) upd(j);
                for (int j = x; j >= max(1, x - k + 1); j--) upd(j);
                for (int j = min(y, n - k + 1); j >= max(1, y - 2 * k + 1); j--) upd2(j);
                for (int j = x; j >= max(1, x - 2 * k + 1); j--) upd2(j);
            }
        } else {
            qc++;
            y += -k + 1;
            if (y < x) continue;
            ans[qc] = bit.query(x - 1) - bit.query(y) + mend(x, y);
            qid++;
            q[qid] = (Query){x, (x - 1) / B, y, (y - 1) / B, ck, qc};
        }
    }
    memcpy(t, ori, sizeof(t));
    sort(q + 1, q + qid + 1);
    for (int i = 1; i <= qid; i++) {
        d.move(q[i].l, q[i].r, q[i].c);
        ans[q[i].id] += d.ans;
    }
    for (int i = 1; i <= qc; i++) printf("%lld\n", ans[i]);
    return 0;
}