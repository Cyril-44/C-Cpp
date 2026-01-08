#include <stdio.h>
#include <vector>
#include <cstring>
#include <algorithm>
constexpr int N1 = 155, N2 = 75, N = N1 * N2, M = 1000005;
char s[N1][N2], t[M];
struct Edge { int to, nxt; } e[N];
int head[N], edgtot;
inline void addedg(const int &fr, const int &to) {
    e[++edgtot] = {to, head[fr]};
    head[fr] = edgtot;
}
struct Node {
    int ch[26];
    int fail;
    std::vector<int> occurrence;
    Node& operator()(int x);
    inline int& operator[](int x) { return ~x ? ch[x] : fail; }
} ac[N];
inline Node& Node::operator()(int x) { return ac[(*this)[x]]; }
long long ans[N1];
namespace ACM {
int tot = 0;
long long f[N];
#define FAIL -1
inline int alloc() { ac[++tot] = {}; f[tot] = 0; return tot; }
inline void insert(char *s, int id) {
    int u = 0;
    for (; *s; ++s) {
        if (!ac[u][*s - 'a']) ac[u][*s - 'a'] = alloc();
        u = ac[u][*s - 'a'];
    }
    ac[u].occurrence.push_back(id);
}
inline void clear() { memset(head, 0, sizeof(int) * (tot+1)); edgtot = 0; ac[tot = 0] = {}; f[0] = 0; }
inline void buildAC() {
    static int q[M];
    int l = 0, r = 0;
    for (int i = 0; i < 26; ++i)
        if (ac[0][i])
            q[r++] = ac[0][i];
    while (l ^ r) {
        int u = q[l++];
        for (int i = 0; i < 26; ++i)
            if (ac[u][i]) {
                ac[u](i)[FAIL] = ac[u](FAIL)[i];
                addedg(ac[u](FAIL)[i], ac[u][i]);
                q[r++] = ac[u][i];
            }
            else ac[u][i] = ac[u](FAIL)[i];
    }
}
inline void dfs(int u) {
    for (int i = head[u]; i; i = e[i].nxt) {
        dfs(e[i].to);
        f[u] += f[e[i].to];
    }
    for (int j : ac[u].occurrence)
        ans[j] += f[u];
}
inline void solve(char *s, int n) {
    int u = 0;
    for (; *s; ++s) {
        u = ac[u][*s - 'a'];
        ++f[u];
    }
    memset(ans, 0, sizeof(long long) * (n + 1));
    for (int i = 0; i < 26; i++)
        if (ac[0][i]) dfs(ac[0][i]);
}
#undef FAIL
}
int main() {
    int n, i;
    long long mx;
start:
    scanf("%d", &n);
    if (n == 0) goto end;
    ACM::clear();
    for (i = 1; i <= n; i++) {
        scanf("%s", s[i]);
        ACM::insert(s[i], i);
    }
    ACM::buildAC();
    scanf("%s", t);
    ACM::solve(t, n);
    mx = *std::max_element(ans+1, ans+1 + n);
    printf("%lld\n", mx);
    for (i = 1; i <= n; i++)
        if (ans[i] == mx)
            puts(s[i]);
    goto start;
end:
    return 0;
}