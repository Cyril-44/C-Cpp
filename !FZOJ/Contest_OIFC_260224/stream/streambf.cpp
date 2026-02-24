#include <bits/stdc++.h>
constexpr int N = 15;
int n;
using ULL = unsigned long long;
std::pair<int,ULL> ans;
void dfs(int i, ULL st, int cnt) {
    if (n - i + cnt < ans.first) return;
    if (i > n) ans = std::max(ans, {cnt, st});
    else {
        if ((i%2 || st >> i/2 & 1 ^ 1) && (i%3 || st >> i/3 & 1 ^ 1))
            dfs(i+1, st | (ULL)1 << i, cnt+1);
        dfs(i+1, st, cnt);
    }
}
std::string reverse(std::string s) { std::reverse(s.begin(), s.end()); return s; }
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        ans = {0,0};
        dfs(1, 0, 0);
        printf("%d %s\n", ans.first, reverse(std::bitset<100>(ans.second).to_string()).c_str());
        if (n % 10 == 0 || n > 50) fputc('.', stderr);
    }
    return 0;
}