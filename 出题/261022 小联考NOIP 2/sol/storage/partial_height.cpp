#include <bits/stdc++.h>
using namespace std;
const int MOD = 998244353;
struct Value { long long cost; int ways; };
Value sum(Value a, Value b) {
    return {a.cost + b.cost, int(1LL * a.ways * b.ways % MOD)};
}
Value best(Value a, Value b) {
    if (a.cost != b.cost) return a.cost < b.cost ? a : b;
    return {a.cost, (a.ways + b.ways) % MOD};
}
struct Node {
    int l = 0, r = 0, first = 0, last = 0, count = 0;
    long long weight = 0;
    Value a{0,1}, b{0,1};
};
// Correct for every tree; O(n + q*h) time, O(n) memory, where h is tree height.
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int n,q;
    if (!(cin >> n >> q)) return 0;
    int N = 2*n-1;
    vector<Node> t(N+1);
    for (int v=1;v<=N;++v) cin >> t[v].l >> t[v].r >> t[v].weight;
    vector<int> order, stack{1}, touched;
    int leaf=0;
    while (!stack.empty()) {
        int v=stack.back(); stack.pop_back(); order.push_back(v);
        if (!t[v].l) t[v].first=t[v].last=++leaf;
        else { stack.push_back(t[v].r); stack.push_back(t[v].l); }
    }
    auto pull = [&](int v) {
        Node &u=t[v];
        if (!u.l) {
            u.b={u.weight,1};
            u.a=u.count ? u.b : Value{0,1};
        } else {
            Value x=sum(t[u.l].a,t[u.r].a);
            u.b=best({u.weight+x.cost,x.ways},sum(t[u.l].b,t[u.r].b));
            u.a=u.count ? u.b : x;
        }
    };
    for (auto it=order.rbegin();it!=order.rend();++it) {
        int v=*it;
        if (t[v].l) { t[v].first=t[t[v].l].first; t[v].last=t[t[v].r].last; }
        pull(v);
    }
    for (int i=0;i<q;++i) {
        char op; int l,r; cin >> op >> l >> r;
        stack={1}; touched.clear();
        while (!stack.empty()) {
            int v=stack.back();stack.pop_back();
            if (t[v].last<l || t[v].first>r) continue;
            touched.push_back(v);
            if (l<=t[v].first && t[v].last<=r) t[v].count+=(op=='+'?1:-1);
            else { stack.push_back(t[v].r); stack.push_back(t[v].l); }
        }
        for (auto it=touched.rbegin();it!=touched.rend();++it) pull(*it);
        cout << t[1].a.cost << ' ' << t[1].a.ways << '\n';
    }
}
