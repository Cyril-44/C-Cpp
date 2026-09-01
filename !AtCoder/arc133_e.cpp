#include <bits/stdc++.h>
/*
经典转化：将中位数和的计数问题转化为，>=x 为 1，< x 为 0，然后枚举 x 统计 >=x 的数量之和，就是最终答案。
本题考虑这种转化，(0,a,1) ==> a 否则 (0,a,0) ==> 0, (1,a,1) ==> 1
所以我们关心最后一次 x_i = y_j 的地方，这决定了最终值。设 f(x, 0/1) 表示最后一次为 0/1 的方案数。
因为 f(x,0) 表示 1~x-1, x~n 分段取 0，由于对称性知道 1~V+1-x, V+2-x~n 分段取 1 即 f(V+2-x,1) 与前式相等。
所以对于 x >= 2，统计 1 的贡献就是 **总方案数** /2.
现在考虑最终仍然是 a 的情况。先假设还是只有 01，手玩一下发现：x 和 y 连边会形成 gcd(n,m) 个连通块，每一个连通块内必须一边全为 0 另外一边全为 1.
扩展到全域，令 g(x) 表示方案数：g(x) = ((x-1) ^ (n / gcd(n,m)) * (V-x+1) ^ (m / gcd(n,m)) + (V-x+1) ^ (n / gcd(n,m)) * (x-1) ^ (m / gcd(n,m))) ^ gcd(n,m)
对于 x=1, 因为所有数都 >=1, 直接有 f(1,0)=0, f(1,1)=V^(n+m) 就可。
上述的总方案数，就是 (V-1) * V^(n+m) - sum_(i=2)^V g(i).
最后答案就是 a 的贡献，加上 sum_(i=1)^n f(i, 1).
*/
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
int main() {
    int n, m, v, a;
    scanf("%d%d%d%d", &n, &m, &v, &a);
    int g = std::__gcd(n, m), ng = n / g, mg = m / g;
    Mint ans = Mint(v) ^ (n+m), sumg;
    for (int x = 1; x < v; x++) {
        Mint gx = ((Mint(x) ^ ng) * (Mint(v-x) ^ mg) + (Mint(v-x) ^ ng) * (Mint(x) ^ mg)) ^ g;
        if (a > x) ans += gx;
        sumg += gx;
    }
    ans += ((v - 1) * (Mint(v) ^ (n+m)) - sumg) / 2;
    printf("%d\n", ans);
    return 0;
}