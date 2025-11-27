#include <stdio.h>
#include <map>
const int N = 100005;
int a[N], f[N];
long long s[N];
inline int max(const int &x, const int &y) {
    return x > y ? x : y;
}
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        std::map<long long, int> mp;
        mp[0] = 0;
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            s[i] = s[i-1] + a[i];
            f[i] = f[i-1];
            if (mp.count(s[i]))
                f[i] = max(f[i], f[mp[s[i]]] + 1);
            mp[s[i]] = i;
        }
        printf("%d\n", f[n]);
    }
    return 0;
}