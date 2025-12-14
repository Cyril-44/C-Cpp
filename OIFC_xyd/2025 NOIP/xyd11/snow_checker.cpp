#include <bits/stdc++.h>
using namespace std;
int main() {
    ifstream out("snow.out"), ans("snow2.out");
    int exp, red;
    bool is40(true), is100(true);
    while ((out >> red) && (ans >> exp)) {
        if (red != exp) {
            is100 = false;
            if (red == -1 && exp != -1) { is40 = false; break; }
            if (red != -1 && exp == -1) { is40 = false; break; }
        }
    }
    if (is100) puts("Accepted!");
    else if (is40) puts("Partically Accepted (40pts)!");
    else puts("Wrong Answer!");
    return 0;
}