#include <cstdint>
#include <cstdio>
#include <vector>

// Independent O(nm) oracle. Operation 1 completes the quadratic sequence of
// window sums directly; it never constructs or repeats array differences.
namespace {
constexpr int64_t MOD = 998244353;

struct FastInput {
    static constexpr size_t SIZE = 1 << 16;
    char buffer[SIZE];
    size_t pos = 0, len = 0;
    int get() {
        if (pos == len) {
            len = std::fread(buffer, 1, SIZE, stdin);
            pos = 0;
            if (!len) return EOF;
        }
        return buffer[pos++];
    }
    bool read(int64_t &value) {
        int ch;
        do { ch = get(); } while (ch != EOF && ch <= ' ');
        if (ch == EOF) return false;
        bool negative = ch == '-';
        if (negative) ch = get();
        value = 0;
        while (ch >= '0' && ch <= '9') {
            value = value * 10 + ch - '0';
            ch = get();
        }
        if (negative) value = -value;
        return true;
    }
    int64_t next() { int64_t value = 0; read(value); return value; }
};

int64_t norm(int64_t x) {
    x %= MOD;
    return x < 0 ? x + MOD : x;
}
}  // namespace

int main() {
    FastInput input;
    int64_t n64;
    if (!input.read(n64)) return 0;
    const int n = static_cast<int>(n64);
    std::vector<int64_t> a(n + 1);
    for (int i = 1; i <= n; ++i) a[i] = norm(input.next());
    const int m = static_cast<int>(input.next());
    for (int q = 0; q < m; ++q) {
        const int type = static_cast<int>(input.next());
        const int l = static_cast<int>(input.next());
        const int r = static_cast<int>(input.next());
        if (type == 1) {
            const int k = static_cast<int>(input.next());
            int64_t w0 = 0;
            for (int i = l; i < l + k; ++i) w0 = norm(w0 + a[i]);
            const int64_t w1 = norm(w0 - a[l] + a[l + k]);
            int64_t previous_window = norm(w1 - a[l + 1] + a[l + k + 1]);
            const int64_t curvature = norm(previous_window - 2 * w1 + w0);
            int64_t window_step = norm(previous_window - w1);
            // The first three windows use exactly the retained k+2 elements.
            for (int i = l + k + 2; i <= r; ++i) {
                window_step = norm(window_step + curvature);
                const int64_t next_window = norm(previous_window + window_step);
                // The new window loses a[i-k] and gains the unknown a[i].
                a[i] = norm(next_window - previous_window + a[i - k]);
                previous_window = next_window;
            }
        } else if (type == 2) {
            const int64_t u = norm(input.next());
            const int64_t v = norm(input.next());
            for (int i = l; i <= r; ++i) a[i] = (u * a[i] + v) % MOD;
        } else if (type == 3) {
            int64_t answer = 0;
            for (int i = l; i <= r; ++i) answer = norm(answer + a[i]);
            std::printf("%lld\n", static_cast<long long>(answer));
        } else {
            return 2;
        }
    }
    return 0;
}
