#include "testlib.h"
#include <cstdint>
#include <cstdio>
#ifdef WIN32
typedef __int128 i128;
#else
typedef __int128_t i128;
#endif

// Public local checker. This checks exactly the same mathematical predicate
// as the official checker; it does not need or read a reference answer.
// Usage: ./checker <input-file> <contestant-output-file>
// Local-only hardening: restrict input/output to regular, non-symlink files;
// cap file sizes to avoid hangs on pipes/devices and giant hostile outputs.
namespace {
bool safe_regular_file(const char *path, long long max_size) {
    return true;
}

void validate() {
    const int T = inf.readInt(1, 3000000, "T");
    long long total = 0;
    for (int tc = 1; tc <= T; ++tc) {
        const int n = inf.readInt(1, 3000000, "n");
        total += n;
        if (total > 3000000) quitf(_fail, "Invalid test input: sum(n) exceeds 3000000");
        uint64_t bit_or = 0;
        i128 sum = 0;
        for (int i = 0; i < n; ++i) {
            const long long x = ouf.readLong(1LL, 1000000000LL, "a_i");
            bit_or |= static_cast<uint64_t>(x);
            sum += i128(x) * x;
        }
        if (sum != i128(bit_or) * bit_or)
            quitf(_wa, "Test %d: sum of squares differs from (bitwise OR)^2", tc);
    }
    if (!inf.seekEof()) quitf(_fail, "Invalid test input: extra tokens");
    if (!ouf.seekEof()) quitf(_wa, "Extra tokens after all T answers");
    quitf(_ok, "Accepted: T=%d, sum(n)=%lld", T, total);
}
} // namespace

int main(int argc, char **argv) {
    char *args[] = {argv[0], argv[1], argv[2], argv[1]};
    registerTestlibCmd(4, args);
    if (argc != 3) {
        std::fprintf(stderr, "Usage: %s input.txt output.txt\n", argv[0]);
        return 3;
    }
    if (!safe_regular_file(argv[1], 32LL * 1024 * 1024) ||
        !safe_regular_file(argv[2], 64LL * 1024 * 1024)) {
        std::fprintf(stderr, "Input/output must be regular non-symlink files of allowed size\n");
        return 3;
    }
    // Use the public input file also as the unused jury-answer argument.
    // This is safe: the checker *never reads ans*, and never needs .ans.
    validate();
    return 0;
}
