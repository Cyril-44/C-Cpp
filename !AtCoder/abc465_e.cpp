#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define Repv(c, ...) for (int tempFor_count = c, __VA_ARGS__; tempFor_count; --tempFor_count)
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
#define EXIT(s...) return (cout << s), void();
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

// #define MULTI_TEST_CASES

constexpr int N = 505, B = 1 << 10;
template<typename bb>constexpr inline bb modInv(bb cb,bb db){assert(cb!=0);bb eb=0,fb=1,gb=cb,hb=db,ib;while(gb!=0){ib=hb/gb;std::swap(gb,hb-=ib*gb);std::swap(eb-=ib*fb,fb);}assert(hb==1);return eb;}template<class kb,typename kb::value_type lb=0>requires std::integral<typename kb::value_type>class MIB{using mb=kb::value_type;mb nb;template<typename pb>constexpr mb ob(pb qb){if constexpr(std::is_unsigned_v<pb>)return static_cast<mb>(qb<pb(mod())?qb:qb%pb(mod()));else{mb sb=static_cast<mb>(-mod()<qb&&qb<mod()?qb:qb%mod());return(sb<0?sb+mod():sb);}}public:static constexpr mb mod(){return kb::value;}constexpr MIB():nb(lb){}template<typename tb>constexpr MIB(tb ub){nb=ob(ub);}template<typename vb>explicit constexpr operator vb()const{return static_cast<vb>(nb);}constexpr mb operator()()const{return nb;}constexpr MIB&operator+=(MIB wb){if((nb+=wb.nb)>=mod())nb-=mod();return*this;}constexpr MIB&operator-=(MIB xb){if((nb-=xb.nb)<0)nb+=mod();return*this;}constexpr MIB&operator*=(MIB yb){if constexpr(std::is_same_v<mb,int>)nb=ob((uint64_t)nb*yb.nb);else if constexpr(std::is_same_v<mb,int64_t>)nb=ob((unsigned __int128)nb*yb.nb);else nb=ob(nb*yb.nb);return*this;}constexpr MIB&operator/=(MIB zb){return*this*=MIB(modInv(zb.nb,mod()));}template<std::integral Ab>constexpr MIB&operator^=(Ab Bb){if(Bb<0)return*this=MIB(modInv(static_cast<mb>(1),(*this^(-Bb))()));MIB Cb=*this;for(*this=static_cast<mb>(1);Bb;Bb>>=1){if(Bb&1)*this*=Cb;Cb*=Cb;}return*this;}constexpr MIB operator-()const{return MIB(-nb);}constexpr MIB&operator++(){return*this+=1;}constexpr MIB&operator--(){return*this-=1;}constexpr MIB operator++(int){MIB Db=*this;++*this;return Db;}constexpr MIB operator--(int){MIB Eb=*this;--*this;return Eb;}constexpr MIB operator+(MIB Fb)const{return MIB(*this)+=Fb;}constexpr MIB operator-(MIB Gb)const{return MIB(*this)-=Gb;}constexpr MIB operator*(MIB Hb)const{return MIB(*this)*=Hb;}constexpr MIB operator/(MIB Ib)const{return MIB(*this)/=Ib;}template<std::integral Jb>constexpr MIB operator^(const Jb Kb)const{return MIB(*this)^=Kb;}constexpr bool operator==(MIB Lb)const{return nb==Lb.nb;}constexpr bool operator!=(MIB Mb)const{return nb!=Mb.nb;}constexpr bool operator!()const{return!nb;}template<std::integral Nb>constexpr friend MIB operator+(Nb Ob,MIB Pb){return MIB(Ob)+Pb;}template<std::integral Qb>constexpr friend MIB operator-(Qb Rb,MIB Sb){return MIB(Rb)-Sb;}template<std::integral Tb>constexpr friend MIB operator*(Tb Ub,MIB Vb){return MIB(Ub)*Vb;}template<std::integral Wb>constexpr friend MIB operator/(Wb Xb,MIB Yb){return MIB(Xb)/Yb;}template<std::integral Zb>constexpr friend MIB operator==(Zb _b,MIB ac){return MIB(_b)==ac;}template<std::integral bc>constexpr friend MIB operator!=(bc cc,MIB dc){return MIB(cc)!=dc;}template<std::integral ec>constexpr friend MIB operator<=(ec fc,MIB gc){return MIB(fc)<=gc;}template<std::integral hc>constexpr friend MIB operator>=(hc ic,MIB jc){return MIB(ic)>=jc;}template<std::integral kc>constexpr friend MIB operator<(kc lc,MIB mc){return MIB(lc)<mc;}template<std::integral nc>constexpr friend MIB operator>(nc oc,MIB pc){return MIB(oc)>pc;}template<typename qc>friend qc&operator>>(qc&rc,MIB&sc){rc>>sc.nb;sc.nb=sc.ob(sc.nb);return rc;}template<typename tc>friend tc&operator<<(tc&uc,MIB vc){return uc<<vc.nb;}};template<class xc,typename xc::value_type yc=0>requires requires(typename xc::value_type zc,typename xc::value_type Ac){{zc+Ac}->std::same_as<typename xc::value_type>;{zc-Ac}->std::same_as<typename xc::value_type>;{zc*Ac}->std::same_as<typename xc::value_type>;{zc%Ac}->std::same_as<typename xc::value_type>;{zc+=Ac}->std::same_as<typename xc::value_type&>;{zc-=Ac}->std::same_as<typename xc::value_type&>;{zc*=Ac}->std::same_as<typename xc::value_type&>;{zc%=Ac}->std::same_as<typename xc::value_type&>;requires std::is_default_constructible_v<typename xc::value_type>;requires std::is_copy_constructible_v<typename xc::value_type>;requires std::is_move_constructible_v<typename xc::value_type>;requires std::is_assignable_v<typename xc::value_type&,int>;requires std::equality_comparable<typename xc::value_type>;}class MDB{using Bc=xc::value_type;Bc Cc;template<std::integral Ec>constexpr Bc Dc(Ec Fc){if constexpr(std::is_unsigned_v<Ec>)return static_cast<Bc>(Fc<Ec(Gc())?Fc:Fc%Ec(Gc()));else{Bc Hc=static_cast<Bc>(-Gc()<Fc&&Fc<Gc()?Fc:Fc%Gc());return(Hc<0?Hc+Gc():Hc);}}public:static constexpr Bc Gc(){return xc::value;}constexpr MDB():Cc(yc){}template<std::integral Ic>constexpr MDB(const Ic&Jc){Cc=ob(Jc);}template<std::integral Kc>explicit constexpr operator Kc()const{return static_cast<Kc>(Cc);}constexpr Bc operator()()const{return Cc;}constexpr MDB&operator+=(const MDB&Lc){if((Cc+=Lc.Cc)>=Gc())Cc-=Gc();return*this;}constexpr MDB&operator-=(const MDB&Mc){if((Cc-=Mc.Cc)<0)Cc+=Gc();return*this;}constexpr MDB&operator*=(const MDB&Nc){if constexpr(std::is_same_v<Bc,int>)Cc=Dc((uint64_t)Cc*Nc.Cc);else if constexpr(std::is_same_v<Bc,int64_t>)Cc=Dc((unsigned __int128)Cc*Nc.Cc);else Cc=Dc(Cc*Nc.Cc);return*this;}constexpr MDB&operator/=(const MDB&Oc){return*this*=MDB(modInv(Oc.Cc,Gc()));}template<std::integral Pc>constexpr MDB&operator^=(Pc Qc){if(Qc<0)return*this=MDB(modInv(static_cast<Bc>(1),(*this^(-Qc))()));MDB Rc=*this;for(*this=static_cast<Bc>(1);Qc;Qc>>=1){if(Qc&1)*this*=Rc;Rc*=Rc;}return*this;}constexpr MDB operator-()const{return MDB(-Cc);}constexpr MDB&operator++(){return*this+=1;}constexpr MDB&operator--(){return*this-=1;}constexpr MDB operator++(int){MDB Sc=*this;++*this;return Sc;}constexpr MDB operator--(int){MDB Tc=*this;--*this;return Tc;}constexpr MDB operator+(const MDB&Uc)const{return MDB(*this)+=Uc;}constexpr MDB operator-(const MDB&Vc)const{return MDB(*this)-=Vc;}constexpr MDB operator*(const MDB&Wc)const{return MDB(*this)*=Wc;}constexpr MDB operator/(const MDB&Xc)const{return MDB(*this)/=Xc;}template<std::integral Yc>constexpr MDB operator^(const Yc Zc)const{return MDB(*this)^=Zc;}constexpr bool operator==(const MDB&_c)const{return Cc==_c.Cc;}constexpr bool operator!=(const MDB&ad)const{return Cc!=ad.Cc;}constexpr bool operator!()const{return!Cc;}template<typename bd>constexpr friend MDB operator+(const bd&cd,const MDB&dd){return MDB(cd)+dd;}template<typename ed>constexpr friend MDB operator-(const ed&fd,const MDB&gd){return MDB(fd)-gd;}template<typename hd>constexpr friend MDB operator*(const hd&id,const MDB&jd){return MDB(id)*jd;}template<typename kd>constexpr friend MDB operator/(const kd&ld,const MDB&md){return MDB(ld)/md;}template<typename nd>constexpr friend MDB operator==(const nd&od,const MDB&pd){return MDB(od)==pd;}template<typename qd>constexpr friend MDB operator!=(const qd&rd,const MDB&sd){return MDB(rd)!=sd;}template<typename td>constexpr friend MDB operator<=(const td&ud,const MDB&vd){return MDB(ud)<=vd;}template<typename wd>constexpr friend MDB operator>=(const wd&xd,const MDB&yd){return MDB(xd)>=yd;}template<typename zd>constexpr friend MDB operator<(const zd&Ad,const MDB&Bd){return MDB(Ad)<Bd;}template<typename Cd>constexpr friend MDB operator>(const Cd&Dd,const MDB&Ed){return MDB(Dd)>Ed;}template<typename Fd>friend Fd&operator>>(Fd&Gd,MDB&Hd){Gd>>Hd.Cc;Hd.Cc=Hd.Dc(Hd.Cc);return Gd;}template<typename Id>friend Id&operator<<(Id&Jd,const MDB&Kd){return Jd<<Kd.Cc;}};
constexpr auto MOD = 998244353;
using Mint = MIB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;

