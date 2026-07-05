#include <bits/stdc++.h>
constexpr int N = 5005, MOD = (int)1e9 + 7;
#define For(i, s, t) for(int i=(s); i<=(t); i++)
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>class MB{using Int=typename Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<class T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<class T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint inv[N];
std::vector<int> g[N];
Mint f[N][N], tf[N], initf; // f[i][j] 表示以 i 为根的子树，保留若干边之后，当前子树大小为 j
Mint initg; // f[i][0] 表示以 i 为根的子树，当前填的值一定确保满足要求
int sz[N];
void dfs(int u, int fa = 0, bool flg = false) { // flg=0 表示父->子， flg=1 表示子->父
    sz[u] = 1, f[u][1] = initf, f[u][0] = initg;
    for (int v : g[u]) if (v != fa) {
        dfs(v, u, !flg);
        Mint fvsum = std::accumulate(f[v]+1, f[v]+sz[v]+1, Mint(0));
        tf[0] = f[u][0] * (f[v][0] + fvsum);
        For(i, 1, sz[u]) tf[i] = f[u][i] * f[v][0];
        memset(tf + sz[u]+1, 0, sz[v] << 2);
        if (!flg) {
            For(i, 1, sz[u])
                For(j, 1, sz[v])
                    tf[i+j] += f[u][i] * f[v][j];
        } else { // 容斥：子->父的拓扑序限制 = 无限制 - 父->子
            For(i, 1, sz[u]) {
                tf[i] += f[u][i] * fvsum;
                For(j, 1, sz[v])
                    tf[i+j] -= f[u][i] * f[v][j];
            }
        }
        memcpy(f[u], tf, (sz[u] += sz[v]) + 1 << 2);
    }
    For(i, 1, sz[u]) f[u][i] *= inv[i]; // 最终的答案是拓扑序数量/总排列数量，而拓扑序数量=总排列数量/每个子树大小的阶乘积
}
/*
套路性的将实数随机转化为 [1,n] 随机排列 
*/
int main() {
    inv[1] = 1;
    For(i, 2, 5000) inv[i] = -inv[MOD % i] * (MOD / i);
    int n, p, q;
    scanf("%d%d%d", &n, &p, &q);
    Mint t = Mint(p) / q;
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    if (p < q) initf = t, initg = 1 - t; // 区间：0 ~ t，选中 >t 的直接成功
    else initf = 2 - t, initg = 0; // 区间：t-1 ~ 1，没有可以直接成功的
    dfs(1, 0);
    printf("%d\n", std::accumulate(f[1], f[1] + n+1, Mint(0)));
    return 0;
}