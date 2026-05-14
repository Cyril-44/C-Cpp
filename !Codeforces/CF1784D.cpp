#include <bits/stdc++.h>
// #define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}

// #define MULTI_TEST_CASES

constexpr int N = 20;

template<typename bb>constexpr bb modInv(bb cb,bb db){assert(cb!=0);bb eb=0,fb=1,gb=cb,hb=db,ib;while(gb!=0){ib=hb/gb;std::swap(gb,hb-=ib*gb);std::swap(eb-=ib*fb,fb);}assert(hb==1);return eb;}template<class kb,typename kb::value_type lb=0>requires std::integral<typename kb::value_type>||requires(typename kb::value_type mb,typename kb::value_type nb){{mb+nb}->std::same_as<typename kb::value_type>;{mb-nb}->std::same_as<typename kb::value_type>;{mb*nb}->std::same_as<typename kb::value_type>;{mb%nb}->std::same_as<typename kb::value_type>;{mb+=nb}->std::same_as<typename kb::value_type&>;{mb-=nb}->std::same_as<typename kb::value_type&>;{mb*=nb}->std::same_as<typename kb::value_type&>;{mb%=nb}->std::same_as<typename kb::value_type&>;requires std::is_default_constructible_v<typename kb::value_type>;requires std::is_copy_constructible_v<typename kb::value_type>;requires std::is_move_constructible_v<typename kb::value_type>;requires std::is_assignable_v<typename kb::value_type&,int>;requires std::equality_comparable<typename kb::value_type>;}class MI{using ob=kb::value_type;inline static constexpr ob pb(){return kb::value;}ob qb;template<typename sb>inline constexpr ob rb(sb tb){if constexpr(std::is_unsigned_v<sb>)return static_cast<ob>(tb<pb()?tb:tb%pb());else{ob ub=static_cast<ob>(-pb()<tb&&tb<pb()?tb:tb%pb());return(ub<0?ub+pb():ub);}}public:constexpr inline MI():qb(lb){}template<typename vb>inline constexpr MI(const vb&wb){qb=rb(wb);}template<typename xb>inline explicit constexpr operator xb()const{return static_cast<xb>(qb);}inline constexpr ob operator()()const{return qb;}inline constexpr MI&operator+=(const MI&yb){if((qb+=yb.qb)>=pb())qb-=pb();return*this;}inline constexpr MI&operator-=(const MI&zb){if((qb-=zb.qb)<0)qb+=pb();return*this;}inline constexpr MI&operator*=(const MI&Ab){if constexpr(std::is_same_v<ob,int>)qb=rb((uint64_t)qb*Ab.qb);else if constexpr(std::is_same_v<ob,int64_t>)qb=rb((unsigned __int128)qb*Ab.qb);else qb=rb(qb*Ab.qb);return*this;}inline constexpr MI&operator/=(const MI&Bb){return*this*=MI(modInv(Bb.qb,pb()));}template<std::integral Cb>inline constexpr MI&operator^=(Cb Db){if(Db<0)return*this=MI(modInv(static_cast<ob>(1),(*this^(-Db))()));MI Eb=*this;for(*this=static_cast<ob>(1);Db;Db>>=1){if(Db&1)*this*=Eb;Eb*=Eb;}return*this;}inline constexpr MI operator-()const{return MI(-qb);}inline constexpr MI&operator++(){return*this+=1;}inline constexpr MI&operator--(){return*this-=1;}inline constexpr MI operator++(int){MI Fb=*this;++*this;return Fb;}inline constexpr MI operator--(int){MI Gb=*this;--*this;return Gb;}inline constexpr MI operator+(const MI&Hb)const{return MI(*this)+=Hb;}inline constexpr MI operator-(const MI&Ib)const{return MI(*this)-=Ib;}inline constexpr MI operator*(const MI&Jb)const{return MI(*this)*=Jb;}inline constexpr MI operator/(const MI&Kb)const{return MI(*this)/=Kb;}template<std::integral Lb>inline constexpr MI operator^(const Lb Mb)const{return MI(*this)^=Mb;}inline constexpr bool operator==(const MI&Nb)const{return qb==Nb.qb;}inline constexpr bool operator!=(const MI&Ob)const{return qb!=Ob.qb;}inline constexpr bool operator!()const{return!qb;}template<typename Pb>inline constexpr friend MI operator+(const Pb&Qb,const MI&Rb){return MI(Qb)+Rb;}template<typename Sb>inline constexpr friend MI operator-(const Sb&Tb,const MI&Ub){return MI(Tb)-Ub;}template<typename Vb>inline constexpr friend MI operator*(const Vb&Wb,const MI&Xb){return MI(Wb)*Xb;}template<typename Yb>inline constexpr friend MI operator/(const Yb&Zb,const MI&_b){return MI(Zb)/_b;}template<typename ac>inline constexpr friend MI operator==(const ac&bc,const MI&cc){return MI(bc)==cc;}template<typename dc>inline constexpr friend MI operator!=(const dc&ec,const MI&fc){return MI(ec)!=fc;}template<typename gc>inline constexpr friend MI operator<=(const gc&hc,const MI&ic){return MI(hc)<=ic;}template<typename jc>inline constexpr friend MI operator>=(const jc&kc,const MI&lc){return MI(kc)>=lc;}template<typename mc>inline constexpr friend MI operator<(const mc&nc,const MI&oc){return MI(nc)<oc;}template<typename pc>inline constexpr friend MI operator>(const pc&qc,const MI&rc){return MI(qc)>rc;}template<typename sc>inline friend sc&operator>>(sc&tc,MI&uc){tc>>uc.qb;uc.qb=uc.normalize(uc.qb);return tc;}template<typename vc>friend inline vc&operator<<(vc&wc,const MI&xc){return wc<<xc.qb;}};
constexpr auto MOD = 998244353;
using Mint = MI<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
// struct Dynamic_ModInt { using value_type = int; static value_type value; };
// Dynamic_ModInt::value_type &Mod = Dynamic_ModInt::value;
// using Mint = ModInt<Dynamic_ModInt>;
struct Fact{inline Fact(const int Ec):Bc(Ec+1,Mint(1)),Cc(Ec+1),Dc(Ec){Bc[0]=1;for(int Fc=1;Fc<=Ec;Fc++)Bc[Fc]=Bc[Fc-1]*Fc;Cc[Ec]=Mint(1)/Bc[Ec];for(int Gc=Ec;Gc>=1;Gc--)Cc[Gc-1]=Cc[Gc]*Gc;}inline Mint C(int Ic,int Jc)const{if(Ic<0||Jc<0||Ic<Jc)[[unlikely]]return 0;if(Ic>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Ic)+".");return Bc[Ic]*Cc[Jc]*Cc[Ic-Jc];}inline Mint A(int Lc,int Mc)const{if(Lc<0||Mc<0||Lc<Mc)[[unlikely]]return 0;if(Lc>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Lc)+".");return Bc[Lc]*Cc[Lc-Mc];}inline Mint F(int Oc)const{if(Oc<0)[[unlikely]]return 0;if(Oc>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Oc)+".");return Bc[Oc];}private:std::vector<Mint>Bc,Cc;const int Dc;};
Fact F(1 << N);

