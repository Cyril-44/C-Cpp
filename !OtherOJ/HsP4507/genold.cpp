#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char* argv[]) {
    registerGen(argc, argv, 1);

    int T = atoi(argv[1]);
    int maxN = atoi(argv[2]);

    cout << T << "\n";

    for (int t = 0; t < T; t++) {
        int n = rnd.next(1, maxN);
        int mode = rnd.next(0, 5);
        long long seed = rnd.next(0LL, (1LL << 60));

        cout << n << " " << mode << " " << seed << "\n";
    }
}
