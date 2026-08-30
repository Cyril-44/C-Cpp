#include <bits/stdc++.h>
constexpr int N = 100004;
char s[N];
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
// struct Dynamic_ModInt { using value_type = int; static value_type value; };
// Dynamic_ModInt::value_type Dynamic_ModInt::value = 998244353, &Mod = Dynamic_ModInt::value;
// using Mint = MB<Dynamic_ModInt>;
struct Fact{Fact(const int n):aa(n+1,Mint(1)),ia(n+1),sz(n){aa[0]=1;for(int i=1;i<=n;i++)aa[i]=aa[i-1]*i;ia[n]=Mint(1)/aa[n];for(int i=n;i>=1;i--)ia[i-1]=ia[i]*i;}Mint C(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[m]*ia[n-m];}Mint A(int n,int m)const{if(n<0||m<0||n<m)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n]*ia[n-m];}Mint F(int n)const{if(n<0)[[unlikely]]return 0;if(n>sz)[[unlikely]]throw std::out_of_range("Expected n < "+std::to_string(sz)+", but found n = "+std::to_string(n)+".");return aa[n];}private:std::vector<Mint>aa,ia;const int sz;};
Fact F(100000);

int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d %s", &n, s+1);
        int mn = 2;
        int cnt[2]{};
        for (int i = 1; i <= n; i++) {
            int x = s[i] - '0';
            mn = std::min(mn, x);
            if (x > mn) ++cnt[x];
        }
        printf("%d\n", Mint(cnt[0]) * cnt[1]);
    }
    return 0;
}