#include <cstdio>
#include <algorithm>
constexpr int N = 100005;
int a[N];
std::pair<int,int> b[N];
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++) {
            scanf("%d", &a[i]);
            b[i] = {a[i] - i, i};
        }
        std::sort(b+1, b+1+n);
        bool noSol = false;
        for (int i = 2; i <= n; i++)
            if (b[i].first == b[i-1].first)
                { noSol = true; break; }
        if (noSol) { puts("-1"); continue; }
        
    }
    return 0;
}