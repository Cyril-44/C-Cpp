#include <stdio.h>
#include <vector>
constexpr int N = 1000005;
char s[N];
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
namespace ACM {
int tot = 0;
#define FAIL -1
inline void insert(char *s, int id) {
    int u = 0;
    for (; *s; ++s) {
        if (!ac[u][*s - 'a']) ac[u][*s - 'a'] = ++tot;
        u = ac[u][*s - 'a'];
    }
    ac[u].occurrence.push_back(id);
}
static int q[N];
inline void buildAC() {
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
bool f[N], ans[N];
inline void dfs(int u) {
    for (int i = head[u]; i; i = e[i].nxt) {
        dfs(e[i].to);
        f[u] |= f[e[i].to];
    }
    for (int j : ac[u].occurrence)
        ans[j] |= f[u];
}
inline void solve(char *s, int n) {
    int u = 0;
    for (; *s; ++s) {
        u = ac[u][*s - 'a'];
        f[u] = true;
    }
    for (int i = 0; i < 26; i++)
        if (ac[0][i]) dfs(ac[0][i]);
    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        cnt += ans[i]; 
    }
    printf("%d\n", cnt);
}
#undef FAIL
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%s", s);
        ACM::insert(s, i);
    }
    ACM::buildAC();
    scanf("%s", s);
    ACM::solve(s, n);
    return 0;
}