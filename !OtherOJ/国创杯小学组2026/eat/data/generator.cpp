#include <bits/stdc++.h>
#include "testlib.h"
#define Generator(N, Qr, w, Agent) [](const std::string& path, const int Tid) { \
    std::ofstream file(path); \
    file << Tid << '\n'; \
    const int n = N; \
    for (int i = 1; i <= n; i++) a[i] = rnd.next(1, (int)1e9); \
    Agent; \
    int l = 1, r = n; \
    std::vector<std::pair<int, int64_t>> ques; ques.reserve(n); \
    while (l <= r) { \
        int p = rnd.next(0, r - l), qr = std::min((int)Qr, r - l + 2 >> 1), ans = rnd.wnext(1, qr, w), k; \
        set<int64_t> st; \
        bool lst = true, rst = true, same = false; \
        int64_t lval, rval; \
        auto calc = [&](int pos) { return 1ll * a[pos] * a[l + (pos-l+p) % (r-l+1)]; }; \
        for (k = 1; (lst || rst) && k <= ans; k++) { \
            lval = 1ll * a[l + k-1] * a[l + (k-1 + p) % (r-l+1)]; \
            rval = 1ll * a[r - (k-1)] * a[l + (r-l+1 - (k-1) + p)) % (r-l+1)]; \
            lst &= st.insert(lval).second; \
            rst &= st.insert(rval).second; \
            if (lval == rval) { same = true; break; } \
        } \
        if (same) { if (!lst && !rst) --k; l += k, r -= k; ques.emplace_back(p, lval); } \
        else if (lst && rst) ques.emplace_back(p, rnd.next(2) ? lval : rval); \
        else if (lst) ques.emplace_back(p, rnd.next()) \
    } \
}
void (*testGen[])(const std::string&, const int) {

};
int main(int argc, char **argv) {
    registerGen(argc, argv, 1);

}