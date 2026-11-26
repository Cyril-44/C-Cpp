#include <bits/stdc++.h>
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
constexpr int N = 505;
char s[N];
int w[N];
Mint f[2][N][N], fac[N];
/*
f[i][j][k] 表示考虑到第 i 天，共寄掉 j 个人，已经选中了 k 个人的顺序（其余 n-k 个人任意排列）
*/
int main() {
    int n, m;
    scanf("%d%d %s", &n, &m, s+1);
    for (int i = 1, ai; i <= n; i++)
        scanf("%d", &ai), ++w[ai];
    for (int i = 1; i <= n; i++)
        w[i] += w[i-1];
    fac[0] = 1;
    for (int i = 1; i <= n; i++) fac[i] = fac[i-1] * i;
    auto fnow = f[0], fnxt = f[1];
    fnow[0][0] = 1;
    for (int i = 1; i <= n; i++) {
        memset(fnxt, 0, sizeof(f) / 2);
        if (s[i] == '0')
            for (int j = 0; j < i; j++)
                for (int k = 0; k < i; k++)
                    fnxt[j+1][k] = fnow[j][k];
        else
            for (int j = 0; j < i; j++)
                for (int k = 0; k < i; k++) {
                    fnxt[j][k] += fnow[j][k];
                    int cnt = w[j] - k;
                    if (cnt > 0) {
                        fnxt[j][k+1] -= fnow[j][k] * cnt;
                        fnxt[j+1][k+1] += fnow[j][k] * cnt;
                    }
                }
        std::swap(fnow, fnxt);
    }
    Mint ans = 0;
    for (int j = 0; j <= n - m; j++)
        for (int k = 0; k <= n; k++)
            ans += fac[n - k] * fnow[j][k];
    printf("%d\n", ans);
;}
