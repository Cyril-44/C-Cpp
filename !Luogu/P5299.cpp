#include <bits/stdc++.h>
constexpr int N = 1505, MOD = 998244353;
template<typename bb>constexpr inline bb modInv(bb cb,bb db){assert(cb!=0);bb eb=0,fb=1,gb=cb,hb=db,ib;while(gb!=0){ib=hb/gb;std::swap(gb,hb-=ib*gb);std::swap(eb-=ib*fb,fb);}assert(hb==1);return eb;}template<class kb,typename kb::value_type lb=0>requires std::integral<typename kb::value_type>class MIB{using mb=kb::value_type;mb nb;template<typename pb>constexpr mb ob(pb qb){if constexpr(std::is_unsigned_v<pb>)return static_cast<mb>(qb<pb(mod())?qb:qb%pb(mod()));else{mb sb=static_cast<mb>(-mod()<qb&&qb<mod()?qb:qb%mod());return(sb<0?sb+mod():sb);}}public:static constexpr mb mod(){return kb::value;}constexpr MIB():nb(lb){}template<typename tb>constexpr MIB(tb ub){nb=ob(ub);}template<typename vb>explicit constexpr operator vb()const{return static_cast<vb>(nb);}constexpr mb operator()()const{return nb;}constexpr MIB&operator+=(MIB wb){if((nb+=wb.nb)>=mod())nb-=mod();return*this;}constexpr MIB&operator-=(MIB xb){if((nb-=xb.nb)<0)nb+=mod();return*this;}constexpr MIB&operator*=(MIB yb){if constexpr(std::is_same_v<mb,int>)nb=ob((uint64_t)nb*yb.nb);else if constexpr(std::is_same_v<mb,int64_t>)nb=ob((unsigned __int128)nb*yb.nb);else nb=ob(nb*yb.nb);return*this;}constexpr MIB&operator/=(MIB zb){return*this*=MIB(modInv(zb.nb,mod()));}template<std::integral Ab>constexpr MIB&operator^=(Ab Bb){if(Bb<0)return*this=MIB(modInv(static_cast<mb>(1),(*this^(-Bb))()));MIB Cb=*this;for(*this=static_cast<mb>(1);Bb;Bb>>=1){if(Bb&1)*this*=Cb;Cb*=Cb;}return*this;}constexpr MIB operator-()const{return MIB(-nb);}constexpr MIB&operator++(){return*this+=1;}constexpr MIB&operator--(){return*this-=1;}constexpr MIB operator++(int){MIB Db=*this;++*this;return Db;}constexpr MIB operator--(int){MIB Eb=*this;--*this;return Eb;}constexpr MIB operator+(MIB Fb)const{return MIB(*this)+=Fb;}constexpr MIB operator-(MIB Gb)const{return MIB(*this)-=Gb;}constexpr MIB operator*(MIB Hb)const{return MIB(*this)*=Hb;}constexpr MIB operator/(MIB Ib)const{return MIB(*this)/=Ib;}template<std::integral Jb>constexpr MIB operator^(const Jb Kb)const{return MIB(*this)^=Kb;}constexpr bool operator==(MIB Lb)const{return nb==Lb.nb;}constexpr bool operator!=(MIB Mb)const{return nb!=Mb.nb;}constexpr bool operator!()const{return!nb;}template<std::integral Nb>constexpr friend MIB operator+(Nb Ob,MIB Pb){return MIB(Ob)+Pb;}template<std::integral Qb>constexpr friend MIB operator-(Qb Rb,MIB Sb){return MIB(Rb)-Sb;}template<std::integral Tb>constexpr friend MIB operator*(Tb Ub,MIB Vb){return MIB(Ub)*Vb;}template<std::integral Wb>constexpr friend MIB operator/(Wb Xb,MIB Yb){return MIB(Xb)/Yb;}template<std::integral Zb>constexpr friend MIB operator==(Zb _b,MIB ac){return MIB(_b)==ac;}template<std::integral bc>constexpr friend MIB operator!=(bc cc,MIB dc){return MIB(cc)!=dc;}template<std::integral ec>constexpr friend MIB operator<=(ec fc,MIB gc){return MIB(fc)<=gc;}template<std::integral hc>constexpr friend MIB operator>=(hc ic,MIB jc){return MIB(ic)>=jc;}template<std::integral kc>constexpr friend MIB operator<(kc lc,MIB mc){return MIB(lc)<mc;}template<std::integral nc>constexpr friend MIB operator>(nc oc,MIB pc){return MIB(oc)>pc;}template<typename qc>friend qc&operator>>(qc&rc,MIB&sc){rc>>sc.nb;sc.nb=sc.ob(sc.nb);return rc;}template<typename tc>friend tc&operator<<(tc&uc,MIB vc){return uc<<vc.nb;}};template<class xc,typename xc::value_type yc=0>requires requires(typename xc::value_type zc,typename xc::value_type Ac){{zc+Ac}->std::same_as<typename xc::value_type>;{zc-Ac}->std::same_as<typename xc::value_type>;{zc*Ac}->std::same_as<typename xc::value_type>;{zc%Ac}->std::same_as<typename xc::value_type>;{zc+=Ac}->std::same_as<typename xc::value_type&>;{zc-=Ac}->std::same_as<typename xc::value_type&>;{zc*=Ac}->std::same_as<typename xc::value_type&>;{zc%=Ac}->std::same_as<typename xc::value_type&>;requires std::is_default_constructible_v<typename xc::value_type>;requires std::is_copy_constructible_v<typename xc::value_type>;requires std::is_move_constructible_v<typename xc::value_type>;requires std::is_assignable_v<typename xc::value_type&,int>;requires std::equality_comparable<typename xc::value_type>;}class MDB{using Bc=xc::value_type;Bc Cc;template<std::integral Ec>constexpr Bc Dc(Ec Fc){if constexpr(std::is_unsigned_v<Ec>)return static_cast<Bc>(Fc<Ec(Gc())?Fc:Fc%Ec(Gc()));else{Bc Hc=static_cast<Bc>(-Gc()<Fc&&Fc<Gc()?Fc:Fc%Gc());return(Hc<0?Hc+Gc():Hc);}}public:static constexpr Bc Gc(){return xc::value;}constexpr MDB():Cc(yc){}template<std::integral Ic>constexpr MDB(const Ic&Jc){Cc=ob(Jc);}template<std::integral Kc>explicit constexpr operator Kc()const{return static_cast<Kc>(Cc);}constexpr Bc operator()()const{return Cc;}constexpr MDB&operator+=(const MDB&Lc){if((Cc+=Lc.Cc)>=Gc())Cc-=Gc();return*this;}constexpr MDB&operator-=(const MDB&Mc){if((Cc-=Mc.Cc)<0)Cc+=Gc();return*this;}constexpr MDB&operator*=(const MDB&Nc){if constexpr(std::is_same_v<Bc,int>)Cc=Dc((uint64_t)Cc*Nc.Cc);else if constexpr(std::is_same_v<Bc,int64_t>)Cc=Dc((unsigned __int128)Cc*Nc.Cc);else Cc=Dc(Cc*Nc.Cc);return*this;}constexpr MDB&operator/=(const MDB&Oc){return*this*=MDB(modInv(Oc.Cc,Gc()));}template<std::integral Pc>constexpr MDB&operator^=(Pc Qc){if(Qc<0)return*this=MDB(modInv(static_cast<Bc>(1),(*this^(-Qc))()));MDB Rc=*this;for(*this=static_cast<Bc>(1);Qc;Qc>>=1){if(Qc&1)*this*=Rc;Rc*=Rc;}return*this;}constexpr MDB operator-()const{return MDB(-Cc);}constexpr MDB&operator++(){return*this+=1;}constexpr MDB&operator--(){return*this-=1;}constexpr MDB operator++(int){MDB Sc=*this;++*this;return Sc;}constexpr MDB operator--(int){MDB Tc=*this;--*this;return Tc;}constexpr MDB operator+(const MDB&Uc)const{return MDB(*this)+=Uc;}constexpr MDB operator-(const MDB&Vc)const{return MDB(*this)-=Vc;}constexpr MDB operator*(const MDB&Wc)const{return MDB(*this)*=Wc;}constexpr MDB operator/(const MDB&Xc)const{return MDB(*this)/=Xc;}template<std::integral Yc>constexpr MDB operator^(const Yc Zc)const{return MDB(*this)^=Zc;}constexpr bool operator==(const MDB&_c)const{return Cc==_c.Cc;}constexpr bool operator!=(const MDB&ad)const{return Cc!=ad.Cc;}constexpr bool operator!()const{return!Cc;}template<typename bd>constexpr friend MDB operator+(const bd&cd,const MDB&dd){return MDB(cd)+dd;}template<typename ed>constexpr friend MDB operator-(const ed&fd,const MDB&gd){return MDB(fd)-gd;}template<typename hd>constexpr friend MDB operator*(const hd&id,const MDB&jd){return MDB(id)*jd;}template<typename kd>constexpr friend MDB operator/(const kd&ld,const MDB&md){return MDB(ld)/md;}template<typename nd>constexpr friend MDB operator==(const nd&od,const MDB&pd){return MDB(od)==pd;}template<typename qd>constexpr friend MDB operator!=(const qd&rd,const MDB&sd){return MDB(rd)!=sd;}template<typename td>constexpr friend MDB operator<=(const td&ud,const MDB&vd){return MDB(ud)<=vd;}template<typename wd>constexpr friend MDB operator>=(const wd&xd,const MDB&yd){return MDB(xd)>=yd;}template<typename zd>constexpr friend MDB operator<(const zd&Ad,const MDB&Bd){return MDB(Ad)<Bd;}template<typename Cd>constexpr friend MDB operator>(const Cd&Dd,const MDB&Ed){return MDB(Dd)>Ed;}template<typename Fd>friend Fd&operator>>(Fd&Gd,MDB&Hd){Gd>>Hd.Cc;Hd.Cc=Hd.Dc(Hd.Cc);return Gd;}template<typename Id>friend Id&operator<<(Id&Jd,const MDB&Kd){return Jd<<Kd.Cc;}};
using Mint = MIB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define roF(i, s, t) for (int i = (s); i >= (t); --i)
Mint C[N][N], fa[N][N], fb[N][N], gb[N][N], a[N], b[N];
/* 题意：给定 A[1..n] (A[i]>=2) 和 B[1..n]，求 2n 个数中所有选取 m 个数的方案中，挑选 k 个使得 prod_A[selected] * sum_B[selected] 最大。
显然有一定选取更多的 A，因为即便你 Ai 是 2, ... * 2 * Bmax >= ... * (Bmax + Bsemax)，由此可以看出选更多的 A 肯定是划算的
f[i][j] 表示考虑了 A[i] 的前 i 大，选了 j 个后，当前答案和为 f[i][j] 
*/
int main() {
#ifdef DEBUG
    freopen("P5299_testdata/4.in", "r", stdin);
    freopen("P5299.out", "w", stdout);
#endif
    int T, n, m, k;
    For(i, 0, N-1) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &k);
        For(i, 1, n) scanf("%d", a+i);
        For(i, 1, n) scanf("%d", b+i);
        std::sort(a+1, a+1+n, [](Mint x, Mint y) { return x() > y(); });
        std::sort(b+1, b+1+n, [](Mint x, Mint y) { return x() > y(); });
        {
            Mint fsum[N];
            fa[0][0] = 1;
            For(i, 1, n) {
                fa[i][1] = a[i];
                For(j, 2, i) {
                    fa[i][j] = fsum[j-1] * a[i];
                    fsum[j-1] += fa[i][j-1];
                }
                fsum[i] += fa[i][i];
            }
        }
        {
            Mint fsum[N], gsum[N];
            For(i, 1, n) {
                fb[i][1] = b[i], gb[i][1] = 1;
                For(j, 2, i) {
                    fb[i][j] = fsum[j-1] + gsum[j-1] * b[i];
                    gb[i][j] = gsum[j-1];
                    fsum[j-1] += fb[i][j-1];
                    gsum[j-1] += gb[i][j-1];
                }
                fsum[i] += fb[i][i];
                gsum[i] += gb[i][i];
            }
        }
        // For(i, 1, n) For(j, 1, i) printf("%d%c", fa[i][j], " \n"[j==i]);
        // For(i, 1, n) For(j, 1, i) printf("%d%c", fb[i][j], " \n"[j==i]);
        // For(i, 1, n) For(j, 1, i) printf("%d%c", gb[i][j], " \n"[j==i]);
        Mint ans;
        if (k-1 <= n) { // 计算：A 里面选 k-1 个最大的，B 选一个
            Mint ffa[N], ffb[N]; // A,B 选 i 个的答案和
            For(i, k-1, n) For(j, 0, n - i) ffa[k-1 + j] += fa[i][k-1] * C[n-i][j]/* , printf("ffa[%d] += fa[%d][%d] * (%d,%d)\n", k-1+j, i, k-1, n-i, j) */;
            For(i, 1, n) For(j, 0, n - i) ffb[1 + j] += fb[i][1] * C[n-i][j]; // 额外 j 个
            For(i, std::max(0, m-n), std::min(n, m-1)) ans += ffa[i] * ffb[m - i];
            // For(i, 1, n) printf("%d%c", ffa[i], " \n"[i==n]);
            // For(i, 1, n) printf("%d%c", ffb[i], " \n"[i==n]);
            // fprintf(stderr, "ans=%d\n", ans);
        }
        { // 计算：A 里面选 k-x 最大的，B 选 x(至少有2个有效) 个
            Mint ffa[N], ffb[N];
            For(x, 2, std::min(n, k)) {
                int target = m - (k-x);
                Mint ffa; For(i, k-x, n) ffa += fa[i][k-x];
                Mint ffb; For(i, x, n) if (target-x <= n-i) ffb += fb[i][x] * C[n-i][target - x];
                // fprintf(stderr, "A{%d} B{%d~%d} %d * %d\n", k-x, x, std::min(target, n), ffa, ffb);
                ans += ffa * ffb;
            }
        }
        printf("%d\n", ans);
        
    }
    return 0;
}
