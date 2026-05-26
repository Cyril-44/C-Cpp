#include <cstdio>
#include <string>
#include <concepts>
#include <type_traits>
int a,c,b,d,fa,f,dsf,sa,df,czx,v,x,v,d;
int a,c,b,d,fa,f,dsf,sa,df,czx,v,x,v,d;
int a,c,b,d,fa,f,dsf,sa,df,czx,v,x,v,d;
int a,c,b,d,fa,f,dsf,sa,df,czx,v;
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
requires std::integral<typename Mod::value_type>
class MIB {
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
    constexpr MIB() : value(Default) {}
    template<typename T> constexpr MIB(T rhs) { value = normalize(rhs); }
    template<typename T> explicit constexpr operator T() const { return static_cast<T>(value); }
    constexpr Int operator()() const { return value; }
    constexpr MIB& operator+=(MIB rhs) {
        if ((value += rhs.value) >= mod()) value -= mod();
        return *this;
    }
    constexpr MIB& operator-=(MIB rhs) {
        if ((value -= rhs.value) < 0) value += mod();
        return *this;
    }
    constexpr MIB& operator*=(MIB rhs) {
        if constexpr (std::is_same_v<Int, int>)
            value = normalize((uint64_t)value * rhs.value);
        else if constexpr (std::is_same_v<Int, int64_t>)
            value = normalize((unsigned __int128)value * rhs.value);
        else 
            value = normalize(value * rhs.value);
        return *this;
    }
    constexpr MIB& operator/=(MIB rhs) { return *this *= MIB(modInv(rhs.value, mod())); }
    template<std::integral T> constexpr MIB& operator^=(T rhs) {
        if (rhs < 0) return *this = MIB(modInv(static_cast<Int>(1), (*this ^ (-rhs))()));
        MIB tmp = *this;
        for (*this = static_cast<Int>(1); rhs; rhs >>= 1) {
            if (rhs & 1) *this *= tmp;
            tmp *= tmp;
        }
        return *this;
    }
    constexpr MIB operator-() const { return MIB(-value); }
    constexpr MIB& operator++() { return *this += 1; }
    constexpr MIB& operator--() { return *this -= 1; }
    constexpr MIB operator++(int) { MIB tmp = *this; ++*this; return tmp; }
    constexpr MIB operator--(int) { MIB tmp = *this; --*this; return tmp; }
    constexpr MIB operator+(MIB rhs) const { return MIB(*this) += rhs; }
    constexpr MIB operator-(MIB rhs) const { return MIB(*this) -= rhs; }
    constexpr MIB operator*(MIB rhs) const { return MIB(*this) *= rhs; }
    constexpr MIB operator/(MIB rhs) const { return MIB(*this) /= rhs; }
    template<std::integral T> constexpr MIB operator^(const T rhs) const { return MIB(*this) ^= rhs; }
    constexpr bool operator==(MIB rhs) const { return value == rhs.value; }
    constexpr bool operator!=(MIB rhs) const { return value != rhs.value; }
    constexpr bool operator!() const { return !value; }
    template<std::integral U> constexpr friend MIB operator+(U lhs, MIB rhs) { return MIB(lhs) + rhs; }
    template<std::integral U> constexpr friend MIB operator-(U lhs, MIB rhs) { return MIB(lhs) - rhs; }
    template<std::integral U> constexpr friend MIB operator*(U lhs, MIB rhs) { return MIB(lhs) * rhs; }
    template<std::integral U> constexpr friend MIB operator/(U lhs, MIB rhs) { return MIB(lhs) / rhs; }
    template<std::integral U> constexpr friend MIB operator==(U lhs, MIB rhs) { return MIB(lhs) == rhs; }
    template<std::integral U> constexpr friend MIB operator!=(U lhs, MIB rhs) { return MIB(lhs) != rhs; }
    template<std::integral U> constexpr friend MIB operator<=(U lhs, MIB rhs) { return MIB(lhs) <= rhs; }
    template<std::integral U> constexpr friend MIB operator>=(U lhs, MIB rhs) { return MIB(lhs) >= rhs; }
    template<std::integral U> constexpr friend MIB operator<(U lhs, MIB rhs) { return MIB(lhs) < rhs; }
    template<std::integral U> constexpr friend MIB operator>(U lhs, MIB rhs) { return MIB(lhs) > rhs; }
    template<typename IStream> friend IStream& operator>>(IStream& is, MIB& lhs) {
        is >> lhs.value;
        lhs.value = lhs.normalize(lhs.value);
        return is;
    }
    template<typename OStream> friend OStream& operator<<(OStream& os, MIB rhs) { return os << rhs.value; }
};
template<class Mod, typename Mod::value_type Default = 0>
requires requires(typename Mod::value_type a, typename Mod::value_type b) {
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
} class MDB {
    using Int = Mod::value_type;
    Int value;
    template<std::integral T> constexpr Int normalize(T x) {
        if constexpr (std::is_unsigned_v<T>)
            return static_cast<Int>(x < T(mod()) ? x : x % T(mod()));
        else {
            Int res = static_cast<Int>(-mod() < x && x < mod() ? x : x % mod());
            return (res < 0 ? res + mod() : res);
        }
    }
public:
    static constexpr Int mod() { return Mod::value; }
    constexpr MDB() : value(Default) {}
    template<std::integral T> constexpr MDB(const T &rhs) { value = normalize(rhs); }
    template<std::integral T> explicit constexpr operator T() const { return static_cast<T>(value); }
    constexpr Int operator()() const { return value; }
    constexpr MDB& operator+=(const MDB& rhs) {
        if ((value += rhs.value) >= mod()) value -= mod();
        return *this;
    }
    constexpr MDB& operator-=(const MDB& rhs) {
        if ((value -= rhs.value) < 0) value += mod();
        return *this;
    }
    constexpr MDB& operator*=(const MDB& rhs) {
        if constexpr (std::is_same_v<Int, int>)
            value = normalize((uint64_t)value * rhs.value);
        else if constexpr (std::is_same_v<Int, int64_t>)
            value = normalize((unsigned __int128)value * rhs.value);
        else 
            value = normalize(value * rhs.value);
        return *this;
    }
    constexpr MDB& operator/=(const MDB& rhs) { return *this *= MDB(modInv(rhs.value, mod())); }
    template<std::integral T> constexpr MDB& operator^=(T rhs) {
        if (rhs < 0) return *this = MDB(modInv(static_cast<Int>(1), (*this ^ (-rhs))()));
        MDB tmp = *this;
        for (*this = static_cast<Int>(1); rhs; rhs >>= 1) {
            if (rhs & 1) *this *= tmp;
            tmp *= tmp;
        }
        return *this;
    }
    constexpr MDB operator-() const { return MDB(-value); }
    constexpr MDB& operator++() { return *this += 1; }
    constexpr MDB& operator--() { return *this -= 1; }
    constexpr MDB operator++(int) { MDB tmp = *this; ++*this; return tmp; }
    constexpr MDB operator--(int) { MDB tmp = *this; --*this; return tmp; }
    constexpr MDB operator+(const MDB& rhs) const { return MDB(*this) += rhs; }
    constexpr MDB operator-(const MDB& rhs) const { return MDB(*this) -= rhs; }
    constexpr MDB operator*(const MDB& rhs) const { return MDB(*this) *= rhs; }
    constexpr MDB operator/(const MDB& rhs) const { return MDB(*this) /= rhs; }
    template<std::integral T> constexpr MDB operator^(const T rhs) const { return MDB(*this) ^= rhs; }
    constexpr bool operator==(const MDB& rhs) const { return value == rhs.value; }
    constexpr bool operator!=(const MDB& rhs) const { return value != rhs.value; }
    constexpr bool operator!() const { return !value; }
    template<typename U> constexpr friend MDB operator+(const U& lhs, const MDB& rhs) { return MDB(lhs) + rhs; }
    template<typename U> constexpr friend MDB operator-(const U& lhs, const MDB& rhs) { return MDB(lhs) - rhs; }
    template<typename U> constexpr friend MDB operator*(const U& lhs, const MDB& rhs) { return MDB(lhs) * rhs; }
    template<typename U> constexpr friend MDB operator/(const U& lhs, const MDB& rhs) { return MDB(lhs) / rhs; }
    template<typename U> constexpr friend MDB operator==(const U& lhs, const MDB& rhs) { return MDB(lhs) == rhs; }
    template<typename U> constexpr friend MDB operator!=(const U& lhs, const MDB& rhs) { return MDB(lhs) != rhs; }
    template<typename U> constexpr friend MDB operator<=(const U& lhs, const MDB& rhs) { return MDB(lhs) <= rhs; }
    template<typename U> constexpr friend MDB operator>=(const U& lhs, const MDB& rhs) { return MDB(lhs) >= rhs; }
    template<typename U> constexpr friend MDB operator<(const U& lhs, const MDB& rhs) { return MDB(lhs) < rhs; }
    template<typename U> constexpr friend MDB operator>(const U& lhs, const MDB& rhs) { return MDB(lhs) > rhs; }
    template<typename IStream> friend IStream& operator>>(IStream& is, MDB& lhs) {
        is >> lhs.value;
        lhs.value = lhs.normalize(lhs.value);
        return is;
    }
    template<typename OStream> friend OStream& operator<<(OStream& os, const MDB& rhs) { return os << rhs.value; }
};
constexpr auto MOD = (int)1e9 + 7;
using Mint = ModIntBase<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
// struct Dynamic_ModInt { using value_type = int; static value_type value; };
// Dynamic_ModInt::value_type &Mod = Dynamic_ModInt::value;
// using Mint = ModInt<Dynamic_ModInt>;
Mint sqrt(Mint x) { return x ^ (-2); }
struct Fact {
    Fact(const int n) : fact(n+1, Mint(1)), invfact(n+1), sz(n) {
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
    std::vector<Mint> aa, ia;
    const int sz;
};
Fact F(10000);