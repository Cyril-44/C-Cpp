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

template<class Mod, typename Mod::value_type Default = 0>
requires requires(typename Mod::value_type a, typename Mod::value_type b) {
    { a + b } -> std::same_as<typename Mod::value_type>;
    { a - b } -> std::same_as<typename Mod::value_type>;
    { a * b } -> std::same_as<typename Mod::value_type>;
    { a % b } -> std::same_as<typename Mod::value_type>;
} class ModInt {
    using Int = Mod::value_type;
    inline static constexpr Int mod() { return Mod::value; }
    Int value;
    template<typename T> inline T normalize(T x) {
        if constexpr (std::is_unsigned_v<T>)
            return static_cast<Int>(x < mod() ? x : x % mod());
        else {
            Int res = static_cast<Int>(-mod() < x && x < mod() ? x : x % mod());
            return (res < 0 ? res + mod() : res);
        }
    }
public:
    constexpr inline ModInt() : value(Default) {}
    template<typename T> inline ModInt(const T &rhs) { value = normalize(rhs); }
    template<typename T> inline explicit operator T() const { return static_cast<T>(value); }
    inline Int operator()() const { return value; }
    inline ModInt& operator+=(const ModInt& rhs) {
        if ((value += rhs.value) >= mod()) value -= mod();
        return *this;
    }
    inline ModInt& operator-=(const ModInt& rhs) {
        if ((value -= rhs.value) < 0) value += mod();
        return *this;
    }
    inline ModInt& operator*=(const ModInt& rhs) {
        if constexpr (std::is_same_v<Int, int>)
            value = normalize((uint64_t)value * rhs.value);
        else if constexpr (std::is_same_v<Int, int64_t>)
            value = normalize((unsigned __int128)value * rhs.value);
        else 
            value = normalize(value * rhs.value);
        return *this;
    }
    inline ModInt& operator/=(const ModInt& rhs) { return *this *= ModInt(modInv(rhs.value, mod())); }
    template<std::integral T> inline ModInt& operator^=(T rhs) {
        if (rhs < 0) return *this = ModInt(modInv(static_cast<Int>(1), (*this ^ (-rhs))()));
        ModInt tmp = *this;
        for (*this = static_cast<Int>(1); rhs; rhs >>= 1) {
            if (rhs & 1) *this *= tmp;
            tmp *= tmp;
        }
        return *this;
    }
    inline ModInt operator-() const { return ModInt(-value); }
    inline ModInt& operator++() { return *this += 1; }
    inline ModInt& operator--() { return *this -= 1; }
    inline ModInt operator++(int) { ModInt tmp = *this; ++*this; return tmp; }
    inline ModInt operator--(int) { ModInt tmp = *this; --*this; return tmp; }
    inline ModInt operator+(const ModInt& rhs) const { return ModInt(*this) += rhs; }
    inline ModInt operator-(const ModInt& rhs) const { return ModInt(*this) -= rhs; }
    inline ModInt operator*(const ModInt& rhs) const { return ModInt(*this) *= rhs; }
    inline ModInt operator/(const ModInt& rhs) const { return ModInt(*this) /= rhs; }
    inline ModInt operator^(const ModInt& rhs) const { return ModInt(*this) ^= rhs; }
    inline bool operator==(const ModInt& rhs) const { return value == rhs.value; }
    inline bool operator!=(const ModInt& rhs) const { return value != rhs.value; }
    inline bool operator<=(const ModInt& rhs) const { return value <= rhs.value; }
    inline bool operator>=(const ModInt& rhs) const { return value >= rhs.value; }
    inline bool operator<(const ModInt& rhs) const { return value < rhs.value; }
    inline bool operator>(const ModInt& rhs) const { return value > rhs.value; }
    inline bool operator!() const { return value; }
    template<typename U> inline friend ModInt operator+(const U& lhs, const ModInt& rhs) { return ModInt(lhs) + rhs; }
    template<typename U> inline friend ModInt operator-(const U& lhs, const ModInt& rhs) { return ModInt(lhs) - rhs; }
    template<typename U> inline friend ModInt operator*(const U& lhs, const ModInt& rhs) { return ModInt(lhs) * rhs; }
    template<typename U> inline friend ModInt operator/(const U& lhs, const ModInt& rhs) { return ModInt(lhs) / rhs; }
    template<typename U> inline friend ModInt operator==(const U& lhs, const ModInt& rhs) { return ModInt(lhs) == rhs; }
    template<typename U> inline friend ModInt operator!=(const U& lhs, const ModInt& rhs) { return ModInt(lhs) != rhs; }
    template<typename U> inline friend ModInt operator<=(const U& lhs, const ModInt& rhs) { return ModInt(lhs) <= rhs; }
    template<typename U> inline friend ModInt operator>=(const U& lhs, const ModInt& rhs) { return ModInt(lhs) >= rhs; }
    template<typename U> inline friend ModInt operator<(const U& lhs, const ModInt& rhs) { return ModInt(lhs) < rhs; }
    template<typename U> inline friend ModInt operator>(const U& lhs, const ModInt& rhs) { return ModInt(lhs) > rhs; }
    template<typename IStream> inline friend IStream& operator>>(IStream& is, ModInt& lhs) {
        is >> lhs.value;
        lhs.value = lhs.normalize(lhs.value);
        return is;
    }
    template<typename OStream> friend inline OStream& operator<<(OStream& os, const ModInt& rhs) { return os << rhs.value; }
};
constexpr auto MOD = (int)1e9 + 7;
using Mint = ModInt<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
// struct Dynamic_ModInt { using value_type = int; static value_type value; };
// Dynamic_ModInt::value_type &Mod = Dynamic_ModInt::value;
// using Mint = ModInt<Dynamic_ModInt>;
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