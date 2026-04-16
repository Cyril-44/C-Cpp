#include "testlib.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);

    long long n = inf.readLong(1LL, 1000000LL, "n");
    inf.readSpace();
    long long m = inf.readLong(0LL, 100000000LL, "m");
    inf.readSpace();
    long long x = inf.readLong(0LL, 10000000000LL, "x");
    inf.readEoln();

    ensuref(n % 2 != 0, "n must be odd");

    for (int i = 0; i < n; ++i) {
        inf.readLong(-1000000000000LL, 1000000000000LL, "a_i");
        if (i < n - 1) inf.readSpace();
    }
    inf.readEoln();
    inf.readEof();

    return 0;
}