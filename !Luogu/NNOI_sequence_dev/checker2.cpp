#include <bits/stdc++.h>
#include "testlib.h"
constexpr int LEN = 7.68e6, SLEN = 9.6e3;
int main(int argc, char** argv) {
    registerInteraction(argc, argv);
    int n = inf.readInt();
    std::cout << n << std::endl;
    std::vector<long long> a(n);
    for (int j = 0; j < n; j++) {
        a[j] = inf.readLong();
        std::cout << a[j] << ' ';
    }
    std::cout << std::endl;

    std::string alice = ouf.readString();
    std::vector<long long> bob;
    for (int cnt = ouf.readInt(); cnt--; )
        ouf.readSpace(), bob.emplace_back(ouf.readLong());
    ouf.readEoln();

    if (alice.length() > LEN)
        quitf(_wa, "Wrong answer! (Alice's string was too long)");
    std::string aliceCompressed;
    char pre = alice[0];
    for (char c : alice) {
        if (c != '0' && c != '1') quitf(_wa, "Wrong answer! (Alice's string was invalid, which ASCII_%d occured)", (int)(c));
        if (pre != c) aliceCompressed += pre, pre = c;
    }
    aliceCompressed += pre;
    if (aliceCompressed.length() > SLEN)
        quitf(_wa, "Wrong answer! (Alice's string after compressing was too long)");
    if (bob != a)
        quitf(_wa, "Wrong answer! (The result Bob returned was incorrect)");
    quitf(_ok, "Accepted.");
    return 0;
}