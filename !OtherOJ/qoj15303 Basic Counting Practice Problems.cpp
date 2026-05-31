#include <bits/stdc++.h>
template<typename bb>constexpr inline bb modInv(bb cb,bb db){assert(cb!=0);bb eb=0,fb=1,gb=cb,hb=db,ib;while(gb!=0){ib=hb/gb;std::swap(gb,hb-=ib*gb);std::swap(eb-=ib*fb,fb);}assert(hb==1);return eb;}template<class kb,typename kb::value_type lb=0>requires std::integral<typename kb::value_type>class MIB{using mb=kb::value_type;mb nb;template<typename pb>constexpr mb ob(pb qb){if constexpr(std::is_unsigned_v<pb>)return static_cast<mb>(qb<pb(mod())?qb:qb%pb(mod()));else{mb sb=static_cast<mb>(-mod()<qb&&qb<mod()?qb:qb%mod());return(sb<0?sb+mod():sb);}}public:static constexpr mb mod(){return kb::value;}constexpr MIB():nb(lb){}template<typename tb>constexpr MIB(tb ub){nb=ob(ub);}template<typename vb>explicit constexpr operator vb()const{return static_cast<vb>(nb);}constexpr mb operator()()const{return nb;}constexpr MIB&operator+=(MIB wb){if((nb+=wb.nb)>=mod())nb-=mod();return*this;}constexpr MIB&operator-=(MIB xb){if((nb-=xb.nb)<0)nb+=mod();return*this;}constexpr MIB&operator*=(MIB yb){if constexpr(std::is_same_v<mb,int>)nb=ob((uint64_t)nb*yb.nb);else if constexpr(std::is_same_v<mb,int64_t>)nb=ob((unsigned __int128)nb*yb.nb);else nb=ob(nb*yb.nb);return*this;}constexpr MIB&operator/=(MIB zb){return*this*=MIB(modInv(zb.nb,mod()));}template<std::integral Ab>constexpr MIB&operator^=(Ab Bb){if(Bb<0)return*this=MIB(modInv(static_cast<mb>(1),(*this^(-Bb))()));MIB Cb=*this;for(*this=static_cast<mb>(1);Bb;Bb>>=1){if(Bb&1)*this*=Cb;Cb*=Cb;}return*this;}constexpr MIB operator-()const{return MIB(-nb);}constexpr MIB&operator++(){return*this+=1;}constexpr MIB&operator--(){return*this-=1;}constexpr MIB operator++(int){MIB Db=*this;++*this;return Db;}constexpr MIB operator--(int){MIB Eb=*this;--*this;return Eb;}constexpr MIB operator+(MIB Fb)const{return MIB(*this)+=Fb;}constexpr MIB operator-(MIB Gb)const{return MIB(*this)-=Gb;}constexpr MIB operator*(MIB Hb)const{return MIB(*this)*=Hb;}constexpr MIB operator/(MIB Ib)const{return MIB(*this)/=Ib;}template<std::integral Jb>constexpr MIB operator^(const Jb Kb)const{return MIB(*this)^=Kb;}constexpr bool operator==(MIB Lb)const{return nb==Lb.nb;}constexpr bool operator!=(MIB Mb)const{return nb!=Mb.nb;}constexpr bool operator!()const{return!nb;}template<std::integral Nb>constexpr friend MIB operator+(Nb Ob,MIB Pb){return MIB(Ob)+Pb;}template<std::integral Qb>constexpr friend MIB operator-(Qb Rb,MIB Sb){return MIB(Rb)-Sb;}template<std::integral Tb>constexpr friend MIB operator*(Tb Ub,MIB Vb){return MIB(Ub)*Vb;}template<std::integral Wb>constexpr friend MIB operator/(Wb Xb,MIB Yb){return MIB(Xb)/Yb;}template<std::integral Zb>constexpr friend MIB operator==(Zb _b,MIB ac){return MIB(_b)==ac;}template<std::integral bc>constexpr friend MIB operator!=(bc cc,MIB dc){return MIB(cc)!=dc;}template<std::integral ec>constexpr friend MIB operator<=(ec fc,MIB gc){return MIB(fc)<=gc;}template<std::integral hc>constexpr friend MIB operator>=(hc ic,MIB jc){return MIB(ic)>=jc;}template<std::integral kc>constexpr friend MIB operator<(kc lc,MIB mc){return MIB(lc)<mc;}template<std::integral nc>constexpr friend MIB operator>(nc oc,MIB pc){return MIB(oc)>pc;}template<typename qc>friend qc&operator>>(qc&rc,MIB&sc){rc>>sc.nb;sc.nb=sc.ob(sc.nb);return rc;}template<typename tc>friend tc&operator<<(tc&uc,MIB vc){return uc<<vc.nb;}};template<class xc,typename xc::value_type yc=0>requires requires(typename xc::value_type zc,typename xc::value_type Ac){{zc+Ac}->std::same_as<typename xc::value_type>;{zc-Ac}->std::same_as<typename xc::value_type>;{zc*Ac}->std::same_as<typename xc::value_type>;{zc%Ac}->std::same_as<typename xc::value_type>;{zc+=Ac}->std::same_as<typename xc::value_type&>;{zc-=Ac}->std::same_as<typename xc::value_type&>;{zc*=Ac}->std::same_as<typename xc::value_type&>;{zc%=Ac}->std::same_as<typename xc::value_type&>;requires std::is_default_constructible_v<typename xc::value_type>;requires std::is_copy_constructible_v<typename xc::value_type>;requires std::is_move_constructible_v<typename xc::value_type>;requires std::is_assignable_v<typename xc::value_type&,int>;requires std::equality_comparable<typename xc::value_type>;}class MDB{using Bc=xc::value_type;Bc Cc;template<std::integral Ec>constexpr Bc Dc(Ec Fc){if constexpr(std::is_unsigned_v<Ec>)return static_cast<Bc>(Fc<Ec(Gc())?Fc:Fc%Ec(Gc()));else{Bc Hc=static_cast<Bc>(-Gc()<Fc&&Fc<Gc()?Fc:Fc%Gc());return(Hc<0?Hc+Gc():Hc);}}public:static constexpr Bc Gc(){return xc::value;}constexpr MDB():Cc(yc){}template<std::integral Ic>constexpr MDB(const Ic&Jc){Cc=ob(Jc);}template<std::integral Kc>explicit constexpr operator Kc()const{return static_cast<Kc>(Cc);}constexpr Bc operator()()const{return Cc;}constexpr MDB&operator+=(const MDB&Lc){if((Cc+=Lc.Cc)>=Gc())Cc-=Gc();return*this;}constexpr MDB&operator-=(const MDB&Mc){if((Cc-=Mc.Cc)<0)Cc+=Gc();return*this;}constexpr MDB&operator*=(const MDB&Nc){if constexpr(std::is_same_v<Bc,int>)Cc=Dc((uint64_t)Cc*Nc.Cc);else if constexpr(std::is_same_v<Bc,int64_t>)Cc=Dc((unsigned __int128)Cc*Nc.Cc);else Cc=Dc(Cc*Nc.Cc);return*this;}constexpr MDB&operator/=(const MDB&Oc){return*this*=MDB(modInv(Oc.Cc,Gc()));}template<std::integral Pc>constexpr MDB&operator^=(Pc Qc){if(Qc<0)return*this=MDB(modInv(static_cast<Bc>(1),(*this^(-Qc))()));MDB Rc=*this;for(*this=static_cast<Bc>(1);Qc;Qc>>=1){if(Qc&1)*this*=Rc;Rc*=Rc;}return*this;}constexpr MDB operator-()const{return MDB(-Cc);}constexpr MDB&operator++(){return*this+=1;}constexpr MDB&operator--(){return*this-=1;}constexpr MDB operator++(int){MDB Sc=*this;++*this;return Sc;}constexpr MDB operator--(int){MDB Tc=*this;--*this;return Tc;}constexpr MDB operator+(const MDB&Uc)const{return MDB(*this)+=Uc;}constexpr MDB operator-(const MDB&Vc)const{return MDB(*this)-=Vc;}constexpr MDB operator*(const MDB&Wc)const{return MDB(*this)*=Wc;}constexpr MDB operator/(const MDB&Xc)const{return MDB(*this)/=Xc;}template<std::integral Yc>constexpr MDB operator^(const Yc Zc)const{return MDB(*this)^=Zc;}constexpr bool operator==(const MDB&_c)const{return Cc==_c.Cc;}constexpr bool operator!=(const MDB&ad)const{return Cc!=ad.Cc;}constexpr bool operator!()const{return!Cc;}template<typename bd>constexpr friend MDB operator+(const bd&cd,const MDB&dd){return MDB(cd)+dd;}template<typename ed>constexpr friend MDB operator-(const ed&fd,const MDB&gd){return MDB(fd)-gd;}template<typename hd>constexpr friend MDB operator*(const hd&id,const MDB&jd){return MDB(id)*jd;}template<typename kd>constexpr friend MDB operator/(const kd&ld,const MDB&md){return MDB(ld)/md;}template<typename nd>constexpr friend MDB operator==(const nd&od,const MDB&pd){return MDB(od)==pd;}template<typename qd>constexpr friend MDB operator!=(const qd&rd,const MDB&sd){return MDB(rd)!=sd;}template<typename td>constexpr friend MDB operator<=(const td&ud,const MDB&vd){return MDB(ud)<=vd;}template<typename wd>constexpr friend MDB operator>=(const wd&xd,const MDB&yd){return MDB(xd)>=yd;}template<typename zd>constexpr friend MDB operator<(const zd&Ad,const MDB&Bd){return MDB(Ad)<Bd;}template<typename Cd>constexpr friend MDB operator>(const Cd&Dd,const MDB&Ed){return MDB(Dd)>Ed;}template<typename Fd>friend Fd&operator>>(Fd&Gd,MDB&Hd){Gd>>Hd.Cc;Hd.Cc=Hd.Dc(Hd.Cc);return Gd;}template<typename Id>friend Id&operator<<(Id&Jd,const MDB&Kd){return Jd<<Kd.Cc;}};
constexpr auto MOD = (int)1e9 + 7;
using Mint = MIB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
// struct Dynamic_ModInt { using value_type = int; static value_type value; };
// Dynamic_ModInt::value_type Dynamic_ModInt::value = (int)1e9 + 7;
// Dynamic_ModInt::value_type &Mod = Dynamic_ModInt::value;
// using Mint = ModInt<Dynamic_ModInt>;
Mint sqrt(Mint Nd){return Nd^(-2);}struct Fact{Fact(const int n):aa(n+1,Mint(1)),ia(n+1),sz(n){aa[0]=1;for(int i=1;i<=n;i++)aa[i]=aa[i-1]*i;ia[n]=Mint(1)/aa[n];for(int i=n;i>=1;i--)ia[i-1]=ia[i]*i;}Mint C(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[m]*ia[n-m];}Mint A(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[n-m];}Mint F(int n)const{if(n<0)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n];}private:std::vector<Mint>aa,ia;const int sz;};
// Fact F(1000000);

