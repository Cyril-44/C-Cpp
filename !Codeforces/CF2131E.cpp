#include <cstdio>
#include <cstring>
#include <algorithm>
#include <map>
constexpr int N = 200005;
int a[N], b[N];
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        int n;
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) scanf("%d", a+i);
        for (int i = 1; i <= n; i++) scanf("%d", b+i);
        bool flg = true;
        int xora = 0;
        a[n+1] = 0;
        for (int i = n; i >= 1; i--) {
            xora ^= a[i];
            if (xora != b[i]) xora = a[i];
            if (xora != b[i]) xora = a[i+1] ^ a[i];
            if (xora != b[i]) { flg = false; break; }
        }
        /* std::map<int, int> mp;
        mp[0] = n + 1;
        int fur = n + 1;
        for (int i = n; i >= 1; i--) {
            if (!mp.count(b[i] ^ a[i]) || mp[b[i] ^ a[i]] > fur) {
                flg = false; break;
            }
            mp[a[i]] = i;
            fur = std::min(fur, mp[b[i] ^ a[i]]);
        } */
        puts(flg ? "YES" : "NO");
    }
    return 0;
}