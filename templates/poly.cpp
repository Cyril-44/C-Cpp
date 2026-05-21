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
    PolyBase& operator+=(const PolyBase& x) { for (uint i = 0; i < N; i++) a[i] += x.a[i]; return *this; }
    PolyBase& operator-=(const PolyBase& x) { for (uint i = 0; i < N; i++) a[i] -= x.a[i]; return *this; }
    PolyBase& operator*=(const PolyBase& x) { for (uint i = 0; i < N; i++) a[i] *= x.a[i]; return *this; }
    PolyBase& operator/=(const PolyBase& x) { for (uint i = 0; i < N; i++) a[i] /= x.a[i]; return *this; }
    friend PolyBase operator+(PolyBase x, const PolyBase &y) { return x += y; }
    friend PolyBase operator-(PolyBase x, const PolyBase &y) { return x -= y; }
    friend PolyBase operator*(PolyBase x, const PolyBase &y) { return x *= y; }
    friend PolyBase operator/(PolyBase x, const PolyBase &y) { return x /= y; }
    template<class OP> requires requires(OP op, T x, T y) { {op(x, y)} -> std::convertible_to<T>; }
    friend PolyBase polyOper()
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
};

template<typename ab>concept IntegerWithI128=std::integral<ab>||std::same_as<ab,__int128_t>||std::same_as<ab,__uint128_t>;class FastInputStream{static constexpr size_t cb=1<<20;char db[cb];char*eb=nullptr,*fb=nullptr;FILE*gb;public:FastInputStream(FILE*hb=stdin):gb(hb){}[[gnu::always_inline]]char ib(){if(eb==fb){eb=db;fb=db+fread(db,1,cb,gb);if(eb==fb)return EOF;}return*eb++;}FastInputStream&operator>>(bool&jb){int kb=ib();while(~kb&&kb!='0'&&kb!='1')kb=ib();jb=(kb=='1');return*this;}FastInputStream&operator>>(char&lb){lb=ib();while(lb==' '||lb=='\r'||lb=='\n'||lb=='\t')lb=ib();return*this;}template<IntegerWithI128 mb>FastInputStream&operator>>(mb&nb){int ob=ib();bool pb=false;while(~ob&&(ob<'0'||ob>'9')&&ob!='-')ob=ib();if constexpr(std::is_signed_v<mb>||std::same_as<mb,__int128_t>){if(ob=='-'){pb=true;ob=ib();}}nb=0;while(ob>='0'&&ob<='9'){nb=(nb<<3)+(nb<<1)+(ob^'0');ob=ib();}if(pb)nb=-nb;return*this;}template<std::floating_point qb>FastInputStream&operator>>(qb&rb){int sb=ib();bool tb=false;while(~sb&&(sb<'0'||sb>'9')&&sb!='-')sb=ib();if(sb=='-'){tb=true;sb=ib();}__uint128_t ub=0;while(sb>='0'&&sb<='9'){ub=(ub<<3)+(ub<<1)+(sb^'0');sb=ib();}rb=static_cast<qb>(ub);if(sb=='.'){qb vb=1.0;for(sb=ib();sb>='0'&&sb<='9';sb=ib()){rb+=(sb^'0')*(vb/=10.0);}}if(tb)rb=-rb;return*this;}FastInputStream&operator>>(char*wb){int xb=ib();while(~xb&&(xb<=' '))xb=ib();while(~xb&&(xb>' '))*wb++=xb,xb=ib();*wb='\0';return*this;}}fin;class FastOutputStream{static constexpr size_t Ab=1<<20;char Bb[Ab],*Cb=Bb;FILE*Db;int Eb=6;public:FastOutputStream(FILE*Fb=stdout):Db(Fb){setvbuf(Db,nullptr,_IONBF,0);}~FastOutputStream(){flush();}void flush(){fwrite(Bb,1,Cb-Bb,Db);Cb=Bb;}[[gnu::always_inline]]void put(char Ib){if(Cb==Bb+Ab)flush();*Cb++=Ib;}FastOutputStream&setprecision(int Kb){Eb=Kb;return*this;}FastOutputStream&operator<<(char Lb){put(Lb);return*this;}FastOutputStream&operator<<(const char*Mb){while(*Mb)put(*Mb++);return*this;}FastOutputStream&operator<<(const std::string&Nb){for(char c:Nb)put(c);return*this;}template<IntegerWithI128 Ob>FastOutputStream&operator<<(Ob Pb){if(Pb==0){put('0');return*this;}Ob Qb=Pb;if constexpr(std::is_signed_v<Ob>||std::same_as<Ob,__int128_t>){if(Qb<0){put('-');Qb=-Qb;}}static char Rb[64];int Sb=0;while(Qb){Rb[Sb++]=static_cast<char>(Qb%10)^'0';Qb/=10;}while(Sb)put(Rb[--Sb]);return*this;}template<std::floating_point Tb>FastOutputStream&operator<<(Tb Ub){if(Ub<0){put('-');Ub=-Ub;}__uint128_t Vb=static_cast<__uint128_t>(Ub);*this<<Vb;Tb Wb=Ub-static_cast<Tb>(Vb);if(Eb>0){put('.');int Xb=Eb;while(Xb--){Wb*=10;int Yb=static_cast<int>(Wb);put(Yb^'0');Wb-=Yb;}}return*this;}}fout;
template<typename bb>constexpr inline bb modInv(bb cb,bb db){assert(cb!=0);bb eb=0,fb=1,gb=cb,hb=db,ib;while(gb!=0){ib=hb/gb;std::swap(gb,hb-=ib*gb);std::swap(eb-=ib*fb,fb);}assert(hb==1);return eb;}template<class kb,typename kb::value_type lb=0>requires std::integral<typename kb::value_type>||requires(typename kb::value_type mb,typename kb::value_type nb){{mb+nb}->std::same_as<typename kb::value_type>;{mb-nb}->std::same_as<typename kb::value_type>;{mb*nb}->std::same_as<typename kb::value_type>;{mb%nb}->std::same_as<typename kb::value_type>;{mb+=nb}->std::same_as<typename kb::value_type&>;{mb-=nb}->std::same_as<typename kb::value_type&>;{mb*=nb}->std::same_as<typename kb::value_type&>;{mb%=nb}->std::same_as<typename kb::value_type&>;requires std::is_default_constructible_v<typename kb::value_type>;requires std::is_copy_constructible_v<typename kb::value_type>;requires std::is_move_constructible_v<typename kb::value_type>;requires std::is_assignable_v<typename kb::value_type&,int>;requires std::equality_comparable<typename kb::value_type>;}class MI{using ob=kb::value_type;ob qb;template<typename sb>constexpr ob rb(sb tb){if constexpr(std::is_unsigned_v<sb>)return static_cast<ob>(tb<sb(mod())?tb:tb%sb(mod()));else{ob ub=static_cast<ob>(-mod()<tb&&tb<mod()?tb:tb%mod());return(ub<0?ub+mod():ub);}}public:static constexpr ob mod(){return kb::value;}constexpr MI():qb(lb){}template<typename vb>constexpr MI(const vb&wb){qb=rb(wb);}template<typename xb>explicit constexpr operator xb()const{return static_cast<xb>(qb);}constexpr ob operator()()const{return qb;}constexpr MI&operator+=(const MI&yb){if((qb+=yb.qb)>=mod())qb-=mod();return*this;}constexpr MI&operator-=(const MI&zb){if((qb-=zb.qb)<0)qb+=mod();return*this;}constexpr MI&operator*=(const MI&Ab){if constexpr(std::is_same_v<ob,int>)qb=rb((uint64_t)qb*Ab.qb);else if constexpr(std::is_same_v<ob,int64_t>)qb=rb((unsigned __int128)qb*Ab.qb);else qb=rb(qb*Ab.qb);return*this;}constexpr MI&operator/=(const MI&Bb){return*this*=MI(modInv(Bb.qb,mod()));}template<std::integral Cb>constexpr MI&operator^=(Cb Db){if(Db<0)return*this=MI(modInv(static_cast<ob>(1),(*this^(-Db))()));MI Eb=*this;for(*this=static_cast<ob>(1);Db;Db>>=1){if(Db&1)*this*=Eb;Eb*=Eb;}return*this;}constexpr MI operator-()const{return MI(-qb);}constexpr MI&operator++(){return*this+=1;}constexpr MI&operator--(){return*this-=1;}constexpr MI operator++(int){MI Fb=*this;++*this;return Fb;}constexpr MI operator--(int){MI Gb=*this;--*this;return Gb;}constexpr MI operator+(const MI&Hb)const{return MI(*this)+=Hb;}constexpr MI operator-(const MI&Ib)const{return MI(*this)-=Ib;}constexpr MI operator*(const MI&Jb)const{return MI(*this)*=Jb;}constexpr MI operator/(const MI&Kb)const{return MI(*this)/=Kb;}template<std::integral Lb>constexpr MI operator^(const Lb Mb)const{return MI(*this)^=Mb;}constexpr bool operator==(const MI&Nb)const{return qb==Nb.qb;}constexpr bool operator!=(const MI&Ob)const{return qb!=Ob.qb;}constexpr bool operator!()const{return!qb;}template<typename Pb>constexpr friend MI operator+(const Pb&Qb,const MI&Rb){return MI(Qb)+Rb;}template<typename Sb>constexpr friend MI operator-(const Sb&Tb,const MI&Ub){return MI(Tb)-Ub;}template<typename Vb>constexpr friend MI operator*(const Vb&Wb,const MI&Xb){return MI(Wb)*Xb;}template<typename Yb>constexpr friend MI operator/(const Yb&Zb,const MI&_b){return MI(Zb)/_b;}template<typename ac>constexpr friend MI operator==(const ac&bc,const MI&cc){return MI(bc)==cc;}template<typename dc>constexpr friend MI operator!=(const dc&ec,const MI&fc){return MI(ec)!=fc;}template<typename gc>constexpr friend MI operator<=(const gc&hc,const MI&ic){return MI(hc)<=ic;}template<typename jc>constexpr friend MI operator>=(const jc&kc,const MI&lc){return MI(kc)>=lc;}template<typename mc>constexpr friend MI operator<(const mc&nc,const MI&oc){return MI(nc)<oc;}template<typename pc>constexpr friend MI operator>(const pc&qc,const MI&rc){return MI(qc)>rc;}template<typename sc>friend sc&operator>>(sc&tc,MI&uc){tc>>uc.qb;uc.qb=uc.qb(uc.qb);return tc;}template<typename vc>friend vc&operator<<(vc&wc,const MI&xc){return wc<<xc.qb;}};
constexpr auto MOD = 998244353;
using Mint = MI<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
// struct Dynamic_ModInt { using value_type = int; static value_type value; };
// Dynamic_ModInt::value_type &Mod = Dynamic_ModInt::value;
// using Mint = ModInt<Dynamic_ModInt>;
Mint sqrt(Mint x){return x^(-2);}struct Fact{Fact(const int Ec):Bc(Ec+1,Mint(1)),Cc(Ec+1),Dc(Ec){Bc[0]=1;for(int Fc=1;Fc<=Ec;Fc++)Bc[Fc]=Bc[Fc-1]*Fc;Cc[Ec]=Mint(1)/Bc[Ec];for(int Gc=Ec;Gc>=1;Gc--)Cc[Gc-1]=Cc[Gc]*Gc;}Mint C(int Ic,int Jc)const{if(Ic<0||Jc<0||Ic<Jc)[[unlikely]]return 0;if(Ic>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Ic)+".");return Bc[Ic]*Cc[Jc]*Cc[Ic-Jc];}Mint A(int Lc,int Mc)const{if(Lc<0||Mc<0||Lc<Mc)[[unlikely]]return 0;if(Lc>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Lc)+".");return Bc[Lc]*Cc[Lc-Mc];}Mint F(int Oc)const{if(Oc<0)[[unlikely]]return 0;if(Oc>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Oc)+".");return Bc[Oc];}private:std::vector<Mint>Bc,Cc;const int Dc;};
// Fact F(1000000);
template<int g> struct ModOmega { Mint operator()(uint n) const { return Mint(g) ^ ((Mint::mod() - 1) / n); } };
template<uint N, int g = 3> using MPoly = PolyBase<Mint, ModOmega<g>, N>;


int main() {
    // Poly<21, FastComplex> a, b;
    MPoly<21> a, b;
    int n, m;
    fin >> n >> m;
    for (int i = 0, v; i <= n; i++) {
        fin >> v;
        a[i] = v;
    }
    for (int i = 0, v; i <= m; i++) {
        fin >> v;
        b[i] = v;
    }
    a.dft(), b.dft();
    a.oper<PolyMul>(b);
    a.idft();
    fout.setprecision(0);
    for (int i = 0; i <= n+m; i++)
        fout << a[i] << ' ';
        // fout << a[i].real + .5 << ' ';
    fout << '\n';
}