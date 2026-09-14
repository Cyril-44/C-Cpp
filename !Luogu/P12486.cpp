#include <bits/stdc++.h>
#ifdef CLANGD
constexpr int N = 1, K = 1;
#else
constexpr int N = 54, K = 10;
#endif
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
Mint C[N][N], f[1<<K][N];
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
int mxreq[N], xid[N]; // 特殊行对 val 的最大限度
std::map<int,uint64_t> flg[1<<K]; // 在选中 S 中的特殊行的情况下，特殊列中权值桶的后缀和
int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    For(i, 0, n) {
        C[i][0] = C[i][i] = 1;
        For(j, 1, i-1) C[i][j] = C[i-1][j-1] + C[i-1][j];
    }
    std::fill(mxreq+1, mxreq+1+n, n+1);
    for (int y, x, w; q--; ) {
        scanf("%d%d%d", &y, &x, &w);
        mp[x].emplace_back(y, w);
        mxreq[x] = std::min(mxreq[x], w);
    }
    int M = 0;
    For(x, 1, n)
        if (mxreq[x] != n+1)
            xid[M++] = x;
    For(s, 0, (1<<M)-1)
        For(x, 0, M-1)
            if (s >> x & 1)
                for (auto [y, w] : mp[xid[x]])
                    flg[s][y] |= 1ull << w;
    f[0][0] = 1;
    For(i, 1, n) {
        For(s, 0, (1<<M)-1) // 先做普通行的钦定
            roF(j, n-M, 1)
                roF(k, j-1, 0)
                    f[s][j] += f[s][k] * C[j][k];
        For(x, 0, M-1) // 再利用高维前缀和结构，钦定特殊行
            For(s, 0, (1<<M)-1)
                if ((s >> x & 1) && mxreq[xid[x]] >= i)
                    For(j, 0, n-M)
                        f[s][j] += f[s^(1<<x)][j];
        For(s, 0, (1<<M)-1) // 最后计算往里面放入 m 个 i 之后的方案数
            For(j, 0, n-M) {
                int base = j - i + 1 + __builtin_popcount(s); // 在普通列中放置 "i" 的方案数
                if (base <= 0) { f[s][j] = 0; continue; }
                Mint mul = qpow(base, m - flg[s].size());
                for (const auto& [y, yflg] : flg[s])
                    if (!(yflg >> i & 1)) {
                        int val = base - __builtin_popcountll(yflg & ~((1ull<<i)-1));
                        if (val <= 0) { mul = 0; break; }
                        mul *= val;
                    }
                f[s][j] *= mul;
            }   
    }
    printf("%d\n", f[(1<<M)-1][n-M]);
    return 0;
}