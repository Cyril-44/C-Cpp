#include <bits/stdc++.h>
using namespace std;
constexpr int N = 100005;
struct Coin {
    int t, x;
    inline bool operator< (const Coin &rhs) const {
        return x < rhs.x || x == rhs.x && t < rhs.t;
    }
} a[N];
struct Unique {
    int a[N];
    template<class T, class Func>
    inline Unique(int n, T* arr, Func grab) {
        for (int i = 1; i <= n; i++)
            a[i] = grab(arr[i]);
        sort(a+1, a+1 + n);
        a[0] = unique(a+1, a+1+n) - a;
    }
    inline int operator()(const int x) const {
        return lower_bound(a+1, a+a[0], x) - a;
    }
};
struct BIT {
    int tr[N], n;
    inline BIT(int n) : n(n) { memset(tr, 0, sizeof(int) * (n+1)); }
    inline void upd(int p, const int x) {
        for (; p <= n; p += p & -p) tr[p] = max(tr[p], x);
    }
    inline int query(int p) const {
        int res = 0;
        for (; p > 0; p -= p & -p) res = max(res, tr[p]);
        return res;
    }
};
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int n;
    cin >> n;
    for (int i = 1; i <= n; i++)
        cin >> a[i].x >> a[i].t;
    Unique uni(n, a, [](const Coin&x){return x.t - x.x;});
    sort(a+1, a+1 + n);
    BIT f(n);
    for (int i = 1; i <= n; i++) {
        if (a[i].t < a[i].x) continue;
        int p = uni(a[i].t - a[i].x);
        f.upd(p, f.query(p) + 1);
    }
    printf("%d\n", f.query(n));
    return 0;
}
/*
6
1 5
2 2 
3 3 
4 3
5 6
4 7
*/