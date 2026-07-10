#include <bits/stdc++.h>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>class MB{using Int=typename Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<class T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<class T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename CharT, typename Traits>friend std::basic_istream<CharT, Traits>& operator>>(std::basic_istream<CharT, Traits>& is, MB& l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename CharT, typename Traits>friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os, const MB& r){return os<<r.v;}};
constexpr int MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;

constexpr int N = 1000005;
Mint pw2[N], ans[1 << 21];
int a[N];
int main() {
    pw2[0] = 1; For(i, 1, 1000000) pw2[i] = pw2[i-1] + pw2[i-1];
    std::ifstream fin("or.in");
    std::ofstream fout("or.out");
    int T, n, m; fin >> T;
    while (T--) {
        fin >> n >> m;
        For(i, 1, n) fin >> a[i];
        if (m == 0) fout << pw2[n] << '\n';
        else if (n <= 20) {
            int all = (1 << m+1) - 2;
            memset(ans, 0, sizeof(Mint) * (all + 1));
            for (int s = 0; !(s >> n); s++) {
                int or1 = 0, or2 = 0;
                For(i, 1, n) if (s >> (i-1) & 1) or1 |= a[i]; else or2 |= a[i];
                ++ans[or1 + or2];
            }
            For(i, 0, all) fout << ans[i] << ' ';
            fout << '\n';
        }
    }
    return 0;
}