Mint f[2][B][3][2];
char str[N];
Mint calc(int n, char* str) {
    memset(f, 0, sizeof(f) >> 1);
    int fir = str[0] - '0';
    For(i, 1, fir-1) f[0][1<<i][i%3][0] = 1;
    f[0][1<<fir][fir%3][1] = 1;
    For(idx, 1, n-1) {
        auto fp = f[idx&1^1], fn = f[idx&1];
        int dig = str[idx] - '0';
        memset(fn, 0, sizeof(f) >> 1);
        For(i, 0, 9) For(s, 0, B-1) {
            int ns = s | 1<<i;
            For(m3, 0, 2) {
                int nm3 = (m3 + i) % 3;
                if (i < dig) {
                    fn[ns][nm3][0] += fp[s][m3][0] + fp[s][m3][1];
                } else if (i == dig) {
                    fn[ns][nm3][0] += fp[s][m3][0];
                    fn[ns][nm3][1] += fp[s][m3][1];
                } else {
                    fn[ns][nm3][0] += fp[s][m3][0];
                }
            }
        }
        For(i, 1, 9) fn[1<<i][i%3][0]++;
    }
    Mint ans;
    auto ff = f[n-1 & 1];
    For(s, 0, B-1) For(m, 0, 2) {
        if (Popcnt((Popcnt(s) == 3) << 2 | (m == 0) << 1 | (s>>3&1)) == 1)
            ans += ff[s][m][0] + ff[s][m][1];
    }
    return ans;
}
inline void solveSingleTestCase() {
    int n; cin >> str; n = strlen(str);
    cout << calc(n, str) << '\n';
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) solveSingleTestCase();
    return 0;
}