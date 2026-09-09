#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 1;
#else
constexpr int N = 15;
#endif
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = (int)1e9 + 7;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint f[1<<N][N+1], g[1<<N][N+1], h[1<<N], C[N+2][N+2], F[N*2+1];
unsigned ins[N]; // In Nodes 入点集合
int main() {
    int n, m, k;
    scanf("%d%d%d", &n, &m, &k);
    F[0] = 1;
    for (int i = 1; i <= n+k; i++) F[i] = F[i-1] * i;
    for (int i = 0; i <= k+1; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        ins[v] |= 1 << u;
    }
    f[0][0] = g[0][0] = h[0] = 1;
    for (int s = 1; s < (1<<n); s++)
        for (int i = 0; i < n; i++)
            if ((s >> i & 1) && !(ins[i] & s))
                h[s] += h[s ^ (1<<i)]; // 直接往里面一个一个塞点，计算的就是排列的个数，已经带上了顺序
    for (int s = 1; s < (1<<n); s++)
        for (int t = s; t; t = (t-1) & s)
            for (int i = 1; i <= n; i++)
                g[s][i] += g[s^t][i-1] * h[t]; // 算有多少个整段的目的是提供容斥系数
    for (int s = 1; s < (1<<n); s++)
        for (int t = s; t; t = (t-1) & s)
            for (int i = 1; i <= n; i++) {
                for (int j = 1; j <= i; j++)
                    if (j & 1) f[s][i] += f[s^t][i-j] * g[t][j];
                    else f[s][i] -= f[s^t][i-j] * g[t][j];
            }
    Mint ans = 0;
    for (int i = 1; i < (1<<n); i++) {
        fprintf(stderr, "[%d]: %d\t\tf g\n", i, h[i]);
        for (int j = 1; j <= n; j++)
            fprintf(stderr, "\t[%d]\t%d %d\n", j, f[i][j], g[i][j]);
    }
    for (int i = 1; i <= n; i++)
        ans += f[(1<<n)-1][i] * C[k+1][i] * F[i] * F[k];
    fprintf(stderr, "ans=%d / %d\n", ans, F[n+k]);
    printf("%d\n", ans / F[n+k]);
}