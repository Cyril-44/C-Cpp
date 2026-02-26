#include <bits/stdc++.h>
char s[105];
int n;
std::string ans[105];
std::ofstream inf("memoryVal.in"), ouf("memoryVal.out");
inline void upd() {
    int k = 0;
    for (int i = 1; i < n; i++) k += s[i] != s[i-1];
    if (ans[k].empty()) ans[k] = std::string(s);
    else ans[k] = std::min(ans[k], std::string(s));
}
void solve(int i) {
    if (i >= n) upd();
    else if (s[i] == '?') {
        s[i] = '0'; solve(i+1);
        s[i] = '1'; solve(i+1);
        s[i] = '?';
    }
    else solve(i+1);
}
inline void gen() {
    for (int k = 0; k < n; k++) {
        inf << n << ' ' << k << '\n' << s << '\n';
        ans[k].clear();
    }
    solve(0);
    for (int k = 0; k < n; k++)
        if (ans[k].empty()) ouf << "-1\n";
        else ouf << ans[k] << '\n';
}
void dfs(int i) {
    if (i >= n) gen();
    else {
        s[i] = '0'; dfs(i+1);
        s[i] = '1'; dfs(i+1);
        s[i] = '?'; dfs(i+1);
    }
}
int main() {
    scanf("%d", &n);
    freopen("memoryVal.in", "w", stdout);
    inf << (int)std::pow(3, n) * n << '\n';
    dfs(0);
    return 0;
}