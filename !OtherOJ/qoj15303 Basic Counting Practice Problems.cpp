#include <bits/stdc++.h>
#include <type_traits>
template<typename bb>constexpr inline bb modInv(bb cb,bb db){assert(cb!=0);bb eb=0,fb=1,gb=cb,hb=db,ib;while(gb!=0){ib=hb/gb;std::swap(gb,hb-=ib*gb);std::swap(eb-=ib*fb,fb);}assert(hb==1);return eb;}template<class kb,typename kb::value_type lb=0>requires std::integral<typename kb::value_type>class MIB{using mb=kb::value_type;mb nb;template<typename pb>constexpr mb ob(pb qb){if constexpr(std::is_unsigned_v<pb>)return static_cast<mb>(qb<pb(mod())?qb:qb%pb(mod()));else{mb sb=static_cast<mb>(-mod()<qb&&qb<mod()?qb:qb%mod());return(sb<0?sb+mod():sb);}}public:static constexpr mb mod(){return kb::value;}constexpr MIB():nb(lb){}template<typename tb>constexpr MIB(tb ub){nb=ob(ub);}template<typename vb>explicit constexpr operator vb()const{return static_cast<vb>(nb);}constexpr mb operator()()const{return nb;}constexpr MIB&operator+=(MIB wb){if((nb+=wb.nb)>=mod())nb-=mod();return*this;}constexpr MIB&operator-=(MIB xb){if((nb-=xb.nb)<0)nb+=mod();return*this;}constexpr MIB&operator*=(MIB yb){if constexpr(std::is_same_v<mb,int>)nb=ob((uint64_t)nb*yb.nb);else if constexpr(std::is_same_v<mb,int64_t>)nb=ob((unsigned __int128)nb*yb.nb);else nb=ob(nb*yb.nb);return*this;}constexpr MIB&operator/=(MIB zb){return*this*=MIB(modInv(zb.nb,mod()));}template<std::integral Ab>constexpr MIB&operator^=(Ab Bb){if(Bb<0)return*this=MIB(modInv(static_cast<mb>(1),(*this^(-Bb))()));MIB Cb=*this;for(*this=static_cast<mb>(1);Bb;Bb>>=1){if(Bb&1)*this*=Cb;Cb*=Cb;}return*this;}constexpr MIB operator-()const{return MIB(-nb);}constexpr MIB&operator++(){return*this+=1;}constexpr MIB&operator--(){return*this-=1;}constexpr MIB operator++(int){MIB Db=*this;++*this;return Db;}constexpr MIB operator--(int){MIB Eb=*this;--*this;return Eb;}constexpr MIB operator+(MIB Fb)const{return MIB(*this)+=Fb;}constexpr MIB operator-(MIB Gb)const{return MIB(*this)-=Gb;}constexpr MIB operator*(MIB Hb)const{return MIB(*this)*=Hb;}constexpr MIB operator/(MIB Ib)const{return MIB(*this)/=Ib;}template<std::integral Jb>constexpr MIB operator^(const Jb Kb)const{return MIB(*this)^=Kb;}constexpr bool operator==(MIB Lb)const{return nb==Lb.nb;}constexpr bool operator!=(MIB Mb)const{return nb!=Mb.nb;}constexpr bool operator!()const{return!nb;}template<std::integral Nb>constexpr friend MIB operator+(Nb Ob,MIB Pb){return MIB(Ob)+Pb;}template<std::integral Qb>constexpr friend MIB operator-(Qb Rb,MIB Sb){return MIB(Rb)-Sb;}template<std::integral Tb>constexpr friend MIB operator*(Tb Ub,MIB Vb){return MIB(Ub)*Vb;}template<std::integral Wb>constexpr friend MIB operator/(Wb Xb,MIB Yb){return MIB(Xb)/Yb;}template<std::integral Zb>constexpr friend MIB operator==(Zb _b,MIB ac){return MIB(_b)==ac;}template<std::integral bc>constexpr friend MIB operator!=(bc cc,MIB dc){return MIB(cc)!=dc;}template<std::integral ec>constexpr friend MIB operator<=(ec fc,MIB gc){return MIB(fc)<=gc;}template<std::integral hc>constexpr friend MIB operator>=(hc ic,MIB jc){return MIB(ic)>=jc;}template<std::integral kc>constexpr friend MIB operator<(kc lc,MIB mc){return MIB(lc)<mc;}template<std::integral nc>constexpr friend MIB operator>(nc oc,MIB pc){return MIB(oc)>pc;}template<typename qc>friend qc&operator>>(qc&rc,MIB&sc){rc>>sc.nb;sc.nb=sc.ob(sc.nb);return rc;}template<typename tc>friend tc&operator<<(tc&uc,MIB vc){return uc<<vc.nb;}};template<class xc,typename xc::value_type yc=0>requires requires(typename xc::value_type zc,typename xc::value_type Ac){{zc+Ac}->std::same_as<typename xc::value_type>;{zc-Ac}->std::same_as<typename xc::value_type>;{zc*Ac}->std::same_as<typename xc::value_type>;{zc%Ac}->std::same_as<typename xc::value_type>;{zc+=Ac}->std::same_as<typename xc::value_type&>;{zc-=Ac}->std::same_as<typename xc::value_type&>;{zc*=Ac}->std::same_as<typename xc::value_type&>;{zc%=Ac}->std::same_as<typename xc::value_type&>;requires std::is_default_constructible_v<typename xc::value_type>;requires std::is_copy_constructible_v<typename xc::value_type>;requires std::is_move_constructible_v<typename xc::value_type>;requires std::is_assignable_v<typename xc::value_type&,int>;requires std::equality_comparable<typename xc::value_type>;}class MDB{using Bc=xc::value_type;Bc Cc;template<std::integral Ec>constexpr Bc Dc(Ec Fc){if constexpr(std::is_unsigned_v<Ec>)return static_cast<Bc>(Fc<Ec(Gc())?Fc:Fc%Ec(Gc()));else{Bc Hc=static_cast<Bc>(-Gc()<Fc&&Fc<Gc()?Fc:Fc%Gc());return(Hc<0?Hc+Gc():Hc);}}public:static constexpr Bc Gc(){return xc::value;}constexpr MDB():Cc(yc){}template<std::integral Ic>constexpr MDB(const Ic&Jc){Cc=ob(Jc);}template<std::integral Kc>explicit constexpr operator Kc()const{return static_cast<Kc>(Cc);}constexpr Bc operator()()const{return Cc;}constexpr MDB&operator+=(const MDB&Lc){if((Cc+=Lc.Cc)>=Gc())Cc-=Gc();return*this;}constexpr MDB&operator-=(const MDB&Mc){if((Cc-=Mc.Cc)<0)Cc+=Gc();return*this;}constexpr MDB&operator*=(const MDB&Nc){if constexpr(std::is_same_v<Bc,int>)Cc=Dc((uint64_t)Cc*Nc.Cc);else if constexpr(std::is_same_v<Bc,int64_t>)Cc=Dc((unsigned __int128)Cc*Nc.Cc);else Cc=Dc(Cc*Nc.Cc);return*this;}constexpr MDB&operator/=(const MDB&Oc){return*this*=MDB(modInv(Oc.Cc,Gc()));}template<std::integral Pc>constexpr MDB&operator^=(Pc Qc){if(Qc<0)return*this=MDB(modInv(static_cast<Bc>(1),(*this^(-Qc))()));MDB Rc=*this;for(*this=static_cast<Bc>(1);Qc;Qc>>=1){if(Qc&1)*this*=Rc;Rc*=Rc;}return*this;}constexpr MDB operator-()const{return MDB(-Cc);}constexpr MDB&operator++(){return*this+=1;}constexpr MDB&operator--(){return*this-=1;}constexpr MDB operator++(int){MDB Sc=*this;++*this;return Sc;}constexpr MDB operator--(int){MDB Tc=*this;--*this;return Tc;}constexpr MDB operator+(const MDB&Uc)const{return MDB(*this)+=Uc;}constexpr MDB operator-(const MDB&Vc)const{return MDB(*this)-=Vc;}constexpr MDB operator*(const MDB&Wc)const{return MDB(*this)*=Wc;}constexpr MDB operator/(const MDB&Xc)const{return MDB(*this)/=Xc;}template<std::integral Yc>constexpr MDB operator^(const Yc Zc)const{return MDB(*this)^=Zc;}constexpr bool operator==(const MDB&_c)const{return Cc==_c.Cc;}constexpr bool operator!=(const MDB&ad)const{return Cc!=ad.Cc;}constexpr bool operator!()const{return!Cc;}template<typename bd>constexpr friend MDB operator+(const bd&cd,const MDB&dd){return MDB(cd)+dd;}template<typename ed>constexpr friend MDB operator-(const ed&fd,const MDB&gd){return MDB(fd)-gd;}template<typename hd>constexpr friend MDB operator*(const hd&id,const MDB&jd){return MDB(id)*jd;}template<typename kd>constexpr friend MDB operator/(const kd&ld,const MDB&md){return MDB(ld)/md;}template<typename nd>constexpr friend MDB operator==(const nd&od,const MDB&pd){return MDB(od)==pd;}template<typename qd>constexpr friend MDB operator!=(const qd&rd,const MDB&sd){return MDB(rd)!=sd;}template<typename td>constexpr friend MDB operator<=(const td&ud,const MDB&vd){return MDB(ud)<=vd;}template<typename wd>constexpr friend MDB operator>=(const wd&xd,const MDB&yd){return MDB(xd)>=yd;}template<typename zd>constexpr friend MDB operator<(const zd&Ad,const MDB&Bd){return MDB(Ad)<Bd;}template<typename Cd>constexpr friend MDB operator>(const Cd&Dd,const MDB&Ed){return MDB(Dd)>Ed;}template<typename Fd>friend Fd&operator>>(Fd&Gd,MDB&Hd){Gd>>Hd.Cc;Hd.Cc=Hd.Dc(Hd.Cc);return Gd;}template<typename Id>friend Id&operator<<(Id&Jd,const MDB&Kd){return Jd<<Kd.Cc;}};
constexpr auto MOD = (int)1e9 + 7;
using Mint = MIB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint sqrt(Mint Nd){return Nd^(-2);}struct Fact{Fact(const int n):aa(n+1,Mint(1)),ia(n+1),sz(n){aa[0]=1;for(int i=1;i<=n;i++)aa[i]=aa[i-1]*i;ia[n]=Mint(1)/aa[n];for(int i=n;i>=1;i--)ia[i-1]=ia[i]*i;}Mint C(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[m]*ia[n-m];}Mint A(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[n-m];}Mint F(int n)const{if(n<0)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n];}private:std::vector<Mint>aa,ia;const int sz;};
// Fact F(1000000);

