#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 2, M = 10;
#else
constexpr int N = 20, M = 10001;
#endif
#define For(i, s, t) for (int i = (s); i <= (t); i++)
int a[N], b[M], setb[M], sa[1<<N], sb[1<<N];
std::bitset<1<<N> fans;
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
struct Fact{Fact(const int n):aa(n+1,Mint(1)),ia(n+1),sz(n){aa[0]=1;for(int i=1;i<=n;i++)aa[i]=aa[i-1]*i;ia[n]=Mint(1)/aa[n];for(int i=n;i>=1;i--)ia[i-1]=ia[i]*i;}Mint C(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[m]*ia[n-m];}Mint A(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[n-m];}Mint F(int n)const{if(n<0)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n];}private:std::vector<Mint>aa,ia;const int sz;};
Fact F(2000000);
Mint f[1<<N], g[1<<N];
/* 本题使用 Hall 定理。
 * 目标是让公司作为左部点不能完美匹配。
 * 因为左部点太多，改为枚举右部点。
 * 考虑一个右部点集合，对应的左部点集合中每一个点一定满足，其连接的右部点集合是当前枚举的子集。
 * 于是最严的限制就是所有连接右部点集合为枚举子集的左部点的 a_i 之和。
 * 考虑如何没有完美匹配：就是每个右部点集合 b_j 之和 都 < 对应的左部点 a_i 之和。也就是求 min{sum a_i - sum b_j + 1}
 * 
 */
int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    For(i, 0, n-1) scanf("%d", &a[i]);
    For(j, 1, m) scanf("%d", &b[j]);
    For(i, 0, n-1) For(j, 1, m) {
        int v; scanf("%d", &v);
        setb[j] |= v << i;
    }
    For(j, 1, m) sb[setb[j]] += b[j];
    For(i, 0, n-1) For(s, 0, (1<<n)-1)
        if (s >> i & 1) sb[s] += sb[s ^ (1<<i)];
    For(s, 1, (1<<n)-1) {
        int x = __builtin_ctz(s);
        sa[s] = sa[s ^ (1<<x)] + a[x];
    }
    int ansx = 0x7fffffff;
    For(s, 1, (1<<n)-1) if (sa[s] && sb[s]) ansx = std::min(ansx, sa[s] - sb[s] + 1);
    if (ansx <= 0) { puts("0 1"); return 0; }
    For(s, 1, (1<<n)-1) if (ansx == sa[s] - sb[s] + 1) fans[s] = true;
    For(i, 0, n-1) For(s, 0, (1<<n)-1)
        if ((s >> i & 1) && fans[s]) fans.set(s ^ (1<<i));
    For(s, 0, (1<<n)-1) f[s] = F.C(sa[s], ansx);
    For(i, 0, n-1) For(s, 0, (1<<n)-1)
        if (s >> i & 1) f[s] -= f[s ^ (1<<i)];
    Mint ans = 0;
    For(s, 0, (1<<n)-1)
        if (fans[s]) ans += f[s];
    printf("%d %d\n", ansx, ans);
    return 0;
}