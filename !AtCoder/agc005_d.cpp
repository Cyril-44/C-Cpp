#include <bits/stdc++.h>
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 924844033;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
constexpr int N = 2005;
#ifdef CLANGD
#define N 2
#endif
Mint f[N+N][N], fac[N]; // f[i][j] 表示已经处理了 i 个点，独立集大小为 j 的方案数
bool cut[N+N];
/*
先容斥，然后化为独立集计数
*/
int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    fac[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = fac[i-1] * i;
    int tot = 0;
    cut[0] = true;
    for (int r = 1; r <= k; r++) { // r, r+k, r+2k, ..., r+(p-1)k
        int p = (n - r) / k; // r+(p-1)k <= n-k
        cut[tot += p] = true;
        cut[tot += p] = true;
    }
    f[0][0] = 1;
    for (int i = 1; i <= 2 * (n-k); i++) {
        for  (int j = 0; j <= n; j++) {
            f[i][j] = f[i-1][j];
            if (j > 0) f[i][j] += f[i-1 - !cut[i-1]][j-1];
        }
    }
    Mint ans = 0;
    for (int i = 0; i <= n; i++) {
        Mint val = f[2*(n-k)][i] * fac[n - i];
        if (i & 1) ans -= val;
        else ans += val;
    }
    printf("%d\n", ans);
}
