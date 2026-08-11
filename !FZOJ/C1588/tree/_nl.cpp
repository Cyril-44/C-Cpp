#include <bits/stdc++.h>
using namespace std;
int tid, n, m, a[500005];
bool flag[500005];
set<int> s;
struct node {
    int sum, x, y;
    bool operator<(const node xx) const {
        if (sum == xx.sum) {
            if (x == xx.x) return y < xx.y;
            return x < xx.x;
        }
        return sum < xx.sum;
    }
};
priority_queue<node> q;
int main() {
    cin >> tid >> n >> m;
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
        s.insert(i);
        flag[i] = 1;
    }
    for (int i = 0; i < n; i++) { q.push({a[i] + a[(i + 1) % n], i, (i + 1) % n}); }
    for (int i = 1; i <= n - m; i++) {
        node d = q.top();
        q.pop();
        while (!flag[d.x] || !flag[d.y]) {
            d = q.top();
            q.pop();
        }
        if (a[d.x] >= a[d.y]) {
            flag[d.x] = 0;
            s.erase(d.x);
            auto it = s.upper_bound(d.x);
            if (it == s.begin()) it = s.end();
            it--;
            q.push({a[*it] + a[d.y], *it, d.y});
        } else {
            flag[d.y] = 0;
            s.erase(d.y);
            auto it = s.upper_bound(d.y);
            if (it == s.end()) it = s.begin();
            q.push({a[d.x] + a[*it], d.x, *it});
        }
    }
    node d = q.top();
    q.pop();
    while (!flag[d.x] || !flag[d.y]) {
        d = q.top();
        q.pop();
    }
    cout << d.sum << endl;
    return 0;
}