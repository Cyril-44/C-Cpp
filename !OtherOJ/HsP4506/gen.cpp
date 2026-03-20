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

    long long limit = 515LL * n;
    vector<int> a(n);

    if (mode == 0) {
        // random
        long long rem = limit;
        for (int i = 0; i < n; i++) {
            int left = n - i;
            long long hi = min<long long>(amax, rem - (left - 1));
            a[i] = rnd.next(1, (int)hi);
            rem -= a[i];
        }
    } else if (mode == 1) {
        // sum close to 515n
        long long rem = limit;
        for (int i = 0; i < n; i++) {
            int left = n - i;
            long long hi = min<long long>(amax, rem - (left - 1));
            a[i] = rnd.next((int)(hi * 0.8), (int)hi);
            rem -= a[i];
        }
    } else if (mode == 2) {
        // sum very small
        for (int i = 0; i < n; i++) {
            a[i] = rnd.next(1, min(amax, 2));
        }
    } else if (mode == 3) {
        // all equal
        int v = min(amax, (int)(limit / n));
        for (int i = 0; i < n; i++) a[i] = v;
    } else if (mode == 4) {
        // extreme: most small, few huge
        long long rem = limit;
        for (int i = 0; i < n; i++) {
            int left = n - i;
            long long hi = min<long long>(amax, rem - (left - 1));
            if (rnd.next(0, 9) == 0) a[i] = hi; // occasionally huge
            else a[i] = rnd.next(1, min(amax, 5));
            rem -= a[i];
        }
    }

    cout << n << "\n";
    for (int i = 0; i < n; i++) {
        if (i) cout << " ";
        cout << a[i];
    }
    cout << "\n";
}