using uint=unsigned;inline constexpr uint32_t bitreverse(uint32_t x){x=(x>>16)|(x<<16);x=((x&0xFF00FF00u)>>8)|((x&0x00FF00FFu)<<8);x=((x&0xF0F0F0F0u)>>4)|((x&0x0F0F0F0Fu)<<4);x=((x&0xCCCCCCCCu)>>2)|((x&0x33333333u)<<2);x=((x&0xAAAAAAAAu)>>1)|((x&0x55555555u)<<1);return x;}template<class T,class Omega>inline void Cooley_Tukey_FFT(T*a,Omega omega,uint n,bool inv){assert(!(n&n-1));const int shift=32-__builtin_ctz(n);for(uint i=0,rev;i<n;i++)if(i<(rev=bitreverse(i)>>shift))std::swap(a[i],a[rev]);for(uint o=2,k,i,j;o<=n;o<<=1){const T g=!inv?T(1)/omega(o):omega(o);T w,x,y;for(k=0;k<n;k+=o)for(i=k,j=k+o/2,w=1;j<k+o;w*=g,i++,j++)x=a[i],y=w*a[j],a[i]=x+y,a[j]=x-y;}if(inv){const T invn=T(1)/T(n);for(uint i=0;i<n;i++)a[i]*=invn;}}struct OPMul{auto operator()(auto x,auto y)const{return x*y;}};struct OPDiv{auto operator()(auto x,auto y)const{return x*y;}};template<class T,class Omega,uint Bits>requires(Bits<=23u)&&requires(Omega g,uint n){{g(n)}->std::convertible_to<T>;}struct PolyBase{constexpr static uint N=1u<<Bits;void dft(const uint n=N)const{Cooley_Tukey_FFT(a,Omega{},n,false);}void idft(const uint n=N)const{Cooley_Tukey_FFT(a,Omega{},n,true);}PolyBase&operator+=(const PolyBase&x){for(uint i=0;i<N;i++)a[i]+=x.a[i];return*this;}PolyBase&operator-=(const PolyBase&x){for(uint i=0;i<N;i++)a[i]-=x.a[i];return*this;}PolyBase&operator*=(const PolyBase&x){dft();x.dft();oper<OPMul>(x);idft();x.idft();return*this;}PolyBase&operator/=(const PolyBase&x){return*this*=!x;}PolyBase&operator*=(const T val){for(uint i=0;i<N;i++)a[i]*=val;return*this;}PolyBase&operator/=(const T val){for(uint i=0;i<N;i++)a[i]/=val;return*this;}PolyBase operator!()const{PolyBase f,g;f[0]=T(1)/a[0];for(uint o=2;o<=N;o<<=1){memcpy(g.a,a,sizeof(*a)*o);f.dft(o),g.dft(o);f.oper(g,[](T x,T y){return 2*x-x*x*y;});f.dft();}return f;}template<class OP>requires requires(OP op,T x,T y){{op(x,y)}->std::convertible_to<T>;}PolyBase&oper(const PolyBase&x,OP op=OP{},const uint n=N){for(uint i=0;i<n;i++)a[i]=op(a[i],x.a[i]);return*this;}friend PolyBase operator+(PolyBase x,const PolyBase&y){return x+=y;}friend PolyBase operator-(PolyBase x,const PolyBase&y){return x-=y;}template<class U>friend PolyBase operator*(PolyBase x,const U&y){return x*=y;}template<class U>friend PolyBase operator/(PolyBase x,const U&y){return x/=y;}friend PolyBase operator*(const T val,const PolyBase&x){return x*val;}friend PolyBase operator/(const T val,const PolyBase&x){return x/val;}template<class OP>requires requires(OP op,T x,T y){{op(x,y)}->std::convertible_to<T>;}friend PolyBase polyOper(PolyBase x,const PolyBase&y,OP op=OP{}){return x.oper(y,op);}T&operator[](uint pos){return a[pos];}const T&operator[](uint pos)const{return a[pos];}void assign(T val){std::fill(a,a+N,val);}private:mutable T a[N]{};};
template<int g>struct ModOmega{Mint operator()(uint n)const{return Mint(g)^((Mint::mod()-1)/n);}};template<uint N,int g=3>using MPoly=PolyBase<Mint,ModOmega<g>,N>;

