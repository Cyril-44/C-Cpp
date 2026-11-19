#include <bits/stdc++.h>
constexpr int N = 2005;
#ifdef CLANGD
#define N 25
#endif
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
// constexpr auto MOD = (int)1e9 + 7;
// using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
struct Dynamic_ModInt { using value_type = int; static value_type value; };
Dynamic_ModInt::value_type Dynamic_ModInt::value = (int)1e9 + 7, &Mod = Dynamic_ModInt::value;
using Mint = MB<Dynamic_ModInt>;
struct Fact{Fact(const int n):aa(n+1,Mint(1)),ia(n+1),sz(n){aa[0]=1;for(int i=1;i<=n;i++)aa[i]=aa[i-1]*i;ia[n]=Mint(1)/aa[n];for(int i=n;i>=1;i--)ia[i-1]=ia[i]*i;}Mint C(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[m]*ia[n-m];}Mint A(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[n-m];}Mint F(int n)const{if(n<0)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n];}private:std::vector<Mint>aa,ia;const int sz;};
// Fact F(5000000);
Mint C[N][N], f[N][N], g[N][N], h[N][N], t[N], ans[N], pw2[N];
int main() {
    int n;
    scanf("%d%d", &n, &Mod);
    pw2[0] = 1;
    for (int i = 1; i <= n; i++) pw2[i] = pw2[i-1] + pw2[i-1];
    for (int i = 0; i <= n; i++) {
        C[i][0] = C[i][i] = 1;
        for (int j = 1; j < i; j++)
            C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    f[1][0] = g[1][0] = h[1][0] = 1;
    for (int i = 1; i < n; i++)
        for (int j = 0; j < i; j++) {
            f[i+1][j+1] += f[i][j], f[i+1][j] += f[i][j] * pw2[n-i-1];                      // f[i][j] 表示考虑了长度为 i（考虑了“1”），1,2 序列长度为 j 的套上 3 的方案数
            g[i+1][j+1] += g[i][j], g[i+1][j] += g[i][j] * (pw2[i]-1);                      // g[i][j] 表示考虑了长度为 i（考虑了“1”），1 序列长度为 j 的套上 2 的方案数
            h[i+1][j+1] += h[i][j] * (pw2[i-j-1]-1), h[i+1][j] += h[i][j] * (pw2[i-j]-1);   // h[i][j] 表示计算了前 i 个点的出边选择方案数，钦定前 i-1 个点中有 j 个无出边（每次考虑前一个的钦定情况）
        }
    for (int i = 1; i <= n; i++)
        for (int j = 0; j < i; j++)
            if (j & 1) t[i] -= h[i][j];
            else t[i] += h[i][j];
    Mint ans0 = f[n][0];
    for (int i = 1; i < n; i++)
        ans0 += f[n-1][i] * g[i+1][0];
    printf("%d 0", ans0);
    for (int i = 0; i < n; i++) // 计数方案：3 可以任意向后连边；然后是 2 可以往前面的 1,2 连边；内层的 1 由钦定零入边数量容斥得到（要求每一个点必须有入边和出边）。
        for (int j = 0; j <= i; j++)
            ans[j+2] += f[n-1][i] * g[i+1][j] * t[j+2];
    for (int i = 2; i <= n; i++)
        printf(" %d", ans[i]);
    putchar('\n');
}