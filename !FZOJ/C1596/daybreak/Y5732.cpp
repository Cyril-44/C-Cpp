#include <bits/stdc++.h>
#define ll long long
using namespace std;
const int fx[5] = {0, -1, 0, 1}, fy[5] = {-1, 0, 1, 0};
const int dx[5] = {-1, 1, 1, -1}, dy[5] = {-1, -1, 1, 1};
int tid, n, m, qu, nn;
ll a[1005][1005], sd, b[400005];
char s[5];
struct node {
    ll d, x, y, z;
} q[100005];
struct BIT {
    int c[400005];
    int lowbit(int x) { return x & (-x); }
    void update(int x, int k) {
        x = nn - x + 1;
        for (int i = x; i <= nn; i += lowbit(i)) { c[i] += k; }
    }
    int query(int x) {
        x = nn - x + 1;
        int cnt = 0;
        for (int i = x; i >= 1; i -= lowbit(i)) { cnt += c[i]; }
        return cnt;
    }
} bit1, bit2, bit3;
void update(int x, int y, int k) {
    bit1.update(a[x][y], k);
    for (int i = 0; i < 4; i++) {
        int xx = x + fx[i], yy = y + fy[i];
        if (xx >= 1 && xx <= n && yy >= 1 && yy <= m) { bit2.update(min(a[x][y], a[xx][yy]), k); }
    }
    for (int i = 0; i < 4; i++) {
        int xx = x + dx[i], yy = y + dy[i];
        if (xx >= 1 && xx <= n && yy >= 1 && yy <= m) { bit3.update(min({a[x][y], a[x][yy], a[xx][y], a[xx][yy]}), k); }
    }
}
int main() {
    cin >> tid >> n >> m >> qu;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            scanf("%lld", &a[i][j]);
            b[++nn] = a[i][j];
        }
    }
    b[++nn] = 0;
    for (int i = 1; i <= qu; i++) {
        scanf("%s", s);
        if (s[0] == 'a') {
            scanf("%lld", &q[i].d);
            q[i].d += sd;
            sd = q[i].d;
            b[++nn] = -q[i].d;
        } else {
            scanf("%lld%lld%lld", &q[i].x, &q[i].y, &q[i].z);
            q[i].d = sd;
            q[i].z -= sd;
            b[++nn] = q[i].z;
        }
    }
    sort(b + 1, b + nn + 1);
    nn = unique(b + 1, b + nn + 1) - b - 1;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            a[i][j] = lower_bound(b + 1, b + nn + 1, a[i][j]) - b;
            bit1.update(a[i][j], 1);
            for (int k = 0; k < 2; k++) {
                int x = i + fx[k], y = j + fy[k];
                if (x >= 1 && x <= n && y >= 1 && y <= m) bit2.update(min(a[i][j], a[x][y]), 1);
            }
            if (i >= 2 && j >= 2) { bit3.update(min({a[i][j], a[i - 1][j], a[i - 1][j - 1], a[i][j - 1]}), 1); }
        }
    }
    for (int i = 1; i <= qu; i++) {
        q[i].d = lower_bound(b + 1, b + nn + 1, -q[i].d) - b + 1;
        if (q[i].x) {
            q[i].z = lower_bound(b + 1, b + nn + 1, q[i].z) - b;
            update(q[i].x, q[i].y, -1);
            a[q[i].x][q[i].y] = q[i].z;
            update(q[i].x, q[i].y, 1);
        }
        printf("%d\n", bit1.query(q[i].d) - bit2.query(q[i].d) + bit3.query(q[i].d));
    }
    return 0;
}