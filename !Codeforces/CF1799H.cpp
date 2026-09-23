#include <bits/stdc++.h>
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
#ifdef CLANGD
constexpr int N = 10, K = 1;
#else
constexpr int N = 5004, K = 6;
#endif
/* 考虑只去做删除子树，然后把所有节点作为根的答案加起来。最后除以初始的 d[k] 就是答案。 */
using DP = std::array<Mint, 1<<K>;
DP f1[N], f2[N], f3[N], pre[N], suf[N]; // f1: 以 1 为根；f2: 除去当前点的 f；f3：以 i 为根的答案
std::vector<int> g[N], ng[N];
int sz[N], d[K], dsum[1<<K], n, k;
DP& operator*=(DP& x, const DP& y) { // 子集卷积
    for (int s = (1<<k)-1; s; s--)
        for (int t = s; t; t = (t-1) & s)
            x[s] += x[s^t] * y[t];
    return x;
}
DP& operator+=(DP& f, int v) {
    for (int s = (1<<k)-1; s; s--) // 要想删掉这颗子树，要确保子树内不会再有后续操作。
        if (dsum[s] == v) f[s] += f[s ^ (1 << 31 - __builtin_clz(s))];
    return f;
}
DP operator*(DP x, const DP& y) { return x *= y; }
DP operator+(DP x, int v) { return x += v; }
void dfs1(int u, int fa) {
    ng[u].reserve(g[u].size());
    sz[u] = 1; f1[u][0] = 1;
    for (int v : g[u]) if (v != fa)
        ng[u].push_back(v), dfs1(v, u);
    for (int v : g[u]) if (v != fa)
        sz[u] += sz[v], f1[u] *= f1[v];
    f3[u] = f1[u];
    f1[u] += sz[u];
}
void dfs2(int u, int fa) {
    f2[u] += n - sz[u];
    f3[u] *= f2[u], f3[u] += n;
    for (int i = 0; i < (int)ng[u].size(); i++)
        pre[i] = suf[i] = f1[ng[u][i]];
    for (int i = 1; i < (int)ng[u].size(); i++)
        pre[i] *= pre[i-1];
    for (int i = (int)ng[u].size() - 2; i >= 0; i--)
        suf[i] *= suf[i+1];
    for (int i = 0; i < (int)ng[u].size(); i++) {
        int v = ng[u][i];
        f2[v] = f2[u];
        if (i > 0) f2[v] *= pre[i-1];
        if (i < (int)ng[u].size() - 1) f2[v] *= suf[i+1];
    }
    for (int v : ng[u]) dfs2(v, u);
}
int main() {
    scanf("%d", &n);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    scanf("%d", &k);
    for (int i = 0; i < k; i++)
        scanf("%d", &d[i]);
    int dk = d[k-1];
    for (int i = k-1; i > 0; i--)
        d[i] = d[i-1] - d[i];
    d[0] = n - d[0];
    for (int s = 1; s < (1<<k); s++)
        dsum[s] = dsum[s&s-1] + d[__builtin_ctz(s)];
    dfs1(1, 0);
    f2[1][0] = 1; dfs2(1, 0);
    Mint ans = 0;
    for (int i = 1; i <= n; i++)
        ans += f3[i][(1<<k)-1];
    printf("%d\n", ans / dk);
}