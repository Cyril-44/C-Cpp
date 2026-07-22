#include <bits/stdc++.h>
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr int N = 5001, MOD = (int)1e9 + 7;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
#define For(i, s, t) for (int i = (s); i <= (t); i++)
std::vector<int> g[N];
Mint f[N][N], C[N][N], b[N], fact[N], psz[N], ipsz[N], ans[N];
int sz[N];
int n;
void dfs1(int u) {
    sz[u] = 1, psz[u] = 1;
    for (int v : g[u]) {
        dfs1(v);
        sz[u] += sz[v];
        psz[u] *= psz[v];
    }
    psz[u] *= sz[u];
    ipsz[u] = 1 / psz[u];
}
void dfs2(int u) {
    for (int v : g[u]) {
        Mint presum = 0;
        For(i, 1, n) {
            f[v][i] += presum;
            if (n-sz[v]-i >= sz[u]-sz[v]-1)
                presum += f[u][i] * C[n - sz[v] - i][sz[u] - sz[v] - 1] * fact[sz[u] - sz[v] - 1] * ipsz[u] * psz[v] * sz[u];
        }
        dfs2(v);
    }
    For(i, 1, n) if (n-i >= sz[u]-1) ans[u] += f[u][i] * b[i] * C[n - i][sz[u] - 1] * fact[sz[u]] * ipsz[u];
}
int main() {
    fact[0] = 1;
    For(i, 1, 5000) fact[i] = fact[i-1] * i;
    For(i, 0, 5000) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    scanf("%d", &n);
    For(i, 2, n) {
        int fa;
        scanf("%d", &fa);
        g[fa].push_back(i);
    }
    For(i, 1, n)
        scanf("%d", &b[i]);
    f[1][1] = 1;
    dfs1(1); dfs2(1);
    For(i, 1, n)
        printf("%d ", ans[i]);
    putchar('\n');
    return 0;
}