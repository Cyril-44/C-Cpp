#include <bits/stdc++.h>
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int Cid, T; std::cin >> Cid >> T;
    while (T--) {
        int n, m; int64_t k;
        std::cin >> n >> m >> k;
        std::map<int64_t, int> mp;
        for (int i = 1; i <= n; i++) { int64_t ai; std::cin >> ai; ++mp[ai]; }
        bool unable = false;
        int ans = 0;
        for (int i = 1; i <= m; i++) {
            int64_t b, c; std::cin >> b >> c;
            if (unable) continue;
            auto it = mp.lower_bound(b);
            while (c > 0) {
                if (it == mp.end()) { //combine
                    int64_t curr = k;
                    bool first = true;
                    while (curr < b) {
                        if (mp.empty()) break;
                        if (it == mp.end()) --it;
                        if (first) first = false;
                        else ++ans;
                        const int64_t a = it->first; int& x = it->second;
                        curr += a - k;
                        if (curr < 0) break;
                        if (x > 1) --x;
                        else it = mp.erase(it);
                    }
                    if (curr < b || first) { unable = true; break; }
                    mp[curr] = 1;
                    it = --mp.end();
                }
                const int64_t a = it->first; int& x = it->second;
                const int64_t one = a - b + 1;
                if (one * x <= c) {
                    c -= one * x;
                    if (b > 1) mp[b-1] += x;
                    it = mp.erase(it);
                } else {
                    if (c/one) {
                        x -= c/one;
                        if (b > 1) mp[b-1] += c/one;
                    }
                    if (c%one) {
                        x--;
                        if (a - c%one) ++mp[a - c%one];
                    }
                    if (!x) it = mp.erase(it);
                    c = 0;
                }
            }
            // for (const auto &[x, y] : mp) fprintf(stderr, "sz[%d] has %d\n", x, y);
            // fprintf(stderr, "currentans=%d\n", ans);
        }
        if (unable) ans = -1;
        std::cout << ans << '\n';
    }
}