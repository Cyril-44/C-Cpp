#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cerrno>
#include <type_traits>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

namespace detail {
    constexpr std::size_t CAP = 31000000;
    struct InputHelper { alignas(64) std::uint8_t table[65536]; constexpr InputHelper() : table{} { for (unsigned a = 0; a < 10; ++a) for (unsigned b = 0; b < 10; ++b) table[(a + 48) | ((b + 48) << 8)] = a * 10 + b + 1; } };
    static constexpr InputHelper I{};
    alignas(64) static char buffer[CAP + 64];
}

class Scanner {
    const char *i = detail::buffer, *end = detail::buffer, *boundary = nullptr;
#ifndef _WIN32
    const char *mapped = nullptr; std::size_t mappedSize = 0;
#endif
    inline void skip() {
        while (i < end && (unsigned char)*i <= 32) ++i;
#ifndef _WIN32
        if (__builtin_expect(boundary && i >= boundary, 0)) { std::size_t n = end - i; std::memcpy(detail::buffer, i, n); std::memset(detail::buffer + n, 0, 32); i = detail::buffer; end = i + n; boundary = nullptr; }
#endif
    }
    template <class T> inline void s2(T& v) { std::uint16_t k; std::memcpy(&k, i, 2); unsigned x = detail::I.table[k]; if (x) v = T(v * 100 + x - 1), i += 2; }
    template <class T> inline void s4(T& v) { s2(v); s2(v); }
    static inline bool digit(char c) { return (unsigned)(c - '0') < 10; }
public:
    explicit Scanner(int fd = 0) {
#ifndef _WIN32
        struct stat st{};
        if (!fstat(fd, &st) && S_ISREG(st.st_mode) && st.st_size > 0) {
            void* p = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (p != MAP_FAILED) { mapped = i = (const char*)p; mappedSize = st.st_size; end = i + mappedSize; boundary = i + (mappedSize > 32 ? mappedSize - 32 : 0); skip(); return; }
        }
#else
        _setmode(fd, _O_BINARY);
#endif
        std::size_t n = 0;
        while (n < detail::CAP) {
#ifdef _WIN32
            int r = _read(fd, detail::buffer + n, (unsigned)(detail::CAP - n));
#else
            ssize_t r = read(fd, detail::buffer + n, detail::CAP - n);
#endif
            if (r > 0) n += r; else if (!r) break; else if (errno != EINTR) break;
        }
        end = i + n; std::memset(detail::buffer + n, 0, 32); skip();
    }
#ifndef _WIN32
    ~Scanner() { if (mapped) munmap((void*)mapped, mappedSize); }
#endif
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;
template <class T> inline void scan(T& v) {
    static_assert(std::is_integral<T>::value && (sizeof(T) == 4 || sizeof(T) == 8), "Unsupported integer type");
    using U = typename std::make_unsigned<T>::type;
    bool neg = std::is_signed<T>::value && *i == '-'; i += neg;
    U x = 0;
    if (sizeof(T) == 4) { s4(x); s4(x); s2(x); }
    else { s4(x); s4(x); s4(x); s4(x); s4(x); }
    while (i < end && digit(*i)) x = x * 10 + (*i++ - '0');
    skip();
    if (neg) x = U(0) - x;
    std::memcpy(&v, &x, sizeof(v));
}
    template <class T> inline Scanner& operator>>(T& v) { return scan(v), *this; }
};

#include<iostream>

int main(){
    static Scanner cin;
    int a,b; cin>>a>>b;
    std::cout<<a+b<<std::endl;
}