#include <bits/stdc++.h>
constexpr int N = 505;
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
struct Dynamic_ModInt { using value_type = int; static value_type value; };
Dynamic_ModInt::value_type Dynamic_ModInt::value = (int)1e9 + 7, &Mod = Dynamic_ModInt::value;
using Mint = MB<Dynamic_ModInt>;
Mint f[2][N][N];
/* f[S] g[T] 表示非叶子集合为 S, T 的第一、二颗树的方案数
f'[S] g'[T] 表示非叶子集合 S' \in S, T' \in T 的第一、二颗树的方案数之和
将第 i 个点接在一个非叶子下面：
    钦定 i 为叶子：f'[S] <- f'[S] * |S|
    i 可能为非叶子：f'[S \cup {i}] f'[S] * |S|
所以有答案是 \sum_{S \cap T = \emptyset, S \cup \T = U} f[S] * g[T]
    = \sum_{S' \cap T' = \emptyset} f'[S'] g'[T'] (-2)^(|U| - |S'| - |T'|)
f[i][j][k] 表示考虑了前 i 个点的系数，满足 S' \cap T' = \emptyset 且 |S'| \cap {1,2,\dots,i} = j, |T'| \cap {i,i+1,\dots,n} = k
*/
int main() {
    int n;
    scanf("%d%d", &n, &Mod);
    for (int k = 1; k < n; k++)
        f[1][1][k] = 1;
    for (int i = 2; i <= n; i++) { // 考虑 i 在第一颗树中的父亲 和 i-1 在第二棵树中的父亲
        memset(f[i&1], 0, sizeof(f) >> 1);
        Mint ans = 0;
        for (int j = 1; j < i; j++) {
            for (int k = 1; j+k <= n+1; k++) {
                Mint jk = j * k;
                f[i&1][j+1][k] += f[i&1^1][j][k] * jk; // i \in S'
                f[i&1][j][k-1] += f[i&1^1][j][k] * jk; // i \in T'
                f[i&1][j][k]   -= f[i&1^1][j][k] * jk * 2; // i 在两颗树都是叶子
            }
            ans += f[i&1^1][j][1] * j; // 注意系数
        }
        printf("%d\n", ans);
    }
    return 0;
}