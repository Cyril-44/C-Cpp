#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    // 读取 n
    int n = inf.readInt(1, 2022, "n");
    inf.readEoln();

    // 读取 a_i
    long long sumA = 0;
    for (int i = 1; i <= n; i++) {
        long long ai = inf.readInt(1, 200000, "a_i");
        sumA += ai;
        if (i < n) inf.readSpace();
    }
    inf.readEoln();

    // 检查 sum(a_i) ≤ 515n
    if (sumA > 515LL * n) {
        quitf(_fail, "sum(a_i) = %lld exceeds 515 * n = %lld", sumA, 515LL * n);
    }

    inf.readEof();
}
