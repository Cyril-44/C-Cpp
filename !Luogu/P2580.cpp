#include <stdio.h>
#include <map>
const int S = 55;
struct Trie {
    std::map<char, Trie*> son;
    bool flg1, flg2;
    inline Trie(): flg1(false), flg2(false), son() {}
};
int main() {
    int n, m;
    scanf("%d", &n);
    Trie* rt = new Trie;
    char s[S];
    while (n--) {
        scanf("%s", s);
        Trie* cur = rt;
        for (int i = 0; s[i]; i++) {
            if (!cur->son.count(s[i]))
                cur->son[s[i]] = new Trie;
            cur = cur->son[s[i]];
        }
        cur->flg1 = true;
    }
    scanf("%d", &m);
    while (m--) {
        scanf("%s", s);
        Trie* cur = rt;
        bool flg = true;
        for (int i = 0; flg && s[i]; i++) {
            if (!cur->son.count(s[i])) flg = false;
            else cur = cur->son[s[i]];
        }
        if (!flg || !cur->flg1) puts("WRONG");
        else if (cur->flg2) puts("REPEAT");
        else puts("OK"), cur->flg2 = true;
    }
    return 0;
}