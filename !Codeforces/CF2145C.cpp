#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
char s[N];
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n >> s;
        int cnta = 0, cntb = 0;
        for (int i = 0; i < n; i++) {
            if (s[i] == 'a') ++cnta;
            else ++cntb;
        }
        int rest = cnta - cntb;
        if (rest == 0) { puts("0"); continue; }
        int sum = 0, ans = n;
        map<int,int> f;
        f[0] = -1;
        for (int i = 0; i < n; i++) {
            if (s[i] == 'a') ++sum;
            else --sum;
            if (f.count(sum - rest))
                ans = min(ans, i - f[sum - rest]);
            f[sum] = i;
        }
        if (ans == n) puts("-1");
        else printf("%d\n", ans);
    }
    return 0;
}