#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 1, K = 1;
#else
constexpr int N = 54, K = 10;
#endif
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint f[2][N][1<<K];
/* 考虑 Product Trick, 将 \prod_i min_j{a_(i,j)} 转化为求 val_i 满足 \forall i, j, val_i <= a_i,j 的数量。
   再套一层转化，本题就是求所有的 val_i 中 a_i,j 满足 a_i,j >= val_i 的方案数之和
 * 我们称一列为一个排列，一行则是一个木桶。称钦定了点的行为特殊行，其余为普通行。
 * 设置 f[i][j][S] 表示已经考虑到了权值 i，并钦定了 j 行 以及 S 中的 val，且在矩阵中填写了 <= val 的所有数的方案数。
   权值从小往大枚举，考虑 i 的时候可以多钦定 k 行 val=i，与原来的状态进行归并，得到新的方案数。
      然后将 m' 个权值为 i 的数放到矩阵中，考虑每一列已经放置了 i-1 个数，一共钦定了 j 行 val，
      所以当前的 val 只能放在 j-(i-1) 这么多行上。每一列都是如此，所以乘上的系数就是 (j-i+1)^m'
    * 接下来考虑带上 S 怎么转移：
      可以类似于高维前缀和，每次在上一次的基础上考虑是不是要加入一行特殊行。
      注意到现在乘上的系数就不对了，进行修正：普通列假设有 m' 列，那乘上 (j + popcount(S) -i+1)^m'
      特殊列如果这一列已经有 i 设置过了，那系数直接是 1，否则可以填的位置数量应当是 j + popcount(S) - (已经钦定过 > i 的位置) - i+1
*/
Mint qpow(Mint b, int n) {
    Mint res = 1;
    while (n) {
        if (n & 1) res *= b;
        b *= b, n >>= 1;
    }
    return res;
}
std::vector<std::pair<int, int>> mp[N];
int mxreq[N]; // 特殊行对 val 的最大限度
std::vector<int> cols[1<<K]; // 在选中 S 中的特殊行的情况下有哪些特殊列
std::map<int,std::array<int, N>> ssum[1<<K]; // 在选中 S 中的特殊行的情况下，特殊列中权值桶的后缀和
int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    std::fill(mxreq+1, mxreq+1+n, n+1);
    for (int x, y, w; q--; ) {
        scanf("%d%d%d", &x, &y, &w);
        mp[x].emplace_back(y, w);
        mxreq[x] = std::min(mxreq[x], w);
    }
    int M = 0;
    for (int x = 1; x <= n; x++)
        if (mxreq[x] != n+1) {
            for (int s = 0; s < (1<<M); s++) {
                int t = s | 1<<M;
                cols[t] = cols[s];
                for (auto [y, w] : mp[x])
                    cols[t].push_back(y);
            }
            ++M;
        }
    for (int s = 0; s < (1<<M); s++) {
        std::sort(cols[s].begin(), cols[s].end());
        cols[s].erase(std::unique(cols[s].begin(), cols[s].end()), cols[s].end());
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                
    }
    for (int s = 0; s < (1<<M); s++) {
        
    }
    return 0;
}