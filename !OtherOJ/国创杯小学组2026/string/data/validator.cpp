#include <bits/stdc++.h>
#include "testlib.h"
constexpr int MapToSubtask[] {1,2,2,3,3,3,3,4,4,4,4,4,5,5,5,6,6,6,6,6};
#define Checker(T, N, SN, CustomCheck) []() {                                       \
    int t = inf.readInt(1, (int)T, "Test cases"); inf.readEoln();                   \
    int sn = 0;                                                                     \
    while (t--) {                                                                   \
        std::string s = inf.readWord("[a-z]{1," + std::to_string((int)N) + "}");    \
        inf.readEoln();                                                             \
        sn += s.length();                                                           \
        CustomCheck;                                                                \
    }                                                                               \
    inf.readEof();                                                                  \
    ensuref(sn <= (int)SN, "Sum of n (%d) Range exceeded.", sn);                    \
}
#define CheckA for (char c : s) ensuref(c == 'a', "On Spec. A, char could only be 'a'.")
#define CheckB for (size_t i = 1; i < s.length(); i++) ensuref(s[i] != s[i-1], "On Spec. B, adjacent chars (tc %d, pos %zu) should be distinct.", t, i)
void (*val[])() {
    Checker(100, 5e6, 1e7, CheckA),
    Checker(100, 10,  20,  ),
    Checker(100, 200, 500, ),
    Checker(100, 5e3, 5e4, ),
    Checker(100, 5e6, 1e7, CheckB),
    Checker(100, 5e6, 1e7, ),
};
int main(int argc, char **argv) {
    registerValidation(argc, argv);
    int C = inf.readInt(1, 20, "Test ID"); inf.readSpace();
    val[MapToSubtask[C-1]-1]();
    return 0;
}
