#include <bits/stdc++.h>
constexpr int MOD = 998244353;
int main() {
    int mx = 0;
    for (int i = 2; i < MOD; i++) {
        std::unordered_set<int> st;
        for (int x = i;; x = 1ull * x * x % MOD)
            if (!st.insert(x).second) break;
        mx = std::max(mx, (int)st.size());
        if ((i & 0xffff) == 0) printf("%d: %d curmx = %d\n", i, (int)st.size(), mx);
    }
}