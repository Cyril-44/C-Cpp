#include <stdio.h>
#include <map>
const int N = 2005;
char s[N];
struct Trie {
    std::map<char, Trie*> son;
    int flag;
    inline Trie(): flag(0), son() {}
};
int ans;
inline void dfs(Trie* u, int d) {
    if (u->son.empty()) ans = std::max(ans, d+1);
    else {
        for (const auto& i : u->son)
            dfs(i.second, d + u->flag);
    }
}
int main() {
    int n;
    scanf("%d", &n);
    Trie* rt = new Trie;
    while (n--) {
        scanf("%s", s);
        Trie* cur = rt;
        for (int i = 0; s[i]; i++) {
            if (!cur->son.count(s[i]))
                cur->son[s[i]] = new Trie;
            cur = cur->son[s[i]];
        }
        ++cur->flag;
    }
    dfs(rt, 0);
    printf("%d\n", ans);
    return 0;
}