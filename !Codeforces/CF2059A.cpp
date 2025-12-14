#include <stdio.h>
#include <set>
const int N = 55;
int a[N], b[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            scanf("%d", &a[i]);
        for (int i = 0; i < n; i++)
            scanf("%d", &b[i]);
        int cnt = 0;
        std::set<int> st;
        for (int i = 0; i < n; i++) {
            if (!st.count(a[i])) {
                st.insert(a[i]);
                ++cnt;
            }
        }
        st.clear();
        for (int i = 0; i < n; i++) {
            if (!st.count(b[i])) {
                st.insert(b[i]);
                ++cnt;
            }
        }
        if (cnt > 3) puts("YES");
        else puts("NO");
    }
    return 0;
}