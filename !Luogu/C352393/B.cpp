#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 34;
#else
constexpr int N = 300004;
#endif
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint pw2[N];
char s[N];
std::vector<int> g[N];
bool leaf[N];
void dfs(int u, int fa) {
    if (fa && g[u].size() == 1) { leaf[u] = 1; return; }
    for (int v : g[u]) if (v != fa) dfs(v, u);
}
int main() {
    int n, q;
    scanf("%d%d %s", &n, &q, s+1);
    pw2[0] = 1; for (int i = 1; i <= n; i++) pw2[i] = pw2[i-1] + pw2[i-1];
    for (int i = 1, u, v; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v), g[v].push_back(u);
    }
    dfs(1, 0);
    int ld = 0, nlq = 0;
    auto add = [&](int i, int x) {
        if (i == 1) return;
        if (leaf[i]) { if (s[i] == 'D') ld += x; }
        else { if (s[i] == '?') nlq += x; }
    };
    for (int i = 2; i <= n; i++) add(i, 1);
    auto output = [&]() {
        if (n == 1) {
            if (s[1] == '?') puts("2");
            else puts("1");
            return;
        }
        if (s[1] == 'U' || ld) puts("0");
        else printf("%d\n", pw2[nlq]);
    };
    output();
    for (int i = 1; i <= q; i++) {
        int x; char y;
        scanf("%d %c", &x, &y);
        add(x, -1); s[x] = y; add(x, 1);
        output();
    }
    return 0;
}