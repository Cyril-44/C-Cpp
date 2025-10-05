#include <cstdio>
#include <cstring>
#include <algorithm>
using std::max;
constexpr int N = 105;
int a[N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            scanf("%d", &a[i]);
        bool flg = true;
        for (int i = 3; i <= n; i++) {
            int cnt_1 = (a[i-2] == -1) + (a[i-1] == -1) + (a[i] == -1);
            if (cnt_1 == 3) continue;
            if (cnt_1 == 2) {
                int tp = ~a[i-2] ? a[i-2] : ~a[i-1] ? a[i-1] : a[i];
                if (!tp) { flg = false; break; }
                a[i-2] = a[i-1] = a[i] = tp;
            } else if (cnt_1 == 1) {
                int tp1 = ~a[i-2] ? a[i-2] : a[i-1];
                int tp2 = ~a[i] ? a[i] : a[i-1];
                if (tp1 != tp2 || !tp1) { flg = false; break; }
                a[i-2] = a[i-1] = a[i] = tp1;
            } else {
                if (!a[i-2] || !a[i-1] || !a[i] || a[i-2] != a[i-1] || a[i-1] != a[i] || a[i-2] != a[i])
                    { flg = false; break; }
            }
        }
        puts(flg ? "YES" : "NO");
    }
    return 0;
}