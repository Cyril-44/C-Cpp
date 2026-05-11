#include <cstdio>
#include <string>
#include <concepts>
#include <type_traits>

// 定义一个概念来匹配所有整数类型，包括 __int128
template <typename T>
concept IntegerWithI128 = std::integral<T> || std::same_as<T, __int128_t> || std::same_as<T, __uint128_t>;

class FastOS {
    static constexpr size_t BUF_SIZ = 1 << 20;
    char buffer[BUF_SIZ], *p = buffer;
    FILE *dest;
    int prec = -1;
public:
    inline FastOS(FILE *f = stdout) : dest(f) { setvbuf(dest, nullptr, _IONBF, 0); }
    inline ~FastOS() { flush(); }
    inline void flush() {
        fwrite(buffer, 1, p - buffer, dest);
        p = buffer;
    }
    [[gnu::always_inline]] inline void put(char c) {
        if (p == buffer + BUF_SIZ) flush();
        *p++ = c;
    }
    inline FastOS& setprecision(int n) { prec = n; return *this; }
    inline FastOS& operator<<(char c) { put(c); return *this; }
    inline FastOS& operator<<(const char *s) {
        while (*s) put(*s++);
        return *this;
    }
    FastOS& operator<<(const std::string &s) {
        for (char c : s) put(c);
        return *this;
    }
    template <IntegerWithI128 T>
    inline FastOS& operator<<(T rhs) {
        if (rhs == 0) { put('0'); return *this; }
        T temp = rhs;
        if constexpr (std::is_signed_v<T> || std::same_as<T, __int128_t>) {
            if (temp < 0) { put('-'); temp = -temp; }
        }
        static char stack[64];
        int top = 0;
        while (temp) {
            stack[top++] = static_cast<char>(temp % 10) ^ '0';
            temp /= 10;
        }
        while (top) put(stack[--top]);
        return *this;
    }
    template <std::floating_point T>
    inline FastOS& operator<<(T rhs) {
        if (rhs < 0) { put('-'); rhs = -rhs; }
        __uint128_t inte = static_cast<__uint128_t>(rhs);
        *this << inte;
        T frac = rhs - static_cast<T>(inte);
        if (prec >= 0 || frac > 1e-12) {
            put('.');
            int count = (prec >= 0) ? prec : 6; // 默认6位精度
            while (count--) {
                frac *= 10;
                int digit = static_cast<int>(frac);
                put(digit ^ '0');
                frac -= digit;
            }
        }
        return *this;
    }
} fout;
#ifdef MULTI_TEST_CASES
auto __read_extra_test_cases = [](int x){fin >> x; return x;}();
#endif
int main() {
    double x;
    fout.setprecision(6) << x;
    return 0;
}