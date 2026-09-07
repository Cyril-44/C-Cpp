#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    if (argc < 2)
        quitf(_fail, "Need testpoint id as first argument");

    int id = atoi(argv[1]);

    // 根据题面配置每个测试点的限制
    int maxN = 1000000;
    int maxW = 6000, maxV = 6000;

    if (id == 1) maxN = 500, maxW = 500, maxV = 500;
    else if (id == 2 || id == 3) maxN = 500, maxW = 6000, maxV = 6000;
    else if (id == 4 || id == 5) maxN = 1000000, maxW = 10, maxV = 10;
    else if (6 <= id && id <= 8) maxN = 2000, maxW = 6000, maxV = 6000;
    else if (id == 9 || id == 10) maxN = 8000, maxW = 6000, maxV = 6000;
    else if (11 <= id && id <= 13) maxN = 40000, maxW = 3000, maxV = 3000;
    else if (id == 14) maxN = 1000000, maxW = 6000, maxV = 6000;  // A
    else if (id == 15 || id == 16) maxN = 1000000, maxW = 6000, maxV = 6000;  // B
    else if (17 <= id && id <= 20) maxN = 400000, maxW = 3000, maxV = 3000;
    else if (21 <= id && id <= 25) maxN = 1000000, maxW = 6000, maxV = 6000;

    // 读取 n
    int n = inf.readInt(1, maxN, "n");
    inf.readEoln();

    vector<int> w(n), v(n);

    for (int i = 0; i < n; i++) {
        w[i] = inf.readInt(1, maxW, "w_i");
        inf.readSpace();
        v[i] = inf.readInt(1, maxV, "v_i");
        inf.readEoln();
    }

    // 特殊性质 A：w 严格递增，v 严格递增
    if (id == 14) {
        for (int i = 0; i + 1 < n; i++) {
            ensuref(w[i] < w[i+1], "Testpoint 14 requires w strictly increasing");
            ensuref(v[i] < v[i+1], "Testpoint 14 requires v strictly increasing");
        }
    }

    // 特殊性质 B：w 严格递增，v 严格递减
    if (id == 15 || id == 16) {
        for (int i = 0; i + 1 < n; i++) {
            ensuref(w[i] < w[i+1], "Testpoint 15/16 requires w strictly increasing");
            ensuref(v[i] > v[i+1], "Testpoint 15/16 requires v strictly decreasing");
        }
    }

    inf.readEof();
    return 0;
}
