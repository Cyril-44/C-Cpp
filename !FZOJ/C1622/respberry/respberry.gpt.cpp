
#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using P = pair<ll, ll>;

int main() {
    ll n; int m;
    scanf("%lld%d", &n, &m);

    vector<P> d{{1, 0}, {n + 1, 0}}, a, b;
    for (ll l, r; m--; ) {
        scanf("%lld%lld", &l, &r);
        d.emplace_back(l, 1);
        d.emplace_back(r + 1, -1);
    }
    sort(d.begin(), d.end());

    ll sum = 0;
    for (int i = 0; i + 1 < (int)d.size();) {
        ll l = d[i].first;
        while (i < (int)d.size() && d[i].first == l)
            sum += d[i++].second;
        if (i == d.size()) break;
        ll r = d[i].first - 1;
        for (ll x = l, y; x <= r; x = y + 1) {
            ll v = n / x;
            y = min(r, n / v);
            a.emplace_back(sum * v, y - x + 1);
        }
    }

    sort(a.begin(), a.end());
    int k = 0;
    for (auto [v, c] : a) {
        if (k && a[k - 1].first == v)
            a[k - 1].second += c;
        else a[k++] = {v, c};
    }
    a.resize(k);

    int i = 0, j = 0;
    auto get = [&]() -> ll {
        if (i < a.size() && (j == b.size() || a[i].first <= b[j].first)) {
            ll v = a[i].first;
            if (!--a[i].second) ++i;
            return v;
        }
        ll v = b[j].first;
        if (!--b[j].second) ++j;
        return v;
    };

    ll ans = 0, tot = n;
    while (tot > 1) {
        bool f = i < a.size() && (j == b.size() || a[i].first <= b[j].first);
        P &p = f ? a[i] : b[j];
        ll v = p.first, c = p.second;

        if (c > 1) {
            ll t = c / 2;
            ans += 2 * v * t;
            p.second %= 2;
            if (!p.second) {
                if (f) ++i;
                else ++j;
            }
            b.emplace_back(2 * v, t);
            tot -= t;
        } else {
            ll x = get(), y = get();
            ans += x + y;
            b.emplace_back(x + y, 1);
            --tot;
        }
    }
    printf("%lld\n", ans);
}
