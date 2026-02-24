#include <stdio.h>
#include <bitset>
#include <algorithm>
constexpr int M = (int)1e6 + 5;
int qry[M];
std::string reverse(std::string s) { std::reverse(s.begin(), s.end()); return s; }
struct BF {
    constexpr static int N = 105;
    std::bitset<N> f;
    inline int operator()(int n) {
        if (n <= 2) return 1;
        if (n == 3) return 2;
        for (int i = 1; i <= n; i++) f[i] = 1;
        f[1] = 0, f[3] = 0;
        for (int i = 2; i <= n; i++)
            if (f[i]) {
                if (i*2 <= n) f[i*2] = 0;
                if (i*3 <= n) f[i*3] = 0;
            }
        printf("%s ", reverse(f.to_string()).c_str());
        return f.count();
    }
};
int main() {
    freopen("stream.in", "r", stdin);
    freopen("stream.out", "w", stdout);
    scanf("%d", &qry[0]);
    for (int i = 1; i <= qry[0]; i++)
        scanf("%d", &qry[i]);
    for (int i = 1; i <= qry[0]; i++)
        printf("%d\n", std::max(BF{}(qry[i]-1), BF{}(qry[i])));
    return 0;
}