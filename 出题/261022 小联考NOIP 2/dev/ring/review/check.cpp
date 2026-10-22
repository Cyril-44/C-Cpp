#include "testlib.h"
// One integer modulo 1e9+7; whitespace is ignored, extra tokens are rejected.
int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);
    long long expected = ans.readLong(0LL, 1000000006LL, "answer");
    ans.seekEof();
    long long actual = ouf.readLong(0LL, 1000000006LL, "answer");
    if (!ouf.seekEof()) quitf(_pe, "extra output tokens");
    if (actual != expected) quitf(_wa, "expected %lld, found %lld", expected, actual);
    quitf(_ok, "answer = %lld", actual);
}
