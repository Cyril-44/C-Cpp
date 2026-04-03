#include "testlib.h"
#include <bits/stdc++.h>
inline std::string genPal(int len, int incProb, int contProb, char startChar) {
    std::string res; res.resize(len);
    char ch = rnd.wnext(startChar, 'z', -10);
    for (int i = 0, cur; i < (len+1) / 2; i++) {
        if (ch < 'z' && (cur = rnd.next(0, 100)) <= incProb) ++ch;
        if (ch < 'z' && cur > contProb) {i--; continue;}
        res[i] = ch;
    }
    std::reverse_copy(res.begin(), res.begin() + len / 2, res.begin() + (len + 1) / 2);
    return res;
}
inline std::string genHack(int len, int incProb, int contProb) {
    assert(len >= 6); // aababa
    char ch = rnd.wnext('a', 'y', -26);
    int l1 = rnd.wnext(2, len - 4, -26);
    int l2 = rnd.wnext(1, (len - l1) / 2 - 1, -26);
    int l3 = rnd.next(1, len - l1 - 2 * l2 - 1);
    int l4 = len - l1 - 2 * l2 - l3;
    // [l1](l3)[l2](l4)[l2]
    return std::string(l1, ch)
         + genPal(l3, incProb, contProb, ch + 1)
         + std::string(l2, ch)
         + genPal(l4, incProb, contProb, ch + 1)
         + std::string(l2, ch);
}
inline std::string genRand(int len) {
    std::string res; res.resize(len);
    for (int i = 0; i < len; i++) res[i] = rnd.next('a', 'z');
    return res;
}

int main(int argc, char **argv) {
    registerGen(argc, argv, 1);
    
    return 0;
}