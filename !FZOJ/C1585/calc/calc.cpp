#include <bits/stdc++.h>
constexpr int N = 100001, K = 101;
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>class MB{using Int=typename Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<class T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<class T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr int MOD = (int)1e9 + 7;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
int fa[N], sz[N];
Mint f[N][K], C[K][K], tmp[K];
#define For(i, s, t) for (int i = (s), _ed = (t); i <= _ed; i++)
#define roF(i, s, t) for (int i = (t); i >= (s); i--)
/*
  sum(连通块 乘积 ^ k)
= sum(所有连通块中选 k 次点的方案数 乘积)

令 f[u][i] 表示以 u 为根连通块，选了 k 个点的方案数乘积
*/
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    For(i, 0, k) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    For(i, 2, n) scanf("%d", &fa[i]);

    For(i, 1, n) f[i][0] = 1, sz[i] = 1;
    roF(v, n, 2) {
        int u = fa[v];
        roF(i, k, 0) roF(j, i-1, 0)
            f[v][i] += f[v][j] * C[i][j];
        For(j, 0, k) tmp[j] = f[u][j] * f[v][k];
        For(j, 0, std::min(sz[v], k))
            For(i, 0, std::min(sz[u], k-j))
                tmp[i+j] += f[u][i] * f[v][j] * C[i+j][j];
        sz[u] += sz[v];
        memcpy(f[u], tmp, sizeof tmp);
    }
    for (int i = 1; i <= n; i++)
        for (int j = 0; j <= k; j++)
            printf("%d%c", f[i][j], " \n"[j==k]);
    printf("%d\n", f[1][k]);
    return 0;
}