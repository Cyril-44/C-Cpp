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
template<class T, class OP> concept BinaryOperation = requires (OP op, T x, T y) { {op(x,y)} -> std::convertible_to<T>; };
template<class T, class Omega, uint Bits>
requires (Bits <= 23u) &&
requires(Omega g, uint n) { { g(n) } -> std::convertible_to<T>; }
struct PolyBase {
    constexpr static uint N = 1u << Bits;
    inline void dft() { Cooley_Tukey_FFT(a, Omega{}, N, false); }
    inline void idft() { Cooley_Tukey_FFT(a, Omega{}, N, true); }
    PolyBase& operator+=(const PolyBase& x) { for (uint i = 0; i < N; i++) a[i] += x.a[i]; return *this; }
    PolyBase& operator-=(const PolyBase& x) { for (uint i = 0; i < N; i++) a[i] -= x.a[i]; return *this; }
    PolyBase& operator*=(const PolyBase& x) { for (uint i = 0; i < N; i++) a[i] *= x.a[i]; return *this; }
    PolyBase& operator/=(const PolyBase& x) { for (uint i = 0; i < N; i++) a[i] /= x.a[i]; return *this; }
    PolyBase& convolute(PolyBase &x) { dft(); x.dft(); *this *= x; idft(); x.idft(); return *this; }
    template<BinaryOperation<T> OP> PolyBase& oper(const PolyBase& x, OP op=OP{}) { for (uint i = 0; i < N; i++) a[i] = op(a[i], x.a[i]); return *this; }
    friend PolyBase operator+(PolyBase x, const PolyBase &y) { return x += y; }
    friend PolyBase operator-(PolyBase x, const PolyBase &y) { return x -= y; }
    friend PolyBase operator*(PolyBase x, const PolyBase &y) { return x *= y; }
    friend PolyBase operator/(PolyBase x, const PolyBase &y) { return x /= y; }
    friend PolyBase polyConvolute(PolyBase x, const PolyBase &y) { return x.convolute(y); }
    template<BinaryOperation<T> OP> friend PolyBase polyOper(PolyBase x, const PolyBase &y, OP op=OP{}) { return x.oper(y, op); }
    T& operator[](uint pos) { return a[pos]; }
    const T& operator[](uint pos) const { return a[pos]; }
private:
    const T InvN = T(1) / T(N);
    T a[N];
};
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
    template<class OS> friend OS& operator<<(OS& os, const FastComplex &x) {
        static constexpr double pow10[] = {1, 10, 100, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11};
        bool hasReal = std::abs(x.real) * (os.precision() < 12 ? pow10[os.precision()] : 1e12) >= .5,
             hasImag = std::abs(x.imag) * (os.precision() < 12 ? pow10[os.precision()] : 1e12) >= .5;
        if (hasReal && hasImag) {
            os << x.real;
            if (x.imag > 0) os << '+';
            os << x.imag << 'i';
        } else if (hasImag) {
            os << x.imag << 'i';
        } else os << x.real;
        return os;
    }
};

