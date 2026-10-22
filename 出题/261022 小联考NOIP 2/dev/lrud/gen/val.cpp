#include "testlib.h"

int main(int argc, char* argv[]) {
    registerValidation(argc, argv);
    prepareOpts(argc, argv);
    const int st = opt<int>("subtask", 5);
    ensuref(1 <= st && st <= 5, "subtask must be in [1,5]");
    int t = inf.readInt(1, 5, "T");
    inf.readEoln();
    for (int i = 1; i <= t; ++i) {
        setTestCase(i);
        int n = inf.readInt(2, 1000, "N");
        inf.readEoln();
        if (st == 1) ensuref(n <= 3, "subtask 1 requires N<=3");
        if (st == 2) ensuref(n <= 4, "subtask 2 requires N<=4");
        if (st == 3) ensuref(n <= 4 || n % 2 == 1, "subtask 3 requires N<=4 or odd N");
        if (st == 4) ensuref(n <= 4 || n % 4 != 0, "subtask 4 requires N<=4 or N%%4!=0");
    }
    inf.readEof();
}
