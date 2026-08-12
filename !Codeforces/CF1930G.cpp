#include <bits/stdc++.h>
#define For(i, s, t) for(int i = (s); i <= (t); i++)
constexpr int N = 1000005;
std::vector<int> g[N];
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
int n;
struct BIT {
    Mint tr[N];
    void init(int n) { memset(tr, 0, n+1 << 2); }
    Mint sum(int p) {
        Mint res = 0;
        for (; p > 0; p -= p & -p) res += tr[p];
        return res;
    }
    Mint sum(int l, int r) { return sum(r) - sum(l-1); }
    void upd(int p, Mint x) { for (; p <= n; p += p & -p) tr[p] += x; }
} fs;
int mx[N];
Mint f[N];
void dfsmx(int u, int fa) {
    mx[u] = u;
    for (int v : g[u]) if (v != fa) {
        dfsmx(v, u);
        mx[u] = std::max(mx[u], mx[v]);
    }
}
void dfs(int u, int fa, int x) {
    if (x > u) f[u] = 0;
    else f[u] = fs.sum(x, u) + f[x];
    for (int v : g[u]) if (v != fa) {
        dfs(v, u, std::max(x, u));
    }
    for (int v : g[u]) if (v != fa)
        fs.upd(mx[v], -f[mx[v]]);
    fs.upd(mx[u], f[mx[u]]);
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        For(i, 1, n) g[i].clear();
        For(i, 1, n-1) {
            int u, v;
            scanf("%d%d", &u, &v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        dfsmx(1, 0);
        For(i, 1, n) std::sort(g[i].begin(), g[i].end(), [](int x, int y) { return mx[x] < mx[y]; });
        fs.init(n);
        f[0] = 1;
        dfs(1, 0, 0);
        printf("%d\n", f[n]);
    }
    return 0;
}