constexpr int N = 70;
#define For(i, s, t) for (int i = (s); i <= (t); i++)
using Vec = std::valarray<Mint>;
using Mat = Mint[N][N];
static Mint q[N], fact[N]; Mat C, m, ans;
static Vec vec;
static std::vector<int> g[N];
static std::vector<Vec> f[N];
static inline void getInv(Mat m, int n) {
    static Mint mat[N][N*2];
    For(i, 0, n-1) { // 初始化伴随矩阵 [A | I]
        memcpy(mat[i], m[i], sizeof(**m) * n);
        memset(mat[i]+n, 0, sizeof(**m) * n);
        mat[i][i+n] = 1;
    }
    For(i, 0, n-1) {
        int pivot = i;
        For(r, i+1, n-1) if (mat[r][i]() != 0) { pivot = r; break; } // 找到一个不为 0 的做主元行
        if (mat[pivot][i]() == 0) throw std::logic_error("Matrix is singular (not invertible)!"); // 如果主元为 0，说明矩阵秩小于 n，不可逆
        if (pivot != i) For(j, 0, 2*n-1) std::swap(mat[i][j], mat[pivot][j]); // 交换当前行与主元行
        Mint inv_pivot = Mint(1) / mat[i][i]; // 主元行归一化：将 mat[i][i] 变为 1
        For(j, i, 2*n-1) mat[i][j] *= inv_pivot;
        For(r, 0, n-1) if (r != i && mat[r][i]() != 0) { // 消去其他所有行（包括上方和下方的行）
            Mint factor = mat[r][i];
            For(j, i, 2*n-1) mat[r][j] = mat[r][j] - factor * mat[i][j];
        }
    }
    For(i, 0, n-1) {
        std::cerr << '{';
        For(j, 0, n-1) std::cerr << mat[i][j] << (j<n-1 ? ", " : " || ");
        For(j, n, n*2-1) std::cerr << mat[i][j] << (j<n*2-1 ? ", " : "}\n");
    }
    For(i, 0, n-1) memcpy(m[i], mat[i]+n, sizeof(**m) * n);
    For(i, 0, n-1) {
        std::cerr << '{';
        For(j, 0, n-1) std::cerr << m[i][j] << (j<n-1 ? ", " : "}\n");
    }
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
std::ostream& operator<<(std::ostream& os, const Vec &a) { os << '['; For(i, 0, n-1) os << a[i] << (i<n-1 ? ", " : "]"); return os; }
std::ostream& operator<<(std::ostream& os, const std::vector<Mint> &a) { for (Mint i : a) os << i << ' '; return os; }
std::ostream& operator<<(std::ostream& os, const std::vector<Vec> &a) { for (const auto &i : a) os << i << '\n'; return os; }
std::ostream& operator<<(std::ostream& os, const Mint* a) { For(i, 1, n) os << a[i] << ' '; return os; }
// Vec OO0, OO1, OO2;
Vec base_pos0_1, base_pos1_1;
static void dfs(int u, int pre) {
    // std::cerr << "Visiting node " << u << std::endl;
    sz[u] = 0;
    f[u].assign(1, base_pos0_1);
    for (int v : g[u]) if (v != pre) { dfs(v, u);
        std::vector<Vec> ff(1 + sz[u] + sz[v], Vec(n));
        For(i, 0, sz[u]) For(j, 0, sz[v]) ff[i+j] += f[u][i] * f[v][j] * C[i+j][i] * C[sz[u]-i + sz[v]-j][sz[u]-i];
        ff.swap(f[u]);
        sz[u] += sz[v];
        // std::cerr << "After " << v << "-->" << u << " updated, the f[u] becomes: " << std::endl; 
        // For(i, 0, sz[u]) std::cerr << f[u][i] << std::endl;
    }
    std::cerr << "Leaving node " << u << ", and calculated\n" << f[u];
    std::vector<Mint> fval(sz[u]+1);
    For(i, 0, sz[u]) fval[i] = (vec * f[u][i]).sum();
    For(i, 0, sz[u]) For(j, i+1, n) ans[u][j] += fval[i] * C[j-1][i] * C[n-j][sz[u]-i] * fact[n-sz[u]-1];
    ++sz[u]; f[u].emplace_back(n);
    // f[u][0] = base_pos0_1;
    for (int i = sz[u] - 1; i >= 0; i--)
        f[u][i+1] += f[u][i] * base_pos1_1 * (i+1),
        f[u][i] += f[u][i] * (sz[u]-i+1);
    std::cerr << "fval[" << u << "] = " << fval << ".\n"
              << "Ans["  << u << "] = " << ans[u] << ".\n"
              << "After adding u:\n" << f[u];
}
int main() {
    std::cin.tie(nullptr) -> sync_with_stdio(false);
    std::cin >> n;
    base_pos1_1.resize(n); std::iota(&base_pos1_1[0], &base_pos1_1[0] + n, 0);
    base_pos0_1.resize(n, 1);
    // OO1[0] = 1;
    // For(i, 0, n) OO2[i] = 1;
    For(i, 1, n) std::cin >> q[i];
    For(i, 0, n) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    fact[0] = 1;
    For(i, 1, n) fact[i] = fact[i-1] * i;
    For(i, 1, n-1) {
        int u, v; std::cin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    For(i, 0, n-1) { Mint now = 1; For(j, 0, n-1) m[i][j] = now, now *= i; }
    Mat tmp{};
    memcpy(tmp, m, sizeof m);
    getInv(m, n);
    vec.resize(n);
    For(k, 0, n-1) For(j, 0, n-1) vec[j] += q[k+1] * m[k][j];
    Mat xx{};
    For(k, 0, n-1) For(i, 0, n-1) For(j, 0, n-1) xx[i][j] += tmp[i][k] * m[k][j];
    std::cerr << "Wish to get an Epsilon Matrix:" << std::endl;
    For(i, 0, n-1) For(j, 0, n-1) std::cerr << xx[i][j] << " \n"[j==n-1];
    std::cerr << "\"Q\" Matrix: " << vec << std::endl;
    dfs(1, 0);
    For(i, 1, n) std::cout << ans[i] << '\n';
    return 0;
}