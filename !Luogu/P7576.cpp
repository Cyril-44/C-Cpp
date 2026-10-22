#include <bits/stdc++.h>
constexpr int N = 100005, M = 10;
#ifdef CLANGD
#define N 5
#endif
int a[N], n, q, t, T;
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 10007;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;

struct Vec {
    Mint m[M];
    Mint sum() const { return std::accumulate(m, m+T, Mint()); }
};
struct Mat {
    Mint m[M][M];
    friend Mat operator*(const Mat &a, const Mat &b) {
        Mat c{};
        for (int i = 0; i < T; i++)
            for (int j = 0; j < T; j++)
                for (int k = 0; k < T; k++)
                    c.m[i][j] += a.m[i][k] * b.m[k][j];
        return c;
    }
    Vec operator*(const Vec &b) const {
        Vec c{};
        for (int i = 0; i < T; i++)
            for (int j = 0; j < T; j++)
                c.m[i] += m[i][j] * b.m[j];
        return c;
    }
};
inline int tovecid(int i, int j) { return j * (j+1) / 2 + i; }
class SegTr {
    Mat tr[N << 2];
    int chain[80], L, R;
    void inq(int u, int l, int r) {
        if (L <= l && r <= R) { chain[++chain[0]] = u; return; }
        int mid = l + r >> 1;
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        inq(u<<1|1, mid+1, r), inq(u<<1, l, mid);
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) { // 令 f[u][i][j] 表示考虑到 u，当前 a[u] 加了 i，一共序列最少加了 j 次的贡献和
            for (int i = 0; i <= t; i++)
                for (int j = i; j <= t; j++) {
                    for (int k = 0; k < i; k++) // f[u][i][j] += f[u-1][k][j - (i-k)]
                        if (k <= j - (i-k)) tr[u].m[tovecid(i, j)][tovecid(k, j - (i-k))] += a[l] + i;
                    for (int k = i; k <= j; k++) // f[u][i][j] += f[u-1][k][j]
                        tr[u].m[tovecid(i, j)][tovecid(k, j)] += a[l] + i;
                }
            return;
        }
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1|1] * tr[u<<1];
    }
    Mint inquire(int l, int r) {
        L=l, R=r; inq(1, 1, n);
        Vec res{1};
        while (chain[0]) res = tr[chain[chain[0]--]] * res;
        return res.sum();
    }
} f;
int main() {
    scanf("%d%d%d", &n, &q, &t);
    T = (t+2) * (t+1) / 2;
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    f.build();
    for (int l, r; q--; ) {
        scanf("%d%d", &l, &r);
        printf("%d\n", f.inquire(l, r));
    }
    return 0;
}