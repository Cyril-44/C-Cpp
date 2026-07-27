#include <bits/stdc++.h>
constexpr int N = 300005;
#define For(i, s, t) for (int i = (s); i <= (int)((int)t); i++)
#define roF(i, s, t) for (int i = (s); i >= (int)((int)t); i--)
#define Forv(i, u) for (int i = 1, v; i <= (int)g[u].size() && (v = g[u][i-1]); i++) 
#define roFv(i, u) for (int i = (int)g[u].size(), v; i >= 1 && (v = g[u][i-1]); i--)
std::vector<int> g[N];
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint fact[N];
int n, k;
namespace BF {
    constexpr int N = 10;
    Mint f[N][N];
    void dfs(int u) {
        if (g[u].empty()) return void(std::fill(f[u], f[u]+n, 1));
        static Mint preProd[N], sufProd[N], prod[N]; // 计算 f[v][k-2] 的前缀积和后缀积，方便计算 \prod v!=v0 f[v][k-2]
        for (int v : g[u]) dfs(v);
        preProd[1] = 1, sufProd[g[u].size()] = 1;
        Forv(i, u) preProd[i+1] = preProd[i] * f[v][k-2];
        roFv(i, u) sufProd[i-1] = sufProd[i] * f[v][k-2], prod[i] = preProd[i] * sufProd[i];
        Forv(i, u) For(x, 1, n) f[u][x] += prod[i] * f[v][x-1];
        For(x, 1, n) f[u][x] *= fact[g[u].size() - 1];
    }
    inline void work() {
        memset(f, 0, sizeof f); dfs(1);
        printf("%d\n", f[1][n]);
    }
}
struct DP {
    Mint *arr=nullptr, mul=1, imul=1, add=0;
    int len=1; // 数组长度
    Mint operator[](int p) const { if (p >= len) p = len - 1; return arr[p] * mul + add; }
    void get(const DP&x) { mul = x.mul, add = x.add, len = x.len + 1; }
    void operator*=(Mint m) {
        if (m()) add *= m, mul *= m, imul /= m;
        else memset(arr, 0, sizeof(Mint) * len), add=0, mul=imul=1;
    }
    void operator+=(Mint a) { add += a; }
    void apply(int p, Mint val) {
        // arr[p] * mul + add = val (mul != 0)
        arr[p] = (val - add) * imul;
    }
} f[N];
Mint _f[N];
int len[N], lson[N], dfn[N], fa[N]; // len 是 0-based 最大深度
void dfs1(int u) {
    f[u].arr = _f + (dfn[u] = ++dfn[0]);
    if (!lson[u]) return;
    dfs1(lson[u]);
    for (int v : g[u]) if (v != lson[u]) dfs1(v);
}
void dfs2(int u) {
    if (!lson[u]) {
        f[u] += 1;
    fprintf(stderr, "=== Node %d === mul %d === add %d === len %d ===\n", u, f[u].mul, f[u].add, f[u].len);
        For(l, 0, len[u])
            fprintf(stderr, "%d%c", f[u][l], " \n"[l==len[u]]);
        return;
    }
    static Mint preProd[N], sufProd[N], prod[N]; // 计算 f[v][k-2] 的前缀积和后缀积，方便计算 \prod v!=v0 f[v][k-2]
    for (int v : g[u]) dfs2(v);
    preProd[1] = 1, sufProd[g[u].size()] = 1;
    Forv(i, u) preProd[i+1] = preProd[i] * f[v][k-2];
    roFv(i, u) sufProd[i-1] = sufProd[i] * f[v][k-2], prod[i] = preProd[i] * sufProd[i];
    
    Forv(i, u) if (v == lson[u]) {
        f[u].get(f[v]); // 复制长儿子系数
        f[u] *= prod[i];
        break;
    }
    Forv(i, u) if (v != lson[u])
        For(x, 0, len[v]) {
            fprintf(stderr, "v=%d f[%d][%d] <== %d + %d * %d\n", v, u, x+1, f[u][x+1], f[v][x], prod[i]);
            f[u].apply(x+1, f[u][x+1] + f[v][x] * prod[i]);
        }
    f[u] *= fact[g[u].size() - 1];
    
    fprintf(stderr, "=== Node %d === mul %d === add %d === len %d ===\n", u, f[u].mul, f[u].add, f[u].len);
    For(l, 0, len[u])
        fprintf(stderr, "%d%c", f[u][l], " \n"[l==len[u]]);
}
int main() {
    fact[0] = 1;
    For(i, 1, 300000) fact[i] = fact[i-1] * i;
    int T;
    scanf("%d", &T);
    For (_t, 1, T) {
        scanf("%d%d", &n, &k);
        For(i, 1, n) g[i].clear(), f[i] = DP();
        bool ischain = true;
        For(u, 2, n) {
            scanf("%d", &fa[u]);
            g[fa[u]].push_back(u);
            if (fa[u] != u-1) ischain = false;
        }
        if (k == 1) printf("%d\n", ischain ? 1 : 0);
        else {
            memset(len, 0, sizeof(int) * (n+1));
            memset(_f, 0, sizeof(Mint) * (n+1));
            roF(i, n, 2) {
                len[fa[i]] = std::max(len[fa[i]], len[i] + 1);
                if (!lson[fa[i]] || len[lson[fa[i]]] < len[i]) lson[fa[i]] = i;
            }
            dfn[0] = 0;
            dfs1(1), dfs2(1);
            printf("%d\n", f[1][len[1]]);
        }
    }
    return 0;
}