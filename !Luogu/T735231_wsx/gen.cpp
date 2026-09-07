#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int n, W, V;

// 生成一个随机区间内的整数
int rndInt(int L, int R) {
    return rnd.next(L, R);
}

// 完全随机
vector<pair<int,int>> genRandom(int n, int W, int V) {
    vector<pair<int,int>> a(n);
    for (int i = 0; i < n; i++) {
        a[i].first = rndInt(1, W);
        a[i].second = rndInt(1, V);
    }
    return a;
}

// 性质 A：w 严格递增，v 严格递增
// 为了保证严格单调，直接构造序列：w[i] = i+1, v[i] = i+1
// 若你想强制不超过某上界，需要保证 n <= 上界
vector<pair<int,int>> genTypeA(int n) {
    vector<pair<int,int>> a(n);
    for (int i = 0; i < n; i++) {
        int x = i + 1;
        int y = i + 1;
        a[i] = {x, y};
    }
    return a;
}

// 性质 B：w 严格递增，v 严格递减
// 构造：w[i] = i+1, v[i] = 2n - i
vector<pair<int,int>> genTypeB(int n) {
    vector<pair<int,int>> a(n);
    for (int i = 0; i < n; i++) {
        int x = i + 1;
        int y = n - i;
        a[i] = {x, y};
    }
    return a;
}

// 大量重复值
vector<pair<int,int>> genManyDuplicates(int n, int W, int V) {
    vector<pair<int,int>> a(n);
    for (int i = 0; i < n; i++) {
        if (rnd.next(0, 4) == 0) {
            a[i] = {rndInt(1, W), rndInt(1, V)};
        } else {
            int x = rndInt(1, W);
            int y = rndInt(1, V);
            a[i] = {x, y};
        }
    }
    return a;
}

// 极端集中：所有 w 相同或所有 v 相同
vector<pair<int,int>> genConcentrated(int n, int W, int V, bool sameW) {
    vector<pair<int,int>> a(n);
    int fixed = rndInt(1, sameW ? W : V);
    for (int i = 0; i < n; i++) {
        if (sameW)
            a[i] = {fixed, rndInt(1, V)};
        else
            a[i] = {rndInt(1, W), fixed};
    }
    return a;
}

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    int id = atoi(argv[1]);  // 测试点编号 1~25

    if (id == 1) n = 500, W = V = 500;
    else if (id == 2 || id == 3) n = 500, W = V = 6000;
    else if (id == 4 || id == 5) n = 1000000, W = V = 10;
    else if (6 <= id && id <= 8) n = 2000, W = V = 6000;
    else if (id == 9 || id == 10) n = 8000, W = V = 6000;
    else if (11 <= id && id <= 13) n = 40000, W = V = 3000;
    else if (id == 14) n = 6000, W = V = 6000;  // A
    else if (id == 15 || id == 16) n = 6000, W = V = 6000;  // B
    else if (17 <= id && id <= 20) n = 400000, W = V = 3000;
    else if (21 <= id && id <= 25) n = 1000000, W = V = 6000;
    else n = 1000000, W = V = 6000;

    vector<pair<int,int>> a;

    if (id == 14) {
        // 严格递增 A
        a = genTypeA(n);
    } else if (id == 15 || id == 16) {
        // 严格递增 / 递减 B
        a = genTypeB(n);
    } else if (id % 5 == 0) {
        a = genManyDuplicates(n, W, V);
    } else if (id % 7 == 0) {
        a = genConcentrated(n, W, V, true);
    } else if (id % 11 == 0) {
        a = genConcentrated(n, W, V, false);
    } else {
        a = genRandom(n, W, V);
    }

    println(n);
    for (auto &p : a) println(p.first, p.second);
}
