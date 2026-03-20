#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    if (argc < 4) {
        fprintf(stderr, "Usage: %s n amax mode\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    int amax = atoi(argv[2]);
    int mode = atoi(argv[3]);

    // 题目上限保护
    amax = max(1, min(amax, 200000));

    long long limit = 515LL * n;
    vector<int> a(n, 1);

    if (mode == 0) {
        // mode 0: 完全随机，但保证 sum <= 515n
        long long rem = limit;
        for (int i = 0; i < n; i++) {
            int left = n - i;
            long long hi = min<long long>(amax, rem - (left - 1));
            // 理论上 hi >= 1 恒成立（因为 limit >= n 且每次至少留 1 给后面）
            int val = rnd.next(1, (int)hi);
            a[i] = val;
            rem -= val;
        }
    } else if (mode == 1) {
        // mode 1: sum 接近 515n，偏大一些
        long long rem = limit;
        for (int i = 0; i < n; i++) {
            int left = n - i;
            long long hi = min<long long>(amax, rem - (left - 1));
            int hiInt = (int)hi;
            int loInt = (int)(hi * 0.8);
            if (loInt < 1) loInt = 1;
            if (loInt > hiInt) loInt = 1; // 兜底，保证区间合法
            int val = rnd.next(loInt, hiInt);
            a[i] = val;
            rem -= val;
        }
    } else if (mode == 2) {
        // mode 2: sum 很小，尽量用小数值（1 或 2），但仍保证 sum <= 515n
        long long rem = limit;
        for (int i = 0; i < n; i++) {
            int left = n - i;
            long long hi = min<long long>(min(amax, 2), rem - (left - 1));
            // hi 至少为 1
            int val = rnd.next(1, (int)hi);
            a[i] = val;
            rem -= val;
        }
    } else if (mode == 3) {
        // mode 3: 全部相等
        int v = (int)min<long long>(amax, limit / n);
        if (v < 1) v = 1; // 理论上不会发生，因为 limit/n = 515
        for (int i = 0; i < n; i++) a[i] = v;
        // sum = v * n <= limit
    } else if (mode == 4) {
        // mode 4: 大部分很小，偶尔给一个尽量大的
        long long rem = limit;
        for (int i = 0; i < n; i++) {
            int left = n - i;
            long long hi = min<long long>(amax, rem - (left - 1));
            int val;
            if (rnd.next(0, 9) == 0) {
                // 偶尔给一个尽量大的
                val = (int)hi;
            } else {
                int smallHi = (int)min<long long>(5, hi);
                val = rnd.next(1, smallHi);
            }
            a[i] = val;
            rem -= val;
        }
    } else {
        // 未知模式：退化为 mode 0
        long long rem = limit;
        for (int i = 0; i < n; i++) {
            int left = n - i;
            long long hi = min<long long>(amax, rem - (left - 1));
            int val = rnd.next(1, (int)hi);
            a[i] = val;
            rem -= val;
        }
    }

    // 输出
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        if (i) cout << " ";
        cout << a[i];
    }
    cout << "\n";
}
