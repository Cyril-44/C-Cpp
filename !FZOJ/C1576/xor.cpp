#include <cstdio>
#include <cstring>
#include <cstdint>
#include <set>
#include <cassert>
#include <vector>
#ifndef DEBUG
constexpr int N = 100000;
#define debug(...) ((void)0)
#else
constexpr int N = 100;
#define debug(...) fprintf(stderr, __VA_ARGS__)
#endif
uint64_t original_tree[N];
template<class T> inline void in(T &x) {
    char ch = getchar();
    while (ch < '0' || ch > '9') ch = getchar();
    for (x = 0; ch >= '0' && ch <= '9'; ch = getchar())
        x = (x * 10) + (ch ^ '0');
}
namespace TRIE {
constexpr int BITS = 60;
struct Node { int sum, ch[2]; } tr[N*60];
int bin[N*60], tot;
inline int alloc() {
    int u = bin[0] ? bin[bin[0]--] : ++tot;
    assert(tot < sizeof(tr) / sizeof(Node));
    tr[u] = { 0,{0,0} }; return u;
}
inline void free(int u) { bin[++bin[0]] = u; }
struct Trie {
    int root;
    std::multiset<int> allsums;
    void insert(uint64_t x) {
        debug("Insert %llu: ", x);
        int u = root;
        for (int i = 0; i < BITS; i++) {
            debug("%d(%d) ", u, tr[u].sum);
            fflush(stderr);
            bool s = x >> i & 1;
            if (!tr[u].ch[s]) tr[u].ch[s] = alloc();
            u = tr[u].ch[s];
            ++tr[u].sum;
            debug("%d(%d)\n", u, tr[u].sum);
        }
        if (tr[u].sum - 1) allsums.erase(allsums.find(tr[u].sum - 1));
        allsums.insert(tr[u].sum);
    }
    void erase(uint64_t x) {
        debug("Erase %llu: ", x);
        int u = root;
        bool freeFlag = false;
        for (int i = 0, v; i < BITS; i++) {
            debug("%d ", u);
            fflush(stderr);
            bool s = x >> i & 1;
            v = tr[u].ch[s];
            assert(v);
            --tr[v].sum;
            if (freeFlag) free(v);
            else if (tr[v].sum == 0) {
                tr[u].ch[s] = 0;
                free(v), freeFlag = true;
            }
            u = v;
        }
        debug("%d\n", u);
        allsums.erase(allsums.find(tr[u].sum + 1));
        if (tr[u].sum) allsums.insert(tr[u].sum);
    }
    int operator()() const { return *allsums.rbegin(); }
} trie[BITS];
inline void init() {
    for (int i = 0; i < BITS; i++) {
        trie[i].root = i, tr[i] = {};
        std::multiset<int>().swap(trie[i].allsums);
    }
    tot = BITS; bin[0] = 0;
}
} using TRIE::trie;
int main() {
    int T; in(T);
    while (T--) {
        TRIE::init();
        int n, m; uint64_t k;
        in(n), in(m), in(k);
        std::vector<uint64_t> mask({k});
        /* while (k) {
            mask.push_back(k-1);
            k &= k-1;
        } */
        for (int i = 1; i <= n; i++) {
            in(original_tree[i]);
            for (int j = 0; j < (int)mask.size(); j++)
                trie[j].insert(original_tree[i] & mask[j]);
        }
        for (int x, y; m--;) {
            in(x), in(y);
            int ans = 0;
            for (int j = 0; j < (int)mask.size(); j++) {
                trie[j].erase(original_tree[x] & mask[j]);
                trie[j].insert(y & mask[j]);
                ans = std::max(ans, trie[j]());
            }
            original_tree[x] = y;
            printf("%d\n", ans);
        }
    }
    return 0;
}