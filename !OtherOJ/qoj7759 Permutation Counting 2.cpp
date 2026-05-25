#include <bits/stdc++.h>
constexpr int N = 505;
template<typename bb>constexpr inline bb modInv(bb cb,bb db){assert(cb!=0);bb eb=0,fb=1,gb=cb,hb=db,ib;while(gb!=0){ib=hb/gb;std::swap(gb,hb-=ib*gb);std::swap(eb-=ib*fb,fb);}assert(hb==1);return eb;}template<class kb,typename kb::value_type lb=0>requires std::integral<typename kb::value_type>||requires(typename kb::value_type mb,typename kb::value_type nb){{mb+nb}->std::same_as<typename kb::value_type>;{mb-nb}->std::same_as<typename kb::value_type>;{mb*nb}->std::same_as<typename kb::value_type>;{mb%nb}->std::same_as<typename kb::value_type>;{mb+=nb}->std::same_as<typename kb::value_type&>;{mb-=nb}->std::same_as<typename kb::value_type&>;{mb*=nb}->std::same_as<typename kb::value_type&>;{mb%=nb}->std::same_as<typename kb::value_type&>;requires std::is_default_constructible_v<typename kb::value_type>;requires std::is_copy_constructible_v<typename kb::value_type>;requires std::is_move_constructible_v<typename kb::value_type>;requires std::is_assignable_v<typename kb::value_type&,int>;requires std::equality_comparable<typename kb::value_type>;}class MI{using ob=kb::value_type;ob qb;template<typename sb>constexpr ob rb(sb tb){if constexpr(std::is_unsigned_v<sb>)return static_cast<ob>(tb<sb(mod())?tb:tb%sb(mod()));else{ob ub=static_cast<ob>(-mod()<tb&&tb<mod()?tb:tb%mod());return(ub<0?ub+mod():ub);}}public:static constexpr ob mod(){return kb::value;}constexpr MI():qb(lb){}template<typename vb>constexpr MI(const vb&wb){qb=rb(wb);}template<typename xb>explicit constexpr operator xb()const{return static_cast<xb>(qb);}constexpr ob operator()()const{return qb;}constexpr MI&operator+=(const MI&yb){if((qb+=yb.qb)>=mod())qb-=mod();return*this;}constexpr MI&operator-=(const MI&zb){if((qb-=zb.qb)<0)qb+=mod();return*this;}constexpr MI&operator*=(const MI&Ab){if constexpr(std::is_same_v<ob,int>)qb=rb((uint64_t)qb*Ab.qb);else if constexpr(std::is_same_v<ob,int64_t>)qb=rb((unsigned __int128)qb*Ab.qb);else qb=rb(qb*Ab.qb);return*this;}constexpr MI&operator/=(const MI&Bb){return*this*=MI(modInv(Bb.qb,mod()));}template<std::integral Cb>constexpr MI&operator^=(Cb Db){if(Db<0)return*this=MI(modInv(static_cast<ob>(1),(*this^(-Db))()));MI Eb=*this;for(*this=static_cast<ob>(1);Db;Db>>=1){if(Db&1)*this*=Eb;Eb*=Eb;}return*this;}constexpr MI operator-()const{return MI(-qb);}constexpr MI&operator++(){return*this+=1;}constexpr MI&operator--(){return*this-=1;}constexpr MI operator++(int){MI Fb=*this;++*this;return Fb;}constexpr MI operator--(int){MI Gb=*this;--*this;return Gb;}constexpr MI operator+(const MI&Hb)const{return MI(*this)+=Hb;}constexpr MI operator-(const MI&Ib)const{return MI(*this)-=Ib;}constexpr MI operator*(const MI&Jb)const{return MI(*this)*=Jb;}constexpr MI operator/(const MI&Kb)const{return MI(*this)/=Kb;}template<std::integral Lb>constexpr MI operator^(const Lb Mb)const{return MI(*this)^=Mb;}constexpr bool operator==(const MI&Nb)const{return qb==Nb.qb;}constexpr bool operator!=(const MI&Ob)const{return qb!=Ob.qb;}constexpr bool operator!()const{return!qb;}template<typename Pb>constexpr friend MI operator+(const Pb&Qb,const MI&Rb){return MI(Qb)+Rb;}template<typename Sb>constexpr friend MI operator-(const Sb&Tb,const MI&Ub){return MI(Tb)-Ub;}template<typename Vb>constexpr friend MI operator*(const Vb&Wb,const MI&Xb){return MI(Wb)*Xb;}template<typename Yb>constexpr friend MI operator/(const Yb&Zb,const MI&_b){return MI(Zb)/_b;}template<typename ac>constexpr friend MI operator==(const ac&bc,const MI&cc){return MI(bc)==cc;}template<typename dc>constexpr friend MI operator!=(const dc&ec,const MI&fc){return MI(ec)!=fc;}template<typename gc>constexpr friend MI operator<=(const gc&hc,const MI&ic){return MI(hc)<=ic;}template<typename jc>constexpr friend MI operator>=(const jc&kc,const MI&lc){return MI(kc)>=lc;}template<typename mc>constexpr friend MI operator<(const mc&nc,const MI&oc){return MI(nc)<oc;}template<typename pc>constexpr friend MI operator>(const pc&qc,const MI&rc){return MI(qc)>rc;}template<typename sc>friend sc&operator>>(sc&tc,MI&uc){tc>>uc.qb;uc.qb=uc.qb(uc.qb);return tc;}template<typename vc>friend vc&operator<<(vc&wc,const MI&xc){return wc<<xc.qb;}};
// constexpr auto MOD = (int)1e9 + 7;
// using Mint = MI<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
struct Dynamic_ModInt { using value_type = int; static value_type value; };
Dynamic_ModInt::value_type Dynamic_ModInt::value = (int)1e9 + 7;
Dynamic_ModInt::value_type &Mod = Dynamic_ModInt::value;
using Mint = MI<Dynamic_ModInt>;
Mint sqrt(Mint x){return x^(-2);}struct Fact{Fact(const int Ec):Bc(Ec+1,Mint(1)),Cc(Ec+1),Dc(Ec){Bc[0]=1;for(int Fc=1;Fc<=Ec;Fc++)Bc[Fc]=Bc[Fc-1]*Fc;Cc[Ec]=Mint(1)/Bc[Ec];for(int Gc=Ec;Gc>=1;Gc--)Cc[Gc-1]=Cc[Gc]*Gc;}Mint C(int Ic,int Jc)const{if(Ic<0||Jc<0||Ic<Jc)[[unlikely]]return 0;if(Ic>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Ic)+".");return Bc[Ic]*Cc[Jc]*Cc[Ic-Jc];}Mint A(int Lc,int Mc)const{if(Lc<0||Mc<0||Lc<Mc)[[unlikely]]return 0;if(Lc>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Lc)+".");return Bc[Lc]*Cc[Lc-Mc];}Mint F(int Oc)const{if(Oc<0)[[unlikely]]return 0;if(Oc>Dc)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(Dc)+", but found n = "+std::to_string(Oc)+".");return Bc[Oc];}private:std::vector<Mint>Bc,Cc;const int Dc;};


