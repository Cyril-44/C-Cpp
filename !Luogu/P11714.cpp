#include <bits/stdc++.h>
constexpr int N = 15;
unsigned ins[N], ous[N];
int n;
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
using Mint = MB<std::integral_constant<int, (int)1e9 + 7>>;
inline int count(unsigned s, unsigned t) { // 计算 s->t 的边数
    int cnt = 0;
    for (int i = 0; i < n; i++)
        if (t >> i & 1) cnt += __builtin_popcount(s & ins[i]);
    return cnt;
}
Mint f[1<<N], g[1<<N], pw2[N * N];
/*
考虑传统的 DAG 计数，f[S] = \sum_(T in S) (-1)^(|T|+1) * val1(T) * val2(S^T)，含义就是加入一个集合 T 并钦定其所有点入度为 0，然后加到 S^T 上。
本题，我们设 f[S] 表示 S 内点是连通块的方案数，g[T] 为辅助转移数组，表示 T 这个集合被缩成 x 个入度为 0 的点，x 为奇数贡献是正的，偶数是负的。
计算 f[S] 的核心思路是用所有连边方案 - 至少缩成两个连通块的方案
于是：f[S] = 2^e(|S|,|S|) - \sum_(T in.noteq S) g[T] * 2^e(T, S\T) * 2^e(S\T, S\T)
g[S] = f[S] - \sum_(T in S) f[T] * g[S \ T] （这里面不能再乘系数了，因为 g 要求内部点没有连边）
*/
inline int e(unsigned s, unsigned t) { // 计算 s-->t 的边的数量
    int res = 0;
    for (int i = 0; i < n; i++)
        if (t >> i & 1) res += __builtin_popcount(s & ins[i]);
    return res;
}
int ee[1<<N];
int main() {
    int m;
    scanf("%d%d", &n, &m);
    pw2[0] = 1;
    for (int i = 1; i <= m; i++)
        pw2[i] = pw2[i-1] + pw2[i-1];
    for (int u, v; m--; ) {
        scanf("%d%d", &u, &v);
        ins[v-1] |= 1u << u-1;
        ous[u-1] |= 1u << v-1;
    }
    for (unsigned s = 1; s < (1<<n); s++)
        ee[s] = e(s, s);
    for (unsigned s = 1; s < (1<<n); s++) {
        f[s] = pw2[ee[s]];
        for (unsigned _s = s & s-1, t = _s; t; t = (t-1) & s)
            if ((s&-s) == (t&-t)) g[s] -= f[t] * g[s^t];
        for (unsigned t = s; t; t = (t-1) & s)
            f[s] -= g[t] * pw2[e(t, s^t) + ee[s^t]];
        g[s] += f[s];
    }
    printf("%d\n", f[(1<<n)-1]);
}