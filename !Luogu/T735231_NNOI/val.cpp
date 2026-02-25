#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    // 读取 n
    int n = inf.readInt(1, 200000, "n");
    inf.readEoln();

    for (int i = 1; i <= n; i++) {
        long long w = inf.readLong(1, 1000000000LL, "w_i");
        inf.readSpace();
        long long v = inf.readLong(1, 1000000000LL, "v_i");
        inf.readEoln();
    }

    // 文件必须结束
    inf.readEof();
    return 0;
}
