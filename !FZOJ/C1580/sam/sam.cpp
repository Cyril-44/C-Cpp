#include <cstdio>
#include <string>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <algorithm>
constexpr int N = 100005;
constexpr unsigned MOD = ~0u;
std::string s[N];
int n;
class Trie {
    struct Node { Node* ch[26]{}; int cnt = 0; } *root;
    void merge(Node*& u, Node* v) {
        if (!v) return;
        if (!u) { u = v; return; }
        sum += 1ull * u->cnt * v->cnt;
        u->cnt += v->cnt;
        for (int i = 0; i < 26; i++)
            merge(u->ch[i], v->ch[i]);
        delete v;
    }
    void clean(Node *u) {
        if (!u) return;
        for (Node* ptr : u->ch) clean(ptr);
        delete u;
    }
public:
    Trie() : root(new Node) {}
    ~Trie() { clean(root); }
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    void clear() { clean(root); root = new Node; sum = 0; }
    uint64_t sum = 0;
    void insert(const std::string &s) {
        Node *u = root;
        for (char c : s) {
            if (!u->ch[c-'a']) u->ch[c-'a'] = new Node;
            u = u->ch[c-'a'], sum += u->cnt++;
        }
    }
    void join(Trie& trie) {
        merge(root, trie.root);
        trie.root = nullptr;
        trie.clear();
    }
};
struct Node {
    int ch[26]; Trie trie;
    void clear() { memset(ch, 0, sizeof ch); trie.clear(); }
} tr[N];
int tot;
inline void clear() { tr[tot=0].clear(); }
inline int alloc() { tr[++tot].clear(); return tot; }
inline void insert(std::string s) {
    int u = 0;
    for (char c : s) {
        if (!tr[u].ch[c-'a']) tr[u].ch[c-'a'] = alloc();
        u = tr[u].ch[c-'a'];
    }
    std::reverse(s.begin(), s.end());
    tr[u].trie.insert(s);
}
uint64_t ans;
void dfs(int u = 0, int dep = 0) {
    for (int i = 0; i < 26; i++)
        if (tr[u].ch[i]) {
            dfs(tr[u].ch[i], dep + 1);
            tr[u].trie.join(tr[tr[u].ch[i]].trie);
        }
    ans += dep * tr[u].trie.sum;
}
/*经典Trick：考虑将后缀 Trie 按照前缀 Trie 顺序 Trie树合并，统计答案。
*/
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    int T;
    std::cin >> T;
    while (T--) {
        std::cin >> n;
        clear();
        for (int i = 1; i <= n; i++) {
            std::cin >> s[i];
            insert(s[i]);
        }
        ans = 0;
        dfs();
        printf("%u\n", unsigned(ans % MOD));
    }
    return 0;
}
