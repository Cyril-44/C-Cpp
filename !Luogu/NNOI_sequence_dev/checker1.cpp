#include <bits/stdc++.h>
#include "testlib.h"
constexpr int LEN = 1.5e6, SLEN = 2e3;
int main(int argc, char** argv) {
    registerInteraction(argc, argv);
    int t = inf.readInt();
    std::cout << t << std::endl;
    for (int i = 0; i < t; i++) {
        int n = inf.readInt();
        std::cout << n << std::endl;
        std::vector<int> a(n);
        for (int j = 0; j < n; j++) {
            a[j] = inf.readInt();
            std::cout << a[j] << ' ';
        }
        std::cout << std::endl;

        std::string alice = ouf.readString();
        std::vector<int> bob;
        for (int cnt = ouf.readInt(); cnt--; )
            ouf.readSpace(), bob.emplace_back(ouf.readInt());
        ouf.readEoln();

        if (alice.length() > LEN)
            quitf(_wa, "Wrong answer on test case %d! (Alice's string was too long)", i+1);
        std::string aliceCompressed;
        char pre = alice[0];
        for (char c : alice) {
            if (c != '0' && c != '1') quitf(_wa, "Wrong answer on test case %d! (Alice's string was invalid, which ASCII_%d occured)", i+1, (int)(c));
            if (pre != c) aliceCompressed += pre, pre = c;
        }
        aliceCompressed += pre;
        if (aliceCompressed.length() > SLEN)
            quitf(_wa, "Wrong answer on test case %d! (Alice's string after compressing was too long)", i+1);
        if (bob != a)
            quitf(_wa, "Wrong answer on test case %d! (The result Bob returned was incorrect)", i+1);
    }
    quitf(_ok, "Accepted.");
    return 0;
}