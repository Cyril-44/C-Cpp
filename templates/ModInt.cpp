#include <bits/c++config.h>
#include <bits/stdc++.h>
#include <type_traits>
template <typename T>
constexpr inline T modInv(T x, T y) { // x * u ≡ 1 (mod y)
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
requires std::integral<typename Mod::value_type> || 
requires(typename Mod::value_type a, typename Mod::value_type b) {
    // 1. 基础算术操作 (你原本定义的)
    { a + b } -> std::same_as<typename Mod::value_type>;
    { a - b } -> std::same_as<typename Mod::value_type>;
    { a * b } -> std::same_as<typename Mod::value_type>;
    { a % b } -> std::same_as<typename Mod::value_type>;
    // 2. 复合赋值操作 (+=, -=, *=, %=)
    { a += b } -> std::same_as<typename Mod::value_type&>;
    { a -= b } -> std::same_as<typename Mod::value_type&>;
    { a *= b } -> std::same_as<typename Mod::value_type&>;
    { a %= b } -> std::same_as<typename Mod::value_type&>;
    // 3. 构造与赋值约束 (确保 Default = 0 以及常规初始化能通过)
    requires std::is_default_constructible_v<typename Mod::value_type>;
    requires std::is_copy_constructible_v<typename Mod::value_type>;
    requires std::is_move_constructible_v<typename Mod::value_type>;
    requires std::is_assignable_v<typename Mod::value_type&, int>; // 确保能从 0 构造/赋值
    // 4. 比较操作 (==, !=)
    requires std::equality_comparable<typename Mod::value_type>;
} class MIBase {
    using Int = Mod::value_type;
    Int value;
    template<typename T> constexpr Int normalize(T x) {
        if constexpr (std::is_unsigned_v<T>)
            return static_cast<Int>(x < T(mod()) ? x : x % T(mod()));
        else {
            Int res = static_cast<Int>(-mod() < x && x < mod() ? x : x % mod());
            return (res < 0 ? res + mod() : res);
        }
    }
public:
    static constexpr Int mod() { return Mod::value; }
    constexpr MIBase() : value(Default) {}
    template<typename T> constexpr MIBase(const T &rhs) { value = normalize(rhs); }
    template<typename T> explicit constexpr operator T() const { return static_cast<T>(value); }
    constexpr Int operator()() const { return value; }
    constexpr MIBase& operator+=(const MIBase& rhs) {
        if ((value += rhs.value) >= mod()) value -= mod();
        return *this;
    }
    constexpr MIBase& operator-=(const MIBase& rhs) {
        if ((value -= rhs.value) < 0) value += mod();
        return *this;
    }
    constexpr MIBase& operator*=(const MIBase& rhs) {
        if constexpr (std::is_same_v<Int, int>)
            value = normalize((uint64_t)value * rhs.value);
        else if constexpr (std::is_same_v<Int, int64_t>)
            value = normalize((unsigned __int128)value * rhs.value);
        else 
            value = normalize(value * rhs.value);
        return *this;
    }
    constexpr MIBase& operator/=(const MIBase& rhs) { return *this *= MIBase(modInv(rhs.value, mod())); }
    template<std::integral T> constexpr MIBase& operator^=(T rhs) {
        if (rhs < 0) return *this = MIBase(modInv(static_cast<Int>(1), (*this ^ (-rhs))()));
        MIBase tmp = *this;
        for (*this = static_cast<Int>(1); rhs; rhs >>= 1) {
            if (rhs & 1) *this *= tmp;
            tmp *= tmp;
        }
        return *this;
    }
    constexpr MIBase operator-() const { return MIBase(-value); }
    constexpr MIBase& operator++() { return *this += 1; }
    constexpr MIBase& operator--() { return *this -= 1; }
    constexpr MIBase operator++(int) { MIBase tmp = *this; ++*this; return tmp; }
    constexpr MIBase operator--(int) { MIBase tmp = *this; --*this; return tmp; }
    constexpr MIBase operator+(const MIBase& rhs) const { return MIBase(*this) += rhs; }
    constexpr MIBase operator-(const MIBase& rhs) const { return MIBase(*this) -= rhs; }
    constexpr MIBase operator*(const MIBase& rhs) const { return MIBase(*this) *= rhs; }
    constexpr MIBase operator/(const MIBase& rhs) const { return MIBase(*this) /= rhs; }
    template<std::integral T> constexpr MIBase operator^(const T rhs) const { return MIBase(*this) ^= rhs; }
    constexpr bool operator==(const MIBase& rhs) const { return value == rhs.value; }
    constexpr bool operator!=(const MIBase& rhs) const { return value != rhs.value; }
    constexpr bool operator!() const { return !value; }
    template<typename U> constexpr friend MIBase operator+(const U& lhs, const MIBase& rhs) { return MIBase(lhs) + rhs; }
    template<typename U> constexpr friend MIBase operator-(const U& lhs, const MIBase& rhs) { return MIBase(lhs) - rhs; }
    template<typename U> constexpr friend MIBase operator*(const U& lhs, const MIBase& rhs) { return MIBase(lhs) * rhs; }
    template<typename U> constexpr friend MIBase operator/(const U& lhs, const MIBase& rhs) { return MIBase(lhs) / rhs; }
    template<typename U> constexpr friend MIBase operator==(const U& lhs, const MIBase& rhs) { return MIBase(lhs) == rhs; }
    template<typename U> constexpr friend MIBase operator!=(const U& lhs, const MIBase& rhs) { return MIBase(lhs) != rhs; }
    template<typename U> constexpr friend MIBase operator<=(const U& lhs, const MIBase& rhs) { return MIBase(lhs) <= rhs; }
    template<typename U> constexpr friend MIBase operator>=(const U& lhs, const MIBase& rhs) { return MIBase(lhs) >= rhs; }
    template<typename U> constexpr friend MIBase operator<(const U& lhs, const MIBase& rhs) { return MIBase(lhs) < rhs; }
    template<typename U> constexpr friend MIBase operator>(const U& lhs, const MIBase& rhs) { return MIBase(lhs) > rhs; }
    template<typename IStream> friend IStream& operator>>(IStream& is, MIBase& lhs) {
        is >> lhs.value;
        lhs.value = lhs.normalize(lhs.value);
        return is;
    }
    template<typename OStream> friend OStream& operator<<(OStream& os, const MIBase& rhs) { return os << rhs.value; }
};
constexpr auto MOD = (int)1e9 + 7;
using Mint = MIBase<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
// struct Dynamic_ModInt { using value_type = int; static value_type value; };
// Dynamic_ModInt::value_type &Mod = Dynamic_ModInt::value;
// using Mint = ModInt<Dynamic_ModInt>;
Mint sqrt(Mint x) { return x ^ (-2); }
struct Fact {
    Fact(const int n) : fact(n+1, Mint(1)), invfact(n+1), size(n) {
        fact[0] = 1;
        for (int i = 1; i <= n; i++) fact[i] = fact[i-1] * i;
        invfact[n] = Mint(1) / fact[n];
        for (int i = n; i >= 1; i--) invfact[i-1] = invfact[i] * i;
    }
    Mint C(int n, int m) const {
        if (n < 0 || m < 0 || n < m) [[unlikely]] return 0;
        if (n > size) [[unlikely]] throw std::out_of_range("Expected n < " + std::to_string(size) + ", but found n = " + std::to_string(n) + ".");
        return fact[n] * invfact[m] * invfact[n - m];
    }
    Mint A(int n, int m) const {
        if (n < 0 || m < 0 || n < m) [[unlikely]] return 0;
        if (n > size) [[unlikely]] throw std::out_of_range("Expected n < " + std::to_string(size) + ", but found n = " + std::to_string(n) + ".");
        return fact[n] * invfact[n - m];
    }
    Mint F(int n) const {
        if (n < 0) [[unlikely]] return 0;
        if (n > size) [[unlikely]] throw std::out_of_range("Expected n < " + std::to_string(size) + ", but found n = " + std::to_string(n) + ".");
        return fact[n];
    }
private:
    std::vector<Mint> fact, invfact;
    const int size;
} F(10000);
constexpr Mint M1 = 1, M2 = 3, M3 = M1 + M2, M4 = M3 / M2, M5 = M4 ^ 4, M6 = M5 * M2, M7 = 4/M3, M8 = 9*M6, M9 = M8/1145154*M6 ^ 4;
constexpr auto F1 = !M1;
int main() {
    1 + Mint(1);
    Mint abc = 1;
    abc += 2;
    M9();
    return 0;
}