#include <stdio.h>
#include <set>
#include <map>
#include <algorithm>
typedef std::set<int> Set;
typedef std::pair<int, int> Pair;
typedef std::map<Pair, int> PMap;
const int N = 1005;
int a[N][2], q1, q2, tot, que;
Set s;
PMap pf;
inline bool fetch(int &res) {
    if (pf[Pair(q1, q2)]) {
        res = pf[Pair(q1, q2)];
        return false;
    }
    ++que;
    printf("? %d %d\n", q1, q2);
    fflush(stdout);
    scanf("%d", &res);
    pf[Pair(q1, q2)] = res;
    return true;
}
inline void upd(int &tot) {
    a[tot][0] = q1, a[tot++][1] = q2;
}
inline void dfs(const int &l, const int &r) {
    int res;
    bool ret;
    q1 = l, q2 = r, ret = fetch(res);
    if (res == l) {
        if (ret) upd(tot);
    }
    else {
        dfs(l, res), dfs(res, r);
        if (l ^ a[0][0]) s.erase(l);
        if (r ^ a[0][1]) s.erase(r);
        s.erase(res);
    }
}
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        pf.clear(), s.clear();
        for (int i = 1; i <= n; i++)
            s.insert(i);
        for (tot = 1, que = 0; tot < n; ) {
            a[0][0] = *s.cbegin();
            a[0][1] = *++s.cbegin();
            while (a[0][1] <= n && pf[Pair(a[0][0],a[0][1])]) ++a[0][1];
            if (pf[Pair(a[0][0], a[0][1])]) {
                s.erase(a[0][0]);
                continue;
            }
            dfs(a[0][0], a[0][1]);
        }
        putchar('!');
        for (int i = 1; i < tot; i++)
            printf(" %d %d", a[i][0], a[i][1]);
        putchar('\n');
        fflush(stdout);
    }
    return 0;
}