constexpr int N = 70;
#define For(i, s, t) for (int i = (s); i <= (t); i++)
using Vec = std::valarray<Mint>;
// using Vec = MPoly<9>;
using Mat = Mint[N][N];
static Mint q[N]; Mat C, m, ans;
static Vec vec;
static std::vector<int> g[N];
static std::vector<Vec> f[N];
static inline void getInv(Mat m, int n) {
    static Mint mat[N][N*2];
    For(i, 1, n) { // 初始化伴随矩阵 [A | I]
        memcpy(mat[i]+1, m[i]+1, sizeof(**m) * n);
        memset(mat[i]+n+1, 0, sizeof(**m) * n);
        mat[i][i+n] = 1;
    }
    For(i, 1, n) {
        int pivot = i;
        For(r, i + 1, n) if (mat[r][i]() != 0) { pivot = r; break; } // 找到一个不为 0 的做主元行
        if (mat[pivot][i]() == 0) throw std::logic_error("Matrix is singular (not invertible)!"); // 如果主元为 0，说明矩阵秩小于 n，不可逆
        if (pivot != i) For(j, 1, 2*n) std::swap(mat[i][j], mat[pivot][j]); // 交换当前行与主元行
        Mint inv_pivot = Mint(1) / mat[i][i]; // 主元行归一化：将 mat[i][i] 变为 1
        For(j, i, 2*n) mat[i][j] *= inv_pivot;
        For(r, 1, n) if (r != i && mat[r][i]() != 0) { // 消去其他所有行（包括上方和下方的行）
            Mint factor = mat[r][i];
            For(j, i, 2*n) mat[r][j] = mat[r][j] - factor * mat[i][j];
        }
    }
    For(i, 1, n) {
        std::cerr << '{';
        For(j, 1, n) std::cerr << mat[i][j] << (j<n ? ", " : " || ");
        For(j, n+1, n*2) std::cerr << mat[i][j] << (j<n*2 ? ", " : "}\n");
    }
    For(i, 1, n) memcpy(m[i]+1, mat[i]+n+1, sizeof(**m) * n);
}
/* f[i][j][k] 表示以 i 为根的子树，走点权<=j的情况下，有 k 个点可达。
 * 可以得到转移式 f[u][j1+j2][k1+k2] += f[v1][j1][k1] * f[v2][j2][k2] * C(j1+j2, j1) * C(sz[v1]-j1 + sz[v2]-j2, sz[v1]-j1)
 * 注意使用了一个经典 Trick，每个子树可以填的值都变成了（映射/压缩为了） 1~sz[u]，方便之后维持相对顺序合并子树
 * 目前 f[i][j] 是一个树上背包，复杂度就是 O(n^2)
 * 计算完 f[u] 之后，不要先加入 u，枚举 j, k, j'，然后 ans[u][j'] += q[k] * f[u][j][k] * C(j'-1, j-1) * C(n-j', sz[u]-j)
   这个计算式意义就是将 u 点权钦定为 j'，然后将子树内和子树外的进行合并
-+---------------------------------------------------------------------------------------------------------------------------
 * 然后就发现，在枚举 j1, j2 的情况下，第三维的 k 事实上是一个加法卷积，复杂度 O(n^2)，总复杂度 O(n^4)。
 * 于是考虑使用点值加速，第三维的 O(n^2) -> O(n)
 * 现在考虑具体的加速方法，我们带入一个范德蒙德矩阵，具体来说就是一个 A_{i,j} = x_i^j 的矩阵，
   这个矩阵乘上第三维的转置向量，就得到了一个点值表达向量，事实上就是 [F(x0), F(x1), F(x2), ..., F(xn)] 其中 F 代表 f[i][j] 的生成函数
 * 然后，我们发现需要使用拉插之类的~~丑陋~~的东西，去弄出来原来的值，时间就爆炸了。
 * 但是！我们会发现一个点，那就是，我们事实上需要的只是一个 \sum_k q[k] * f[u][j][k]，也就是，两个向量的点积。
 * 不妨将这个值 fval[i][j] 记录为 Q * F[i][j]^T，然后我们来看我们求的点值表达向量 Fval[i][j] = Mat * F[i][j]^T，那我们只需要——
   变成 F[i][j]^T = Mat^{-1} * Fval[i][j] ====> fval[i][j] = Q * Mat^{-1} * Fval[i][j]
 * 有了这个 Key Observation，我们就能知道，在预处理了 Q * Mat^{-1} 之后，我们计算 fval[i][j] 就非常的 easy 了，然后来看最终的 ans 表达式：
 * ans[i][j'] = fval[i][j] * C(j'-1, j-1) * C(n-j', sz[u]-j)
-+---------------------------------------------------------------------------------------------------------------------------
 * 现在考虑一个小细节，就是如何加入点 u。
 * 加入点 u 需要和计算答案时将点 u 的点权放到最大区分开来，不要混淆了。
 * 直接将 u 视作一个单独的子树，其容量为 1。那也就是加入一个 vec[j][k], 其 vec[1][1]=1。
 * 形式化一点，那就是
 * f
 */
