#include <cstdio>
#include <cstring>
#include <algorithm>
#include <vector>
#include <map>
constexpr int N = 300005;
int a[N], b[N];
struct BruteForce {
    constexpr static int N = 5005;
    int c[N], d[N];
    inline void operator()(int n, int m) {
        const size_t Sz = sizeof(int) * m;
        int ans = 0;
        for (int i = 1, iR = n - m + 1; i <= iR; i++)  {
            memcpy(c+1, a+i, Sz);
            memcpy(d+1, c+1, Sz);
            std::sort(c+1, c+m+1);
            for (int j = 1; j <= m; j++)
                d[j] = std::lower_bound(c+1, c+1+m, d[j]) - c;
            ans += !memcmp(b+1, d+1, Sz);
        }
        printf("%d\n", ans);
    }

};
inline void callMle10(int n, int m) {
    static bool initialized[15]{};
    static unsigned fact[15];
    static int c[15];
    static std::map<std::vector<int>, int> ans[11];
    if (!initialized[m]) {
        const size_t Sz = sizeof(int) * m;
        initialized[m] = true;
        fact[0] = 1;
        for (int i = 1; i <= m; i++) fact[i] = fact[i-1] * i;
        for (int i = 1, iR = n - m + 1; i <= iR; i++)  {
            memcpy(c, a+i, Sz);
            std::vector<int> d(c, c+m);
            std::sort(c, c+m);
            for (int j = 0; j < m; j++)
                d[j] = std::lower_bound(c, c+m, d[j]) - c;
            ans[m][d]++;
        }
    }
    std::vector<int> vec(m);
    for (int i = 1; i <= m; i++)
        vec[i-1] = b[i] - 1;
    printf("%d\n", ans[m][vec]);
}
int main() {
    int n, T, m;
    scanf("%d", &n);
    bool aIsInc = true;
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]), aIsInc &= a[i-1] < a[i];
    static int af[N];
    for (int i = 1, cnt = 0; i <= n+1; i++)
        (a[i-1] < a[i]) ? (++cnt) : (++af[cnt], cnt = 1);
    for (int i = n-1; i >= 1; i--) af[i] += af[i+1];
    for (int i = n-1; i >= 1; i--) af[i] += af[i+1];
    scanf("%d", &T);
    const bool Tis1 = T == 1;
    while (T--) {
        scanf("%d", &m);
        bool bIsInc = true;
        for (int i = 1; i <= m; i++)
            scanf("%d", &b[i]), bIsInc &= b[i-1] < b[i];
        if (aIsInc) printf("%d\n", bIsInc ? std::max(n - m + 1, 0) : 0);
        else if (bIsInc) printf("%d\n", af[m]); 
        else if (n <= 5000) BruteForce{}(n, m);
        else if (m <= 10) callMle10(n, m);
        else {
            
        }
    }
    return 0;
}
