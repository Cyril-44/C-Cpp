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
class Modular_Base {
    using Int = typename std::decay<decltype(Mod::value)>::type;
    Int value;
    constexpr static Int mod() { return Mod::value; }
    template<typename T> T normalize(T x) {
        Int res = static_cast<Int>(-mod() < x && x < mod() ? x : x % mod());
        return (res < 0 ? res += mod() : res);
    }
public:
    constexpr Modular_Base() : value() {}
    template<typename T> Modular_Base(const T &rhs) { value = normalize(rhs); }
    template<typename T> explicit operator T() const { return static_cast<T>(value); }
    const Int& operator()() const { return value; }
    Modular_Base& operator+=(const Modular_Base& rhs) {
        if ((value += rhs.value) >= mod()) value -= mod();
        return *this;
    }
    Modular_Base& operator-=(const Modular_Base& rhs) {
        if ((value -= rhs.value) < 0) value += mod();
        return *this;
    }
    Modular_Base& operator*=(const Modular_Base& rhs) {
        if constexpr (std::is_same<Int, int>::value) 
            value = normalize(static_cast<int64_t>(value) * rhs.value);
        else value = normalize(value * rhs.value);
        return *this;
    }
    Modular_Base& operator/=(const Modular_Base& rhs) { return *this *= Modular_Base(modInv(rhs.value, mod())); }
    template<typename T> typename std::enable_if<std::is_integral<T>::value, Modular_Base>::type& operator^=(T rhs) {
        if (rhs < 0) return *this = Modular_Base(modInv(static_cast<Int>(1), (*this ^ (-rhs))()));
        Modular_Base tmp = *this;
        for (*this = static_cast<Int>(1); rhs; rhs >>= 1) {
            if (rhs & 1) *this *= tmp;
            tmp *= tmp;
        }
        return *this;
    }
    Modular_Base operator-() const { return Modular_Base(-value); }
    Modular_Base& operator++() { return *this += 1; }
    Modular_Base& operator--() { return *this -= 1; }
    Modular_Base operator++(int) { Modular_Base tmp = *this; ++*this; return tmp; }
    Modular_Base operator--(int) { Modular_Base tmp = *this; --*this; return tmp; }
    Modular_Base operator+(const Modular_Base& rhs) const { return Modular_Base(*this) += rhs; }
    Modular_Base operator-(const Modular_Base& rhs) const { return Modular_Base(*this) -= rhs; }
    Modular_Base operator*(const Modular_Base& rhs) const { return Modular_Base(*this) *= rhs; }
    Modular_Base operator/(const Modular_Base& rhs) const { return Modular_Base(*this) /= rhs; }
    bool operator==(const Modular_Base& rhs) const { return value == rhs.value; }
    bool operator!=(const Modular_Base& rhs) const { return value != rhs.value; }
    bool operator<=(const Modular_Base& rhs) const { return value <= rhs.value; }
    bool operator>=(const Modular_Base& rhs) const { return value >= rhs.value; }
    bool operator<(const Modular_Base& rhs) const { return value < rhs.value; }
    bool operator>(const Modular_Base& rhs) const { return value > rhs.value; }
    bool operator!() const { return value; }
    template<typename IStream, typename T> friend IStream& operator>>(IStream& , Modular_Base<T>& );
    template<typename OStream, typename T> friend OStream& operator<<(OStream& , const Modular_Base<T>& );
    template<typename T, typename U> friend Modular_Base operator+(const Modular_Base<T>& lhs, const U& rhs) { return lhs + Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator+(const U& lhs, const Modular_Base<T>& rhs) { return Modular_Base<T>(lhs) + rhs; }
    template<typename T, typename U> friend Modular_Base operator-(const Modular_Base<T>& lhs, const U& rhs) { return lhs - Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator-(const U& lhs, const Modular_Base<T>& rhs) { return Modular_Base<T>(lhs) - rhs; }
    template<typename T, typename U> friend Modular_Base operator*(const Modular_Base<T>& lhs, const U& rhs) { return lhs * Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator*(const U& lhs, const Modular_Base& rhs) { return Modular_Base<T>(lhs) * rhs; }
    template<typename T, typename U> friend Modular_Base operator/(const Modular_Base<T>& lhs, const U& rhs) { return lhs / Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator/(const U& lhs, const Modular_Base& rhs) { return Modular_Base<T>(lhs) / rhs; }
    template<typename T, typename U> friend typename std::enable_if<std::is_integral<U>::value, Modular_Base>::type operator^(Modular_Base<T> lhs, const U& rhs) { return lhs ^= rhs; }
    template<typename T, typename U> friend Modular_Base operator+=(Modular_Base<T>& lhs, const U& rhs) { return lhs += Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator-=(Modular_Base<T>& lhs, const U& rhs) { return lhs -= Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator*=(Modular_Base<T>& lhs, const U& rhs) { return lhs *= Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator/=(Modular_Base<T>& lhs, const U& rhs) { return lhs /= Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator==(const Modular_Base<T>& lhs, const U& rhs) { return lhs == Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator==(const U& lhs, const Modular_Base<T>& rhs) { return Modular_Base<T>(lhs) == rhs; }
    template<typename T, typename U> friend Modular_Base operator!=(const Modular_Base<T>& lhs, const U& rhs) { return lhs != Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator!=(const U& lhs, const Modular_Base<T>& rhs) { return Modular_Base<T>(lhs) != rhs; }
    template<typename T, typename U> friend Modular_Base operator<=(const Modular_Base<T>& lhs, const U& rhs) { return lhs <= Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator<=(const U& lhs, const Modular_Base& rhs) { return Modular_Base<T>(lhs) <= rhs; }
    template<typename T, typename U> friend Modular_Base operator>=(const Modular_Base<T>& lhs, const U& rhs) { return lhs >= Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator>=(const U& lhs, const Modular_Base& rhs) { return Modular_Base<T>(lhs) >= rhs; }
    template<typename T, typename U> friend Modular_Base operator<(const Modular_Base<T>& lhs, const U& rhs) { return lhs < Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator<(const U& lhs, const Modular_Base& rhs) { return Modular_Base<T>(lhs) < rhs; }
    template<typename T, typename U> friend Modular_Base operator>(const Modular_Base<T>& lhs, const U& rhs) { return lhs > Modular_Base<T>(rhs); }
    template<typename T, typename U> friend Modular_Base operator>(const U& lhs, const Modular_Base& rhs) { return Modular_Base<T>(lhs) > rhs; }
};
template<typename IStream, typename T>
IStream& operator>>(IStream& is, Modular_Base<T>& lhs) {
    typename Modular_Base<T>::Int val; is >> val;
    lhs.value = lhs.normalize(val);
    return is;
}
template<typename OStream, typename T>
OStream& operator<<(OStream& os, const Modular_Base<T>& rhs) { return os << rhs.value; }
using ModType = int;
constexpr ModType MOD = 1e9 + 7;
using Modular = Modular_Base<std::integral_constant<ModType, MOD>>;
// struct Dynamic_ModInt { static ModType value; };
// ModType &Mod = Dynamic_ModInt::value;
// using Modular = Modular_Base<Dynamic_ModInt>;
struct Fact {
    std::vector<Modular> fact, invfact;
    const int size;
    Fact(const int &n) : fact(n+1, Modular(1)), invfact(n+1), size(n) {
        fact[0] = 1;
        for (int i = 1; i <= n; i++) fact[i] = fact[i-1] * i;
        invfact[n] = Modular(1) / fact[n];
        for (int i = n; i >= 1; i--) invfact[i-1] = invfact[i] * i;
    }
    Modular C(const int &n, const int &m) const {
        if (n < 0 || m < 0 || n < m) return 0;
        if (n > size) throw std::out_of_range("Expected n < " + std::to_string(size) + ", but found n = " + std::to_string(n) + ".");
        return fact[n] * invfact[m] * invfact[n - m];
    }
    Modular A(const int &n, const int &m) const {
        if (n < 0 || m < 0 || n < m) return 0;
        if (n > size) throw std::out_of_range("Expected n < " + std::to_string(size) + ", but found n = " + std::to_string(n) + ".");
        return fact[n] * invfact[n - m];
    }
};
int main() {
    return 0;
}