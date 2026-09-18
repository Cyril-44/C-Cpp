#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 1;
#else
constexpr int N = 1004;
#endif
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint f[N][N*3], tf[N*3];
std::vector<int> g[N], ig[N];
int sz[N];
void dfs(int u, int fa) {
    sz[u] = 3;
    for (int v : g[u]) if (v != fa) dfs(v, u);
    for (int v : ig[u]) if (v != fa) dfs(v, u);
    for (int v : g[u]) if (v != fa) { // 外向边
        memset(tf, 0, sizeof(Mint) * (sz[u] + sz[v] + 1));
        roF(i, sz[u], 1) roF(j, sz[v], 1)
            tf[i+j] += f[u][i] * f[v][j];
        sz[u] += sz[v];
        memcpy(f[u], tf, sizeof(Mint) * (sz[u] + 1));
    }
    for (int v : ig[u]) if (v != fa) { // 内向边 = 无边 - 外向边
        memset(tf, 0, sizeof(Mint) * (sz[u] + sz[v] + 1));
        roF(i, sz[u], 1)
            roF(j, sz[v], 1) {
                tf[i] += f[u][i] * f[v][j];
                tf[i+j] -= f[u][i] * f[v][j];
            }
        sz[u] += sz[v];
        memcpy(f[u], tf, sizeof(Mint) * (sz[u] + 1));
    }
    for (int i = 1; i <= sz[u]; i++) f[u][i] /= i;
}
int main() {
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        int sum = 0;
        for (int j = 1, aij; j <= 3; j++) {
            scanf("%d", &aij);
            f[i][j] = j * aij;
            sum += aij;
        }
        Mint invsum = Mint(1) / Mint(sum);
        for (int j = 1; j <= 3; j++)
            f[i][j] *= invsum;
    }
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        ig[v].push_back(u);
    }
    dfs(1, 0);
    Mint ans = 0;
    for (int i = 1; i <= 3*n; i++)
        ans += f[1][i];
    printf("%d\n", ans);
    return 0;
}