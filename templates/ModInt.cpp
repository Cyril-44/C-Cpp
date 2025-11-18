#include <bits/c++config.h>
#include <bits/stdc++.h>
template <typename T>
T modInv(const T& x, const T& y) { // x * u ≡ 1 (mod y)
    assert(x != 0);
    T u = 0, v = 1, a = x, m = y, t;
    while (a != 0) {
        t = m / a;
        std::swap(a, m -= t * a);
        std::swap(u -= t * v, v);
    }
    assert(m == 1);
    return u;
}

template<typename Mod>
class MB {
    using Int = typename std::enable_if<std::is_signed<typename std::decay<decltype(Mod::value)>::type>::value, typename std::decay<decltype(Mod::value)>::type>::type;
    Int value;
    constexpr static Int mod() { return Mod::value; }
    template<typename T> T normalize(T x) {
        if _GLIBCXX17_CONSTEXPR (std::is_unsigned<T>::value)
            return static_cast<Int>(x < mod() ? x : x % mod());
        else {
            Int res = static_cast<Int>(-mod() < x && x < mod() ? x : x % mod());
            return (res < 0 ? res + mod() : res);
        }
    }
public:
    constexpr MB() : value() {}
    template<typename T> MB(const T &rhs) { value = normalize(rhs); }
    template<typename T> explicit operator T() const { return static_cast<T>(value); }
    const Int& operator()() const { return value; }
    MB& operator+=(const MB& rhs) {
        if ((value += rhs.value) >= mod()) value -= mod();
        return *this;
    }
    MB& operator-=(const MB& rhs) {
        if ((value -= rhs.value) < 0) value += mod();
        return *this;
    }
    MB& operator*=(const MB& rhs) {
        if _GLIBCXX17_CONSTEXPR (std::is_same<Int, int>::value)
            value = normalize((uint64_t)value * rhs.value);
        else if _GLIBCXX17_CONSTEXPR (std::is_same<Int, int64_t>::value)
            value = normalize((unsigned __int128)value * rhs.value);
        else 
            value = normalize(value * rhs.value);
        return *this;
    }
    MB& operator/=(const MB& rhs) { return *this *= MB(modInv(rhs.value, mod())); }
    template<typename T> typename std::enable_if<std::is_integral<T>::value, MB>::type& operator^=(T rhs) {
        if (rhs < 0) return *this = MB(modInv(static_cast<Int>(1), (*this ^ (-rhs))()));
        MB tmp = *this;
        for (*this = static_cast<Int>(1); rhs; rhs >>= 1) {
            if (rhs & 1) *this *= tmp;
            tmp *= tmp;
        }
        return *this;
    }
    MB operator-() const { return MB(-value); }
    MB& operator++() { return *this += 1; }
    MB& operator--() { return *this -= 1; }
    MB operator++(int) { MB tmp = *this; ++*this; return tmp; }
    MB operator--(int) { MB tmp = *this; --*this; return tmp; }
    MB operator+(const MB& rhs) const { return MB(*this) += rhs; }
    MB operator-(const MB& rhs) const { return MB(*this) -= rhs; }
    MB operator*(const MB& rhs) const { return MB(*this) *= rhs; }
    MB operator/(const MB& rhs) const { return MB(*this) /= rhs; }
    bool operator==(const MB& rhs) const { return value == rhs.value; }
    bool operator!=(const MB& rhs) const { return value != rhs.value; }
    bool operator<=(const MB& rhs) const { return value <= rhs.value; }
    bool operator>=(const MB& rhs) const { return value >= rhs.value; }
    bool operator<(const MB& rhs) const { return value < rhs.value; }
    bool operator>(const MB& rhs) const { return value > rhs.value; }
    bool operator!() const { return value; }
    template<typename IStream, typename T> friend IStream& operator>>(IStream& , MB<T>& );
    template<typename OStream, typename T> friend OStream& operator<<(OStream& , const MB<T>& );
    template<typename U> friend MB operator+(const MB& lhs, const U& rhs) { return lhs + MB(rhs); }
    template<typename U> friend MB operator+(const U& lhs, const MB& rhs) { return MB(lhs) + rhs; }
    template<typename U> friend MB operator-(const MB& lhs, const U& rhs) { return lhs - MB(rhs); }
    template<typename U> friend MB operator-(const U& lhs, const MB& rhs) { return MB(lhs) - rhs; }
    template<typename U> friend MB operator*(const MB& lhs, const U& rhs) { return lhs * MB(rhs); }
    template<typename U> friend MB operator*(const U& lhs, const MB& rhs) { return MB(lhs) * rhs; }
    template<typename U> friend MB operator/(const MB& lhs, const U& rhs) { return lhs / MB(rhs); }
    template<typename U> friend MB operator/(const U& lhs, const MB& rhs) { return MB(lhs) / rhs; }
    template<typename U> friend typename std::enable_if<std::is_integral<U>::value, MB>::type operator^(MB lhs, const U& rhs) { return lhs ^= rhs; }
    template<typename U> friend MB operator+=(MB& lhs, const U& rhs) { return lhs += MB(rhs); }
    template<typename U> friend MB operator-=(MB& lhs, const U& rhs) { return lhs -= MB(rhs); }
    template<typename U> friend MB operator*=(MB& lhs, const U& rhs) { return lhs *= MB(rhs); }
    template<typename U> friend MB operator/=(MB& lhs, const U& rhs) { return lhs /= MB(rhs); }
    template<typename U> friend MB operator==(const MB& lhs, const U& rhs) { return lhs == MB(rhs); }
    template<typename U> friend MB operator==(const U& lhs, const MB& rhs) { return MB(lhs) == rhs; }
    template<typename U> friend MB operator!=(const MB& lhs, const U& rhs) { return lhs != MB(rhs); }
    template<typename U> friend MB operator!=(const U& lhs, const MB& rhs) { return MB(lhs) != rhs; }
    template<typename U> friend MB operator<=(const MB& lhs, const U& rhs) { return lhs <= MB(rhs); }
    template<typename U> friend MB operator<=(const U& lhs, const MB& rhs) { return MB(lhs) <= rhs; }
    template<typename U> friend MB operator>=(const MB& lhs, const U& rhs) { return lhs >= MB(rhs); }
    template<typename U> friend MB operator>=(const U& lhs, const MB& rhs) { return MB(lhs) >= rhs; }
    template<typename U> friend MB operator<(const MB& lhs, const U& rhs) { return lhs < MB(rhs); }
    template<typename U> friend MB operator<(const U& lhs, const MB& rhs) { return MB(lhs) < rhs; }
    template<typename U> friend MB operator>(const MB& lhs, const U& rhs) { return lhs > MB(rhs); }
    template<typename U> friend MB operator>(const U& lhs, const MB& rhs) { return MB(lhs) > rhs; }
};
template<typename IStream, typename T>
IStream& operator>>(IStream& is, MB<T>& lhs) {
    typename MB<T>::Int val; is >> val;
    lhs.value = lhs.normalize(val);
    return is;
}
template<typename OStream, typename T>
OStream& operator<<(OStream& os, const MB<T>& rhs) { return os << rhs.value; }
constexpr auto MOD = (int)1e9 + 7;
using Mint = MB<std::integral_constant<decltype(MOD), MOD>>;
// struct Dynamic_ModInt { using type = int; static type value; };
// ModType &Mod = Dynamic_ModInt::value;
// using Modular = Modular_Base<Dynamic_ModInt>;
struct Fact {
    Fact(const int &n) : fact(n+1, Mint(1)), invfact(n+1), size(n) {
        fact[0] = 1;
        for (int i = 1; i <= n; i++) fact[i] = fact[i-1] * i;
        invfact[n] = Mint(1) / fact[n];
        for (int i = n; i >= 1; i--) invfact[i-1] = invfact[i] * i;
    }
    Mint C(const int &n, const int &m) const {
        if (n < 0 || m < 0 || n < m) return 0;
        if (n > size) throw std::out_of_range("Expected n < " + std::to_string(size) + ", but found n = " + std::to_string(n) + ".");
        return fact[n] * invfact[m] * invfact[n - m];
    }
    Mint A(const int &n, const int &m) const {
        if (n < 0 || m < 0 || n < m) return 0;
        if (n > size) throw std::out_of_range("Expected n < " + std::to_string(size) + ", but found n = " + std::to_string(n) + ".");
        return fact[n] * invfact[n - m];
    }
private:
    std::vector<Mint> fact, invfact;
    const int size;
};
int main() {
    1 + Mint(1);
    Mint abc = 1;
    return 0;
}