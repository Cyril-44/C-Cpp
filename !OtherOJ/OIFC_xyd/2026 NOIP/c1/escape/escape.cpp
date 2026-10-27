#include <bits/stdc++.h>
constexpr int N = 100005;
struct Stone {
    int s, p;
    bool operator<(const Stone &o) const {
        return p < o.p;
    }
} a[N];
int l[N], r[N], id[N], f1[N], f2[N];
int main(){
    freopen("escape.in", "r", stdin);
    freopen("escape.out", "w", stdout);
    int n;
    scanf("%d", &n);
    std::map<int, int> mp;
    for (int i = 1, s, p; i <= n; i++) {
        scanf("%d%d", &s, &p);
        mp[p] = std::max(mp[p], s);
    }
    n = 0;
    for (auto [p, s] : mp) a[++n] = {s, p};
    for (int i = 1; i <= n; i++) {
        l[i] = std::lower_bound(a+1, a+1+n, Stone{0, a[i].p - a[i].s}) - a;
        r[i] = std::upper_bound(a+1, a+1+n, Stone{0, a[i].p + a[i].s}) - a-1;
    }
    std::set<std::pair<int,int>> st;
    auto ins = [&](int p) {
        auto it = st.insert({p, p}).first;
        int lp = p, rp = p;
        if (it != st.begin() && std::prev(it)->second+1 == p)
            lp = std::prev(it)->first, st.erase(std::prev(it));
        if (it != --st.end() && std::next(it)->first-1 == p)
            rp = std::next(it)->second, st.erase(std::next(it));
        st.erase(it);
        st.emplace(lp, rp);
    };
    std::iota(id+1, id+1+n, 1);
    std::sort(id+1, id+1+n, [](int x, int y){ return r[x] < r[y]; });
    for (int i = 1, idx = 1; i <= n; i++) {
        for (; idx <= n && r[id[idx]] < i; idx++) ins(id[idx]);
        auto it = st.upper_bound({l[i],n});
        int lp = l[i];
        if (it != st.begin()) {
            --it;
            if (it->first <= l[i]) lp = it->second + 1;
            if (lp > i) lp = i;
        }
        f1[lp]++, f1[i]--;
    }
    std::sort(id+1, id+1+n, [](int x, int y){ return l[x] > l[y]; });
    st.clear();
    for (int i = n, idx = 1; i >= 1; i--) {
        for (; idx <= n && l[id[idx]] > i; idx++) ins(id[idx]);
        auto it = st.upper_bound({r[i],n});
        int rp = r[i];
        if (it != st.begin()) {
            --it;
            if (it->second >= r[i]) rp = it->first - 1;
            if (rp < i) rp = i;
        }
        f2[i]++, f2[rp]--;
    }
    int ans = 0;
    for (int i = 1; i < n; i++) {
        f1[i] += f1[i-1], f2[i] += f2[i-1];
        if (f1[i] && f2[i]) ans += a[i+1].p - a[i].p;
    }
    printf("%d\n", ans);
}