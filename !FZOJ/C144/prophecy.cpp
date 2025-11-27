#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
template <class T>
void in(T &x) {
    char c = getchar();
    T f = 1;
    x = 0;
    while (c < '0' || c > '9') {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
const int N = 500010;
int n, a[N], qry, l, r;
int lgv[N];
struct Node {
    int val, id;
} f[20][N];
Node operator+(const Node &a, const Node &b) {
    if (a.val > b.val)
        return a;
    return b;
}
Node query(int l, int r) {
    int len = lgv[r - l + 1];
    return f[len][l] + f[len][r - (1 << len) + 1];
}
int qryy(int l, int r) {
    int len = lgv[r - l + 1];
    return max(f[len][l].val, f[len][r - (1 << len) + 1].val);
}
struct Seg {
    int l, r, id;
    bool operator<(const Seg &x) const { return a[id] < a[x.id]; }
    bool operator>(const Seg &x) const { return a[id] > a[x.id]; }
};
int main() {
    freopen("prophecy.in", "r", stdin);
    freopen("prophecy.out", "w", stdout);
    in(n);
    lgv[1] = 0;
    for (int i = 2; i <= n; i++)
        lgv[i] = lgv[i / 2] + 1;
    for (int i = 1; i <= n; i++)
        in(a[i]), f[0][i] = (Node){ a[i], i };
    for (int i = 1; i <= 19; i++)
        for (int j = 1; j + (1 << i) - 1 <= n; j++)
            f[i][j] = f[i - 1][j] + f[i - 1][j + (1 << i - 1)];
    in(qry);
    Node tmp;
    Seg t;
    while (qry--) {
        in(l);
        in(r);
        tmp = query(l, r);
        priority_queue<Seg> q;
        q.push((Seg){ l, r, tmp.id });
        vector<int> mx;
        while ((!q.empty()) && mx.size() <= 20) {
            t = q.top();
            q.pop();
            mx.push_back(t.id);
            if (t.l < t.id) {
                tmp = query(t.l, t.id - 1);
                q.push((Seg){ t.l, t.id - 1, tmp.id });
            }
            if (t.id < t.r) {
                tmp = query(t.id + 1, t.r);
                q.push((Seg){ t.id + 1, t.r, tmp.id });
            }
        }
        sort(mx.begin(), mx.end());
        int ans = 0;
        for (int i = 0; i < mx.size(); i++) {
            if (mx[i] + 1 < mx[i + 1] && i + 1 != mx.size())
                ans = max(ans, qryy(mx[i] + 1, (mx[i] + mx[i + 1]) / 2) + a[mx[i]] + a[mx[i + 1]]);
            for (int j = i + 2; j < mx.size(); j++) {
                // if(mx[i]+1<mx[j])
                ans = max(ans, qryy(mx[i] + 1, (mx[i] + mx[j]) / 2) + a[mx[i]] + a[mx[j]]);
            }
        }
        printf("%d\n", ans);
    }
}