Mint C[N][N], f[N][N];
#define For(i, s, t) for (int i = int(s); i <= (t); i++)
#define roF(i, s, t) for (int i = int(s); i >= (t); i--)
/*
经典 Observation：x 个相邻上升 <==> n-x 个上升段
显然有 p^{-1}(x) = pos(x)，即 x 在排列中的位置
现在弱化题目，只求 Ans_{x,y}
我们考虑一组一组手玩 p^{-1}0
具体来说每次加入一组上升 p^{-1}，往 p 里面塞。注意到 p^{-1} 的一组里面 pos 是增的，所以在 p 里面塞的位置也是增的
因为我们是从小往大塞的，所以 p 于被分成 y 上升段，每一段里面的数属于的组号肯定是不减的
我们不妨用一个矩阵来表示这个玩意，A_i_j 表示在 p^{-1} 的第i组 往 p 的第j组塞了 A_{i,j} 个数
这种矩阵显然可以唯一描述一种手玩操作，也就是这种矩阵和方案构成双射。
但是！但是！这里的 A_{x,y} 是钦定的，也就是说，我们事实上钦定了 x 个 p 上升段，y 个 p^{-1} 上升段，也就是说实际上没有这么多。可能我们把 1 3 | 5 7 这样分了。
所以，我们事实上要求出所有的 A_{x,y} 然后再去容斥得到答案。

接下来就是对这种矩阵计数了。
注意到 A_{x,y} 满足以下条件：
1. 每一列和不为 0 （不能有空的 p 组）
2. 每一行和不为 0 （不能有空的 p^{-1} 组）
先钦定(至多)有p列和q行和不为 0
这样的方案数就是 p*q 矩阵放 n 个数，也就是插板法 C(n+p*q-1, p*q-1) = C(n+p*q-1, n)
然后做一个容斥就行了
*/
int main() {
    int n;
    scanf("%d%d", &n, &Mod);
    Fact F(N*N);
    For(i, 1, n) For(j, C[i][0] = C[i][i] = 1, i-1)
        C[i][j] = C[i-1][j-1] + C[i-1][j];
    For(i, 1, n) For(j, 1, n) f[i][j] = F.C(n + i*j - 1, n);
    For(i, 1, n) For(j, 2, n) For(k, 1, j-1)
        f[i][j] -= f[i][k] * C[j][k]; // 在 j 行里面选择 k 行，这 k 行都是非空的，其他行都是空的，那这些方案就是非法的
    For(j, 1, n) For(i, 2, n) For(k, 1, i-1)
        f[i][j] -= f[k][j] * C[i][k];
    For(i, 1, n) For(j, 2, n) For(k, 1, j-1)
        f[i][j] -= f[i][k] * C[n-k][n-j]; // 在 n-k 个拆分点中选 n-j 个拆分点，把合法的拆分开，那这些方案就是非法的
    For(j, 1, n) For(i, 2, n) For(k, 1, i-1)
        f[i][j] -= f[k][j] * C[n-k][n-i];
    roF(i, n, 1) roF(j, n, 1) printf("%d%c", f[i][j](), " \n"[j==1]);
    return 0;
}
