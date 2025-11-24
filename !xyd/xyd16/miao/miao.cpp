#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <vector>
constexpr int N = 2000005;
int a[N];
inline void in(int &x) {
    char ch = getchar();
    while ((ch ^ '1') && (ch ^ '2')) ch = getchar();
    x = ch & 3;
}
int nearl[N], nearr[N];
inline void solveSingle() {
    int n;
    scanf("%d", &n);
    a[n+1] = 0;
    for (int i = 1; i <= n; i++)
        in(a[i]);
    std::vector<bool> rg;
    std::vector<int> pos0;
    rg.reserve(n);
    for (int i = 1, last = a[1], cnt = 1; i <= n+1; i++) {
        if (a[i] != last) {
            if (cnt == 1) rg.emplace_back(0);
            else rg.emplace_back(1), pos0.emplace_back(i);
            last = a[i], cnt = 1;
        } else ++cnt;
    }
    memset(nearl, 0x3f, sizeof(int) * (n + 4));
    for (int i = 0; i < (int)rg.size(); i++) {
        if (rg[i]) nearl[i] = i;
        else nearl[i] = i ? nearl[i-1] : -1;
    }
    nearr[rg.size()] = rg.size();
    for (int i = (int)rg.size() - 1; i >= 0; i--) {
        if (rg[i]) nearr[i] = i;
        else nearr[i] = nearr[i+1];
    }
    if (rg.size() & 1) {
        int mid = rg.size() >> 1;
        int l = nearl[mid], r = nearr[mid];
        if (r-l-1 <= n-r+l-1) {
            
        }
    }
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) solveSingle();
    return 0;
}