static int n, sz[N];
std::ostream& operator<<(std::ostream& os, const Vec &a) { os << '['; For(i, 0, n) os << a[i] << (i<n ? ", " : "]"); return os; }
// Vec OO0, OO1, OO2;
Vec base1;
static void dfs(int u, int pre) {
    std::cerr << "Visiting node " << u << std::endl;
    sz[u] = 0;
    // f[u].resize(1, OO1);
    f[u].resize(1, Vec(1, n+1));
    for (int v : g[u]) if (v != pre) {
        dfs(v, u);
        // std::vector<Vec> ff(1 + sz[u] + sz[v], OO0);
        std::vector<Vec> ff(1 + sz[u] + sz[v], Vec(n+1));
        For(i, 0, sz[u]) For(j, 0, sz[v]) ff[i+j] += f[u][i] * f[v][j] * C[i+j][i] * C[sz[u]-i + sz[v]-j][sz[u]-i];
        ff.swap(f[u]);
        sz[u] += sz[v];
        // std::cerr << "After " << v << "-->" << u << " updated, the f[u] becomes: " << std::endl; 
        // For(i, 0, sz[u]) std::cerr << f[u][i] << std::endl;
    }
    std::vector<Mint> fval(sz[u]+1);
    For(i, 0, sz[u]) For(j, sz[u]+2, n) f[u][i][j] = 0;
    For(i, 0, sz[u]) fval[i] = (vec * f[u][i]).sum();
    std::cerr << "fval[" << u << "] = "; For(j, 0, sz[u]) std::cerr << fval[j] << ' '; std::cerr << "." << std::endl;
    // For(i, 0, sz[u]) For(j, 0, n) fval[i] += f[u][i][j] * q[j];
    For(i, 0, sz[u]) For(j, i+1, n) ans[u][j] += fval[i] * C[j-1][i] * C[n-j][sz[u]-i];
    std::cerr << "Ans[" << u << "] = "; For(j, 1, n) std::cerr << ans[u][j] << ' '; std::cerr << std::endl;
    // std::vector<Vec> ff(++sz[u] + 1, OO0);
    For(i, 0, sz[u]) std::cerr << f[u][i] << std::endl;
    ++sz[u];
    f[u].emplace_back(n+1);
    for (int i = sz[u] - 1; i >= 0; i--) 
        f[u][i+1] += f[u][i] * base1 * (i+1),
        f[u][i] += f[u][i] * (sz[u]-i+1);
    std::cerr << "Leaving node " << u << ", and calculated\n";
    For(i, 0, sz[u]) std::cerr << f[u][i] << std::endl;
}
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    std::cin >> n;
    base1.resize(n+1); std::iota(&base1[0], &base1[n]+1, 0);
    // OO1[0] = 1;
    // For(i, 0, n) OO2[i] = 1;
    For(i, 1, n) std::cin >> q[i];
    For(i, 0, n) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    For(i, 1, n-1) {
        int u, v; std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    For(i, 1, n) {
        Mint now = 1;
        For(j, 1, n) m[i][j] = now *= i;
    }
    Mat tmp{};
    memcpy(tmp, m, sizeof m);
    getInv(m, n);
    vec.resize(n+1);
    For(k, 1, n) For(j, 1, n) vec[j] += q[k] * m[k][j];
    Mat xx{};
    For(k, 1, n) For(i, 1, n) For(j, 1, n) xx[i][j] += tmp[i][k] * m[k][j];
    For(i, 1, n) For(j, 1, n) std::cerr << xx[i][j] << " \n"[j==n];
    std::cerr << vec << std::endl;
    dfs(1, 0);
    For(i, 1, n) {
        For(j, 1, n) std::cout << ans[i][j] << ' ';
        std::cout << std::endl;
    }
    return 0;
}