Mint f[2][(1<<N) + 1];
/* 对于一个已经固定下来的路径 1=a0 < a1 < a2 < a3 < ... < an，每一个 ai 都代表一个区域的最大值。
 *** 考虑知道这个之后如何去计算原来序列的方案数。
 * 从后往前分析。对于 an，有 2^n 种放置方法。an-1 一定是 an 的对手。
 * an-2 肯定是在与 an, an-1 组成的框互补的那个框里面。
 * 框里面其实相当于选择一些数 C(2^n - ai - 2^{n-i-1}, 2^{n-i-1} - 1)，然后再去排列，也就是 (2^{n-i-1})!。
 * 整体的其实也就出来了，全部给它乘起来就行。
 *** 考虑知道这个之后如何去计算所有的 a1..an 序列的 f 的和。
 * 设计 DP 状态 F(x,i) 表示 ai = x 的时候只考虑 1 < a1 < a2 < ... < ai 的 a_{1..i} 在 f 中的贡献的和。
 * F(x,i) = f_i(x) * \sum_{y=1}^{x-1} F(y,i-1)，其中 f_i(x) 表示 a_i 在 f 中的贡献（就是上面两个式子的乘积）。
 *** 注意设置初始值的时候，f(\{\}) 本身就有一个 2^n 的贡献。这意味着应该设置 F(1,0)=2^n * f_i(1,0)。
 * 注意要使用前缀和优化哦。
 */
inline void solveSingleTestCase() {
    int n;
    cin >> n;
    auto func = [&](int x, int i) { return F.C((1<<n) - x - (1<<n-i-1), (1<<n-i-1) - 1) * F.F(1<<n-i-1); };
    f[0][1] = (1<<n) * func(1, 0);
    For (i, 1, n-1) {
        For (x, 1, 1<<n) f[i&1^1][x] += f[i&1^1][x-1];
        memset(f[i&1], 0, sizeof(*f[i&1]) * (i + 1));
        For (x, i+1, 1<<n) f[i&1][x] = f[i&1^1][x - 1] * func(x, i);
    }
    For (x, 1, 1<<n) // 最后一轮实际上位置已经确定了，只要做一个前缀和统计。
        f[n&1][x] = f[n&1^1][x-1] + f[n&1][x-1];
    For (i, 1, 1 << n)
        cout << f[n&1][i] << '\n';
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