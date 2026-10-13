#include <bits/stdc++.h>
constexpr int N = 11;
constexpr uint64_t B = 131;
int v[N], a[N];
int main() {
    freopen("film.in", "r", stdin);
    freopen("film.out", "w", stdout);
    int n, m;
    scanf("%d%d", &n, &m);
    for (int i = 1; i < n; i++) scanf("%*d%*d");
    for (int i = 0; i <= m; i++) scanf("%d", &v[i]);
    std::vector<std::pair<int,int>> revs;
    for (int i = 0; i <= m; i++)
        for (int j = i+4; j <= m; j++)
            if (v[i] == v[j]) revs.emplace_back(i, j);
    int k = revs.size();
    std::unordered_set<uint64_t> st;
    for (unsigned s = 0; s < (1u << k); s++) {
        memcpy(a, v, sizeof v);
        for (int j = 0; j < k; j++)
            if (s >> j & 1) std::reverse(a + revs[j].first, a + revs[j].second + 1);
        bool lt = false;
        for (int i = 0; i <= m; i++) {
            if (a[i] > v[i]) break;
            if (a[i] < v[i]) { lt = true; break; }
        }
        if (lt) {
            uint64_t hash = 0;
            for (int i = 0; i <= m; i++)
                hash = hash * B + a[i];
            st.emplace(hash);
        }
    }
    printf("%d\n", (int)st.size());
}