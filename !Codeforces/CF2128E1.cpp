#include <cstdio>
#include <algorithm>
#include <set>
const int N = 300005;
int a[N];
std::multiset<int> s;
std::multiset<int>::iterator mid;

void insert(int x) {
    s.insert(x);
    if (s.size() == 1) {
        mid = s.begin();
    } else if (x < *mid) {
         mid--;
    } else {
         mid++;
    }
}

void erase(int x) {
    if (s.empty()) return;

    if (x <= *mid) {
         mid--;
         if (x == *mid) {
            auto it = s.find(x);
            if (it == mid) mid = s.erase(it);
            else s.erase(it);
            return;
        }
    } else {
         mid++;
    }

    s.erase(s.find(x));
}
int main() {
    int t, n, k;
    scanf("%d", &t);
    while (t--) {
        scanf("%d%d", &n, &k);
        for (int i = 1; i <= n; i++) scanf("%d", a + i);
        // std::multiset<int> st;
        s.clear();
        int ans = (k==1) ? a[1] : 0, L(1), R(1), last = a[1];
        s.insert(a[1]);
        mid = s.begin();
        // auto mid = st.begin();
        for (int l(1), r(2); r <= n; ++r) {
            while ((r-l+1) > k && !s.empty() && a[l] < a[r]) {
                // auto it = st.find(a[l++]);
                // if (it == mid) {
                //     mid = st.erase(it);
                //     if (st.size() & 1) --mid;
                // }
                // else {
                //     if (*it < *mid) { if(st.size() & 1) ++mid; }
                //     else { if (st.size() & 1 ^ 1) --mid; }
                //     st.erase(it);
                // }
                erase(a[l++]);
            }
            // st.insert(a[r]);
            // if (st.size() == 1) mid = st.begin();
            // else if (a[r] < *mid) { if (st.size() & 1 ^ 1) --mid; }
            // else { if (st.size() & 1 ^ 1) ++mid; }
            insert(a[r]);
            if ((r-l+1) >= k && *mid > ans) {
                ans = *mid;
                L = l, R = r;
            }
        }
        printf("%d %d %d\n", ans, L, R);
    }
    return 0;
}