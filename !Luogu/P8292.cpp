#include <cstdio>
#include <vector>
#include <cstring>
#include <cassert>
#include <cstdint>
#include <type_traits>
#include <set>
constexpr int N = 2001, _N = 1000001, DVD = 43, M = 13, P = 305; // 43 以下有13个质数
template<typename T>constexpr inline T modInv(T x,T y){T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
int h[N];
bool np[N];
int primes[P], pmap[N];
int cnt[P][1<<M];
Mint pw2[_N], ipw2[_N], g[P][1<<M], f[1<<M];
inline void initPrimes() {
    for (int i = 2; i < N; i++) {
        if (!np[i]) pmap[i] = primes[0], primes[++primes[0]] = i;
        for (int j = 1; j <= primes[0] && i * primes[j] < N; j++) {
            np[i * primes[j]] = true;
            if (i % primes[j] == 0) break;
        }
    }
}

/*
经典容斥 Trick：选择集合包含 {s1, s2, ..., sk} 的贡献和 = f(所有集合) - f(至少一个元素不在 S 中的集合) + f(至少两个元素不在 S 中的集合) - ...
经典根号分治：小质数容斥，大质数暴力加到容斥中
令 f[s] 表示钦定不选集合为 S 的贡献和
*/

int main() {
    initPrimes();
    pw2[0] = ipw2[0] = 1; constexpr Mint i2 = Mint(1) / 2;
    for (int i = 1; i < _N; i++) pw2[i] = pw2[i-1] + pw2[i-1], ipw2[i] = ipw2[i-1] * i2;
    int n, m;
    scanf("%d", &n);
    for (int i = 1, ai; i <= n; i++)
        scanf("%d", &ai), ++h[ai];
    for (int v = 1; v < N; v++) {
        if (!h[v]) continue;
        std::vector<int> facs;
        int x = v;
        for (int ip = 1; ip <= M; ip++)
            if (x % primes[ip] == 0) {
                facs.push_back(primes[ip]);
                do x /= primes[ip]; while (x % primes[ip] == 0);
            }
        if (x == 43 * 43) x = 43; // 傻逼边界操死你妈
        if (x > 1) x = pmap[x];
        else x = 0;
        int mask = 0;
        for (int p : facs) mask |= 1 << pmap[p];
        for (int s = 0; s < (1<<M); s++)
            if (!(s & mask)) cnt[x][s] += h[v];
    }
    for (int s = 0; s < (1<<M); s++)
        f[s] = pw2[cnt[0][s]];
    for (int x = M; x < P; x++)
        for (int s = 0; s < (1<<M); s++)
            f[s] *= pw2[cnt[x][s]],
            g[x][s] = 1 - ipw2[cnt[x][s]];
    scanf("%d", &m);
    for (int c; m--; ) {
        scanf("%d", &c);
        std::set<int> bigPrimeIDs;
        int mask = 0;
        for (int x; c--; ) {
            scanf("%d", &x);
            if (x >= DVD) bigPrimeIDs.insert(pmap[x]);
            else mask |= 1 << pmap[x];
        }
        Mint nf[1<<M];
        memcpy(nf, f, sizeof f);
        for (int p : bigPrimeIDs)
            for (int s = 0; s < (1<<M); s++)
                nf[s] *= g[p][s];
        Mint ans = 0;
        for (int s = 0; s < (1<<M); s++)
            if ((s & mask) == s)
                if (__builtin_popcount(s) & 1) ans -= nf[s];
                else ans += nf[s];
        printf("%d\n", ans);
    }
}