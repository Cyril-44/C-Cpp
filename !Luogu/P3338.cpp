#include <bits/stdc++.h>
using uint = unsigned;
inline constexpr uint32_t bitreverse(uint32_t x) {
    x = (x >> 16) | (x << 16);
    x = ((x & 0xFF00FF00u) >> 8) | ((x & 0x00FF00FFu) << 8);
    x = ((x & 0xF0F0F0F0u) >> 4) | ((x & 0x0F0F0F0Fu) << 4);
    x = ((x & 0xCCCCCCCCu) >> 2) | ((x & 0x33333333u) << 2);
    x = ((x & 0xAAAAAAAAu) >> 1) | ((x & 0x55555555u) << 1);
    return x;
}
template<class T, class Omega> inline void Cooley_Tukey_FFT(T *a, Omega omega, uint n, bool inv) {
    const int shift = 32 - __builtin_ctz(n);
    for (uint i = 0, rev; i < n; i++)
        if (i < (rev = bitreverse(i) >> shift)) std::swap(a[i], a[rev]);
    for (uint o = 2, k, i, j; o <= n; o <<= 1) {
        const T g = inv ? T(1) / omega(o) : omega(o); T w, x, y;
        for (k = 0; k < n; k += o)
            for (i = k, j = k+o/2, w = 1; j < k+o; w *= g, i++, j++)
                x=a[i], y=w*a[j], a[i]=x+y, a[j]=x-y;
    }
    if (inv) {
        const T invn = T(1) / T(n);
        for (uint i = 0; i < n; i++) a[i] *= invn;
    }
}
template<class T, class Omega, uint Bits>
requires (Bits <= 23u) &&
requires(Omega g, uint n) { { g(n) } -> std::convertible_to<T>; }
struct PolyBase {
    constexpr static uint N = 1u << Bits;
    inline void dft() { Cooley_Tukey_FFT(a, Omega{}, N, false); }
    inline void idft() { Cooley_Tukey_FFT(a, Omega{}, N, true); }
    template<class OP> requires requires(OP op, T x, T y) { {op(x, y)} -> std::convertible_to<T>; } 
    PolyBase& oper(const PolyBase& x, OP op=OP{}) { for (uint i = 0; i < N; i++) a[i] = op(a[i], x.a[i]); return *this; }
    T& operator[](uint pos) { return a[pos]; }
    const T& operator[](uint pos) const { return a[pos]; }
private:
    const T InvN = T(1) / T(N);
    T a[N];
};
struct PolyAdd { auto operator()(auto x, auto y) { return x + y; } };
struct PolySub { auto operator()(auto x, auto y) { return x - y; } };
struct PolyMul { auto operator()(auto x, auto y) { return x * y; } };
struct PolyDiv { auto operator()(auto x, auto y) { return x / y; } };
template<class T> struct ComplexOmega { T operator()(uint n) const { return {cos(2*M_PI / n), sin(2*M_PI / n)}; } };
template<uint N, class B=std::complex<double>> using Poly = PolyBase<B, ComplexOmega<B>, N>;

struct FastComplex {
    double real, imag;
    FastComplex() : real(), imag() {}
    FastComplex(double x) : real(x), imag(0) {}
    FastComplex(double x, double y) : real(x), imag(y) {}
    FastComplex& operator+=(const double val) { real += val; return *this; }
    FastComplex& operator-=(const double val) { real -= val; return *this; }
    FastComplex& operator*=(const double val) { real *= val, imag *= val; return *this; }
    FastComplex& operator/=(const double val) { real /= val, imag /= val; return *this; }
    FastComplex operator+(const double val) const { return FastComplex(real + val, imag); }
    FastComplex operator-(const double val) const { return FastComplex(real - val, imag); }
    FastComplex operator*(const double val) const { return FastComplex(real * val, imag * val); }
    FastComplex operator/(const double val) const { return FastComplex(real / val, imag / val); }
    FastComplex operator-() const { return FastComplex(-real, -imag); }
    FastComplex& operator+=(const FastComplex& rhs) { real += rhs.real, imag += rhs.imag; return *this; }
    FastComplex& operator-=(const FastComplex& rhs) { real -= rhs.real, imag -= rhs.imag; return *this; }
    FastComplex& operator*=(const FastComplex& rhs) { return *this = *this * rhs; }
    FastComplex& operator/=(const FastComplex& rhs) { return *this = *this / rhs; }
    friend FastComplex operator+(double lhs, const FastComplex& rhs) { return FastComplex(lhs + rhs.real, rhs.imag); }
    friend FastComplex operator-(double lhs, const FastComplex& rhs) { return FastComplex(lhs - rhs.real, -rhs.imag); }
    friend FastComplex operator*(double lhs, const FastComplex& rhs) { return FastComplex(lhs * rhs.real, lhs * rhs.imag); }
    friend FastComplex operator/(double lhs, const FastComplex& rhs) { 
        double deno = rhs.real * rhs.real + rhs.imag * rhs.imag;
        return FastComplex((lhs * rhs.real) / deno, (-lhs * rhs.imag) / deno);
    }
    friend FastComplex operator+(const FastComplex& a, const FastComplex& b) { return FastComplex(a.real + b.real, a.imag + b.imag); }
    friend FastComplex operator-(const FastComplex& a, const FastComplex& b) { return FastComplex(a.real - b.real, a.imag - b.imag); }
    friend FastComplex operator*(const FastComplex& a, const FastComplex& b) { return FastComplex(a.real * b.real - a.imag * b.imag, a.real * b.imag + a.imag * b.real); }
    friend FastComplex operator/(const FastComplex& a, const FastComplex& b) { double deno = b.real * b.real + b.imag * b.imag; return FastComplex((a.real * b.real + a.imag * b.imag) / deno, (a.imag * b.real - a.real * b.imag) / deno); }
};

constexpr int N = 100005;
double p[N];
int main() {
    Poly<18, FastComplex> a, b, c;
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) scanf("%lf", &p[i]);
    for (int i = 0; i < n; i++) a[i].real = p[i+1], b[i].real = p[n-i], c[i].real = 1. / ((i+1.) * (i+1.));
    a.dft(), b.dft(), c.dft();
    a.oper<PolyMul>(c), b.oper<PolyMul>(c);
    a.idft(), b.idft();
    for (int i = 1; i <= n; i++)
        printf("%.9lf\n", (i>1?a[i-2].real:0) - (i<n?b[n-i-1].real:0));
}