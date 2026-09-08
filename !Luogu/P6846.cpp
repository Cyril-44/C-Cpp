#include <bits/stdc++.h>
constexpr int N = 18, MOD = 998244353;
unsigned g[N];
bool isind[1<<N];
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint f[1<<N];
/* DAG 定向板子题，最终因为 DAG 的对称性，翻转边数互补，所以总边数=方案数*m/2 */
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    for (int u, v, i = 1; i <= m; i++) {
        scanf("%d%d", &u, &v);
        g[u-1] |= 1 << v-1;
        g[v-1] |= 1 << u-1;
    }
    isind[0] = 1;
    for (int s = 0; s < (1<<n); s++)
        for (int i = 0; i < n; i++)
            if (!(s>>i&1) && !(g[i] & s))
                isind[s | 1<<i] |= isind[s];
    f[0] = 1;
    for (int s = 1; s < (1<<n); s++)
        for (int t = s; t; t = (t-1) & s) if (isind[t]) {
            if (__builtin_parity(t)) f[s] += f[s ^ t];
            else f[s] -= f[s ^ t];
        }
    printf("%d\n", f[(1<<n)-1] * m / 2);
    return 0;
}