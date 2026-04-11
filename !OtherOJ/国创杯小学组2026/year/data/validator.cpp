#include <bits/stdc++.h>
#include "testlib.h"
constexpr int MapToSubTask[]{1,1,1,1,2,2,2,2,3,3,4,4,4,4,5,5,5,5,5,5};
#define Checker(T, M, N, SN, ABC) []() {                                \
    int t = inf.readInt(1, (int)T, "t"); inf.readEoln();                \
    int64_t sumn = 0;                                                   \
    for (int i = 1; i <= t; i++) {                                      \
        auto m = inf.readLong(1ll, (long long)M, "m"); inf.readSpace(); \
        auto a = inf.readInt(1, (int)ABC, "a"); inf.readSpace();        \
        auto b = inf.readInt(1, (int)ABC, "b"); inf.readSpace();        \
        auto c = inf.readInt(1, (int)ABC, "c"); inf.readSpace();        \
        ensuref(a < b && b < c, "a < b < c not granteed");              \
        ensuref(b % a == 0, "b %% a"); ensuref(c % b == 0, "c %% b");   \
        sumn += inf.readInt(1, (int)N, "n"); inf.readEoln();            \
    }                                                                   \
    ensuref(sumn <= (int64_t)SN, "Sum of n exceeded!");                 \
}
void (*check[])() = {
    //      t    m     n    sumn  abc 
    Checker(3e5, 1e3,  1,   3e5,  1e2),
    Checker(3e5, 1e3,  1e2, 1e3,  1e3),
    Checker(3e5, 1e6,  1e3, 1e4,  1e6),
    Checker(3e5, 1e15, 1e5, 1e6,  1e6),
    Checker(3e5, 1e15, 1e9, 3e14, 1e8),
};
int main(int argc, char** argv) {
    registerValidation(argc, argv);
    int testid = inf.readInt(1, 20, "tid"); inf.readSpace();
    check[MapToSubTask[testid - 1] - 1]();
    inf.readEof();
    return 0;
}