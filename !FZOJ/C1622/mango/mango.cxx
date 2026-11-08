#include <bits/stdc++.h>
#define ll long long
using namespace std;
ll n, ans;
int m, nn, nn_;
map<ll, int> mp;
struct node {
    ll x, k;
    bool operator<(const node &xx) const { return x < xx.x; }
    bool operator>(const node &xx) const { return x > xx.x; }
} a[2000005];
priority_queue<node, vector<node>, greater<node>> q;
void work(ll l, ll r, ll sum) {
    for (ll i = l; i <= r; i++) {
        ll cnt = n / i, j = min(r, n / cnt);
        a[++nn] = {sum * cnt, j - i + 1};
        i = j;
    }
}
int main() {
    cin >> n >> m;
    for (int i = 1; i <= m; i++) {
        ll l, r;
        scanf("%lld%lld", &l, &r);
        mp[l]++, mp[r + 1]--;
    }
    ll sum = 0;
    if (!mp.count(1)) mp[1] = 0;
    if (!mp.count(n + 1)) mp[n + 1] = 0;
    for (auto it = mp.begin(); next(it) != mp.end(); it++) {
        auto it2 = next(it);
        sum += it->second;
        ll l = it->first, r = it2->first - 1;
        work(l, r, sum);
    }
    sort(a + 1, a + nn + 1);
    a[0].x = -1;
    for (int i = 1; i <= nn; i++) {
        if (a[i].x == a[i - 1].x)
            a[nn_].k += a[i].k;
        else
            a[++nn_] = a[i];
    }
    nn = nn_;
    if (!a[1].x) a[1].k = 1;
    for (int i = 1; i <= nn; i++) { q.push({a[i].x, a[i].k}); }
    while (!(q.size() == 1 && q.top().k == 1)) {
        node d = q.top();
        q.pop();
        if (d.k >= 2) {
            ans += d.x * (d.k - (d.k & 1));
            q.push({d.x << 1, d.k >> 1});
        }
        if (d.k & 1) {
            node d2 = q.top();
            q.pop();
            ans += d.x + d2.x;
            d2.k--;
            q.push({d.x + d2.x, 1});
            if (d2.k) q.push({d2.x, d2.k});
        }
    }
    cout << ans << endl;
    return 0;
}