template<typename bb>concept IntegerWithI128=std::integral<bb>||std::same_as<bb,__int128_t>||std::same_as<bb,__uint128_t>;class FastInputStream{static constexpr size_t db=1<<20;char eb[db];char*fb=nullptr,*gb=nullptr;FILE*hb;public:FastInputStream(FILE*ib=stdin):hb(ib){}[[gnu::always_inline]]char get(){if(fb==gb){fb=eb;gb=eb+fread(eb,1,db,hb);if(fb==gb)return EOF;}return*fb++;}FastInputStream&operator>>(bool&kb){int lb=get();while(~lb&&lb!='0'&&lb!='1')lb=get();kb=(lb=='1');return*this;}FastInputStream&operator>>(char&mb){mb=get();while(mb==' '||mb=='\r'||mb=='\n'||mb=='\t')mb=get();return*this;}template<IntegerWithI128 nb>FastInputStream&operator>>(nb&ob){int pb=get();bool qb=false;while(~pb&&(pb<'0'||pb>'9')&&pb!='-')pb=get();if constexpr(std::is_signed_v<nb>||std::same_as<nb,__int128_t>){if(pb=='-'){qb=true;pb=get();}}ob=0;while(pb>='0'&&pb<='9'){ob=(ob<<3)+(ob<<1)+(pb^'0');pb=get();}if(qb)ob=-ob;return*this;}template<std::floating_point rb>FastInputStream&operator>>(rb&sb){int tb=get();bool ub=false;while(~tb&&(tb<'0'||tb>'9')&&tb!='-')tb=get();if(tb=='-'){ub=true;tb=get();}__uint128_t vb=0;while(tb>='0'&&tb<='9'){vb=(vb<<3)+(vb<<1)+(tb^'0');tb=get();}sb=static_cast<rb>(vb);if(tb=='.'){rb wb=1.0;for(tb=get();tb>='0'&&tb<='9';tb=get()){sb+=(tb^'0')*(wb/=10.0);}}if(ub)sb=-sb;return*this;}FastInputStream&operator>>(char*xb){int yb=get();while(~yb&&(yb<=' '))yb=get();while(~yb&&(yb>' '))*xb++=yb,yb=get();*xb='\0';return*this;}}fin;class FastOutputStream{static constexpr size_t Bb=1<<20;char Cb[Bb],*Db=Cb;FILE*Eb;long double Fb=5e-6;unsigned char Gb=6;public:FastOutputStream(FILE*Hb=stdout):Eb(Hb){setvbuf(Eb,nullptr,_IONBF,0);}~FastOutputStream(){flush();}void flush(){fwrite(Cb,1,Db-Cb,Eb);Db=Cb;}[[gnu::always_inline]]void put(char Kb){if(Db==Cb+Bb)flush();*Db++=Kb;}unsigned char precision()const{return Gb;}FastOutputStream&precision(int Mb){Gb=Mb;Fb=0.5;while(Mb--)Fb*=0.1;return*this;}FastOutputStream&operator<<(char Nb){put(Nb);return*this;}FastOutputStream&operator<<(const char*Ob){while(*Ob)put(*Ob++);return*this;}FastOutputStream&operator<<(const std::string&Pb){for(char c:Pb)put(c);return*this;}template<IntegerWithI128 Qb>FastOutputStream&operator<<(Qb Rb){if(Rb==0){put('0');return*this;}Qb Sb=Rb;if constexpr(std::is_signed_v<Qb>||std::same_as<Qb,__int128_t>){if(Sb<0){put('-');Sb=-Sb;}}static char Tb[64];int Ub=0;while(Sb){Tb[Ub++]=static_cast<char>(Sb%10)^'0';Sb/=10;}while(Ub)put(Tb[--Ub]);return*this;}template<std::floating_point Vb>FastOutputStream&operator<<(Vb Wb){Wb+=Fb;if(Wb<0){put('-');Wb=-Wb;}__uint128_t Xb=static_cast<__uint128_t>(Wb);*this<<Xb;Vb Yb=Wb-static_cast<Vb>(Xb);if(Gb>0){put('.');for(unsigned Zb=Gb;Zb;--Zb){Yb*=10;int _b=static_cast<int>(Yb);put(_b^'0');Yb-=_b;}}return*this;}}fout;
constexpr int N = 1000005;
int c[N*2];
char sa[N], sb[N];
int main() {
    Poly<21, FastComplex> a, b;
    int n, m;
    fin >> sa >> sb;
    n = strlen(sa) - 1, m = strlen(sb) - 1;
    for (int i = n; i >= 0; i--)
        a[i] = sa[n-i] - '0';
    if (fin.get() == '\r') fin.get();
    for (int j = m; j >= 0; j--)
        b[j] = sb[m-j] - '0';
    a.convolute(b);
    for (int i = 0; i <= n+m; i++) {
        c[i] += std::round(a[i].real);
        if (c[i] >= 10) c[i+1] += c[i] / 10, c[i] %= 10;
    }
    int p = n+m+1;
    while (p > 0 && c[p] == 0) --p;
    for (int i = p; i >= 0; i--)
        fout.put(c[i] | '0');
    fout.put('\n');
}