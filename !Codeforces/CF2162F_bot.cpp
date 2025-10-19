#include <bits/stdc++.h>
using namespace std;
using i64_t = long long;
using u64_t = unsigned long long;
using pii_t = pair<int, int>;
#ifndef ONLINE_JUDGE
# pragma GCC optimize(2)
#endif
#pragma GCC target("tune=native")
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(i, c) for (int tempFor_##c = c; tempFor_##c; --tempFor_##c)
#define YES return cout << "YES\n", void()
#define NO return cout << "NO\n", void()
#define YESNO(j) cout << ((j) ? "YES\n" : "NO\n")
Inline int Popcnt(int x) { return __builtin_popcount((unsigned)x); }
Inline int Popcnt(unsigned x) { return __builtin_popcount(x); }
Inline int Popcnt(i64_t x) { return __builtin_popcountll((u64_t)x); }
Inline int Popcnt(u64_t x) { return __builtin_popcountll(x); }
Inline int Log2(int x) { return 31 - __builtin_clz((unsigned)x | 1); }
Inline int Log2(unsigned x) { return 31 - __builtin_clz(x | 1); }
Inline int Log2(i64_t x) { return 63 - __builtin_clzll((u64_t)x | 1); }
Inline int Log2(u64_t x) { return 63 - __builtin_clzll(x | 1); }

constexpr int N = 3005;
using Range = pair<int, int>;
#define L first
#define R second
Range a[N];
inline void solveSingleTestCase() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; ++i) {
        int l, r;
        cin >> l >> r;
        a[i] = {l, r};
    }

    auto func = [&](int mn, int mx) -> pair<int,int> {
        int L = 1, R = n;
        bool any = false;
        for (auto &pr : a) {
            int l = pr.first, r = pr.second;
            if (l <= mn && r >= mx) {
                if (!any) { L = l; R = r; any = true; }
                else { L = max(L, l); R = min(R, r); }
            }
        }
        if (!any || L > R) return {1, 0};
        return {L, R};
    };

    bool found = false;
    vector<int> ans(n + 1, -1);
    int best_s = INT_MAX;

    For (l, 1, n) {
        set<int> arr;
        For (i, 1, n) arr.insert(i);

        vector<int> pos; pos.reserve(n+1);
        pos.push_back(l);
        arr.erase(l);
        int mn = l, mx = l;

        bool flg = false;
        for (int step = 1; step <= n; ++step) {
            auto I = func(mn, mx);
            int L = I.first, R = I.second;
            bool insflg = false;
            int ins = -1;
            if (L <= R) {
                auto it = arr.lower_bound(L);
                if (it != arr.end() && *it <= R) {
                    insflg = true;
                    ins = *it;
                }
            }
            if (insflg) {
                pos.push_back(ins);
                arr.erase(ins);
                mn = min(mn, ins);
                mx = max(mx, ins);
                flg = true;
                break;
            }
            bool placed = false;
            if (L > 1) {
                auto it = arr.lower_bound(L);
                if (it != arr.begin()) {
                    auto it2 = it; --it2;
                    int x = *it2;
                    pos.push_back(x);
                    arr.erase(x);
                    mn = min(mn, x);
                    mx = max(mx, x);
                    placed = true;
                }
            }
            if (!placed) {
                if (R < n) {
                    auto it = arr.upper_bound(R);
                    if (it != arr.end()) {
                        auto it2 = arr.end(); --it2;
                        if (*it2 > R) {
                            int x = *it2;
                            pos.push_back(x);
                            arr.erase(x);
                            mn = min(mn, x);
                            mx = max(mx, x);
                            placed = true;
                        }
                    }
                }
            }
            if (!placed) {
                bool placed2 = false;
                if (mn <= mx) {
                    if (mn <= L-1) {
                        auto it = arr.lower_bound(mn);
                        if (it != arr.end() && *it <= L-1) {
                            int x = *it;
                            pos.push_back(x);
                            arr.erase(x);
                            mn = min(mn, x);
                            mx = max(mx, x);
                            placed2 = true;
                        }
                    }
                    if (!placed2 && R+1 <= mx) {
                        auto it = arr.lower_bound(R+1);
                        if (it != arr.end() && *it <= mx) {
                            int x = *it;
                            pos.push_back(x);
                            arr.erase(x);
                            mn = min(mn, x);
                            mx = max(mx, x);
                            placed2 = true;
                        }
                    }
                }
                if (placed2) placed = true;
            }
            if (!placed) {
                flg = false;
                break;
            }
        }

        if (flg) {
            int s = (int)pos.size() - 1;
            if (s < best_s) {
                best_s = s;
                vector<int> p(n+1, -1);
                for (int v = 0; v <= s; ++v) {
                    int pp = pos[v];
                    p[pp] = v;
                }
                int curVal = s + 1;
                for (int pp = 1; pp <= n; ++pp) {
                    if (p[pp] == -1) {
                        p[pp] = curVal++;
                    }
                }
                ans = p;
                found = true;
                if (best_s == 0) break;
            }
        }
    }

    if (!found) {
        throw;
    } else {
        for (int i = 1; i <= n; ++i) {
            cout << ans[i] << ' ';
        }
        cout << '\n';
    }
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases;
    for (cin >> testCases; testCases; --testCases)
        solveSingleTestCase();
    return 0;
}
