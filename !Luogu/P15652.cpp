#include <bits/stdc++.h>
void init(int, int) {}
int query(int, int);
std::vector<int> perm(int n) {
    std::set<int> st;
    std::vector<int> ans(n, -1), lv, rv;
    lv.reserve(n), rv.reserve(n);
    int p, last = n;
    for (int i = 0; i < n; i++) st.insert(st.end(), i);
    for (p = n-2; p >= 0; p--) {
        int res = query(0, p);
        if (last != res) {
            ans[p+1] = res;
            last = res;
            rv.push_back(res);
            st.erase(res);
            if (res == 0) break;
        }
    }
    last = n;
    for (int i = 1; i <= p + 1; i++) {
        int res = query(i, n-1);
        if (last != res) {
            ans[i-1] = res;
            last = res;
            lv.push_back(res);
            st.erase(res);
        }
    }
    int lp = 0, rp = n-1;
    auto it = st.rbegin();
    while (it != st.rend()) {
        while (lp <= p && ~ans[lp]) ++lp;
        while (rp > p && ~ans[rp]) --rp;
        if (lp <= p && ans[lp-1] > ans[rp+1] || rp <= p) 
            while (lp <= p && ans[lp] == -1) ans[lp++] = *it++;
        else
            while (rp > p && ans[rp] == -1) ans[rp--] = *it++;
    }
    return ans;
}
