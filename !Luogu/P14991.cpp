#include <bits/stdc++.h>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
#define roF(i, s, t) for(int i = (s); i >= (t); i--)
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
constexpr int N = 
#ifdef CLANGD
3
#else
30004
#endif
, K = 16;
std::vector<std::pair<int, int>> sons[N];
Mint f[N][K+1], C[K+1][K+1], fac[K+1], ifac[K+1];
/* 本题应当先构建双射，最终目的是求本质不同的上传颜色序列集合个数，满足根节点上传颜色序列长度为 0（根节点不上传）
 * f[u][i] 表示上传序列长度为 i 的方案数
 * 有：g_n = \sum_{\forall a,b,t \text{ s.t. } n = \sum\limits_i (a_i+b_i)} \frac{(\sum\limits_i (a_i+b_i-t_i))!}{\prod\limits_i (a_i+b_i-t_i)} \prod_i (\sum_i (-1)^{t_i}  \binom{b_i}{t_i} \binom{a_i+b_i-1}{a_i-1} f_{v_i, a_i})
 * 其中 sum/prod 是将若干个元素集不重合的有序序列归并的方案数，
 * 后面的容斥是钦定最终归并的序列中（最少）有 t_i 组相邻元素，技巧是将这些元素粘起来，当作有效长度 -1。于是可以容斥。
 * 组合数 (a_i + b_i - 1, a_i - 1) 是指将长度为 a 的无相邻相同元素的原序列，变成长度为 a+b 的序列，有 b_i 个元素是由复制前一个元素得到的方案数
 * h[i][j] 表示扩展到了长度为 i 容斥后长度为 j 的转移系数
 * g[i][j] 子树背包，总长度 i，容斥总长度 j 的转移系数
 * 最终上面说的 g_n = \sum_j g_{i,j} \cdot j!
 * 子树上传的时候，可以考虑加入当前节点颜色 u（u=0 不行），以及是否用掉当前序列尾部的颜色。
 * f[u][i] = (i!=0) * (g[i-1] + g[i]) + g[i] + g[i+1]
 */
void dfs(int u) {
    for (auto [v, w] : sons[u]) dfs(v);
    Mint g[K+1][K+1]{};
    g[0][0] = 1;
    for (auto [v, w] : sons[u]) {
        Mint h[K+1][K+1]{}, tg[K+1][K+1]{};
        h[0][0] = f[v][0];
        For(a, 1, w) For(b, 0, w-a) For(t, 0, b) {
            Mint val = ifac[a+b-t] * C[a+b-1][a-1] * C[b][t] * f[v][a];
            if (t & 1) h[a+b][a+b-t] -= val;
            else h[a+b][a+b-t] += val;
        }
        For(a, 0, K) For(b, 0, a) roF(c, K-a, 0) roF(d, std::min(K-b, c), 0)
            tg[a+c][b+d] += g[a][b] * h[c][d];
        memcpy(g, tg, sizeof g);
    }
    For(i, 0, K) {
        Mint sum = 0;
        For(j, 0, i) sum += g[i][j] * fac[j];
        if (i == 0) f[u][i] += sum, f[u][i+1] += sum;
        else {
            f[u][i-1] += sum, f[u][i] += sum + sum;
            if (i != K) f[u][i+1] += sum;
        }
    }
}
int main() {
    For(i, 0, K) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    fac[0] = 1; For(i, 1, K) fac[i] = fac[i-1] * i;
    ifac[K] = 1 / fac[K]; roF(i, K, 1) ifac[i-1] = ifac[i] * i;
    int n; scanf("%d", &n);
    For(i, 1, n-1) {
        int p, k; scanf("%d%d", &p, &k);
        sons[p].emplace_back(i, k);
    }
    dfs(0);
    printf("%d\n", f[0][0]);
    return 0;
}