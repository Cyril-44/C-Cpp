#include <bits/stdc++.h>
class FastIS {
    static constexpr size_t BUF_SIZ = 1 << 20;
    char buffer[BUF_SIZ];
    char *p1, *p2;
    FILE *src;
public:
    FastIS(FILE* f = stdin) : p1(nullptr), p2(nullptr), src(f) {}
    __attribute__((always_inline)) inline int get() {
        return (p1 == p2) && (p2 = (p1 = buffer) + fread(buffer, 1, BUF_SIZ, src), p1 == p2) ? EOF : *p1++;
    }
    template<typename T> typename std::enable_if<std::is_unsigned<T>::value && std::is_integral<T>::value, 
    FastIS&>::type inline operator>>(T &rhs) {
        int ch = get();
        while (~ch && (ch < '0' || ch > '9')) ch = get();
        for (rhs = 0; ch >= '0' && ch <= '9'; ch = get())
            rhs = (rhs << 3) + (rhs << 1) + (ch ^ '0');
        return *this;
    }
    template<typename T> typename std::enable_if<
        std::is_signed<T>::value && std::is_integral<T>::value &&
        !std::is_same<T, bool>::value && !std::is_same<T, char>::value, 
    FastIS&>::type inline operator>>(T &rhs) {
        int ch = get();
        bool flg = false;
        while (~ch && (ch < '0' || ch > '9') && (ch ^ '-')) ch = get();
        if (ch == '-') ch = get(), flg = true;
        for (rhs = 0; ch >= '0' && ch <= '9'; ch = get())
            rhs = (rhs << 3) + (rhs << 1) + (ch ^ '0');
        rhs = flg ? -rhs : rhs;
        return *this;
    }
    template<typename T> typename std::enable_if<std::is_floating_point<T>::value, 
    FastIS&>::type inline operator>>(T &rhs) {
        int ch = get();
        bool flg = false;
        while (~ch && (ch < '0' || ch > '9') && (ch ^ '-')) ch = get();
        if (ch == '-') ch = get(), flg = true;
        using Int = typename std::conditional<std::is_same<T, float>::value, std::uint32_t, std::uint64_t>::type;
        Int integer;
        for (integer = 0; ch >= '0' && ch <= '9'; ch = get())
            integer = (integer << 3) + (integer << 1) + (ch ^ '0');
        rhs = integer;
        if (ch == '.') {
            Int base = 1;
            for (ch = get(); ch >= '0' && ch <= '9'; ch = get())
                rhs += (ch ^ '0') * (1. / (base *= 10));
        }
        if (flg) rhs = -rhs;
        return *this;
    }
    inline FastIS& operator>>(bool &x) {
        int ch = get();
        while (~ch && ch != '0' && ch != '1') ch = get();
        x = static_cast<bool>(ch & 1);
        return *this;
    }
    inline FastIS& operator>>(char &s) {
        int ch = get();
        while (ch == ' ' || ch == '\r' || ch == '\n') ch = get();
        s = ch;
        return *this;
    }
    inline FastIS& operator>>(char *s) {
        int ch = get();
        while (~ch && (ch == ' ' || ch == '\r' || ch == '\n')) ch = get();
        while (~ch && (ch != ' ' && ch != '\r' && ch != '\n'))
            *s++ = ch, ch = get();
        return *this;
    }
} fin;
class FastOS {
    static constexpr size_t BUF_SIZ = 1 << 20;
    static constexpr double EPS = 1e-9;
    char buffer[BUF_SIZ], *p;
    const char *pt;
    FILE *dest;
    int prec;
public:
    FastOS(FILE* f = stdout) : p(buffer), pt(buffer + BUF_SIZ), dest(f), prec(-1) { setvbuf(dest, nullptr, _IONBF, 0); }
    ~FastOS() { flush(); fclose(dest); }
    FastOS& flush() {
        fwrite(buffer, p - buffer, 1, dest);
        return *this;
    }
    FastOS& put(int c) {
        *p++ = c;
        if (p == pt) fwrite(buffer, BUF_SIZ, 1, dest), p = buffer;
        return *this;
    }
    FastOS& setprecision(const int& new_prec) {
        prec = new_prec;
        return *this;
    }
    template<typename T> typename std::enable_if<std::is_unsigned<T>::value && std::is_integral<T>::value, 
    FastOS&>::type inline operator<<(T rhs) {
        static int s[32];
        s[*s = 1] = rhs % 10;
        while (rhs /= 10) s[++(*s)] = rhs % 10;
        while (*s) put(s[(*s)--] | '0');
        return *this;
    }
    template<typename T> typename std::enable_if<
        std::is_signed<T>::value && std::is_integral<T>::value &&
        !std::is_same<T, char>::value, 
    FastOS&>::type inline operator<<(const T &rhs) {
        if (rhs < 0) put('-') << (typename std::make_unsigned<T>::type)(-rhs);
        else *this << (typename std::make_unsigned<T>::type)(rhs);
    }
    template<typename T> typename std::enable_if<std::is_floating_point<T>::value, 
    FastOS&>::type inline operator<<(T rhs) {
        if (rhs < 0) put('-'), rhs = -rhs;
        using Int = typename std::conditional<std::is_same<T, float>::value, std::uint32_t, std::uint64_t>::type;
        Int inte = static_cast<Int>(rhs);
        T frac = rhs - inte;
        *this << inte;
        if (prec > 0 || prec < 0 && frac > EPS) {
            put('.');
            for (int i = 0; prec > 0 && i < prec || prec < 0 && frac > EPS; i++) {
                int digit = static_cast<int>(frac *= 10);
                put(digit | '0');
                frac -= digit;
            }
        }
        return *this;
    }
    FastOS& operator<<(const char &c) {
        put(c);
        return *this;
    }
} fout;
int main() {
    double x;
    fin >> x;
    fout.setprecision(6) << x;
    return 0;
}