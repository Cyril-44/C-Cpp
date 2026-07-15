#include <cstdio>
#include <algorithm>
#include <cassert>
#include <type_traits>
#include <cstdint>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
constexpr int N = 500005, MOD = 998244353;
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>class MB{using Int=typename Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<class T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend MB operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend MB operator!=(MB l,const MB&r){return l.v!=r.v;}template<class T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;
using pii_t = std::pair<int,int>;
pii_t a[N];
int n, m;
struct Mat {
    Mint a[2][2] = {1, 0, 0, 1};
    const Mint* operator[](bool x) const { return a[x]; }
    Mint* operator[](bool x) { return a[x]; }
    friend Mat operator*(const Mat& x, const Mat& y) {
        Mat res{0};
        For(k, 0, 1) For(i, 0, 1) For(j, 0, 1)
            res[i][j] += x[i][k] * y[k][j];
        return res;
    }
    Mat& operator*=(const Mat& o) { return *this = *this * o; }
};
struct Vec {
    Mint a[2];
    Mint operator[](bool x) const { return a[x]; }
    Mint& operator[](bool x) { return a[x]; }
    friend Vec operator*(const Mat& x, const Vec& y) {
        Vec res{};
        For(k, 0, 1) For(i, 0, 1)
            res[i] += x[i][k] * y[k];
        return res;
    }
    friend Vec operator+(const Vec& x, const Vec& y) {
        return Vec{x[0]+y[0], x[1]+y[1]};
    }
    Mint sum() const { return a[0] + a[1]; }
};
struct SegTr {
    Vec tr[N << 2]; Mat lz[N << 2];
    void pushup(int u) {
        tr[u] = tr[u<<1] + tr[u<<1|1];
    }
    void pushdown(int u) {
        lz[u<<1] *= lz[u]; tr[u<<1] = lz[u] * tr[u<<1];
        lz[u<<1|1] *= lz[u]; tr[u<<1|1] = lz[u] * tr[u<<1|1];
        lz[u] = Mat();
    }
    Mint X1; int P;
    const Mat X2{2,1,0,1}; int L, R;
    void upd1(int u, int l, int r) {
        if (l == r) { tr[u][1] += X1; return; }
        int mid = l + r >> 1;
        pushdown(u);
        if (P <= mid) upd1(u<<1, l, mid);
        else upd1(u<<1|1, mid+1, r);
        pushup(u);
    }
    void upd2(int u, int l, int r) {
        if (L <= l && r <= R) { lz[u] *= X2, tr[u] = X2 * tr[u]; return; }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd2(u<<1, l, mid);
        if (mid < R) upd2(u<<1|1, mid+1, r);
        pushup(u);
    }
    Vec inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1;
        pushdown(u);
        if (R <= mid) return inq(u<<1, l, mid);
        if (L > mid) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
    void update1(int pos, Mint val) {
        P = pos, X1 = val; upd1(1, 2, n+1);
    }
    void update2(int l, int r) {
        L = l, R = r; upd2(1, 2, n+1);
    }
    Vec inquire(int l, int r) {
        L = l, R = r; return inq(1, 2, n+1);
    }
} f;
#define L first
#define R second
struct Sub1 {
    constexpr static int N = 2005;
    Mint f[N][2];
    void work() {
        For(i, 1, m) {
            for (int j = a[i].R; j >= a[i].L; j--)
                f[a[i].R][1] += f[j][0] + f[j][1];
            ++f[a[i].R][1];
            for (int j = 2; j < a[i].L; j++)
                f[j][0] += f[j][0] + f[j][1];
        }
        Mint ans;
        For(i, 2, n+1) ans += f[i][1];
        printf("%d\n", ans);
    }
};
int main() {
    freopen("a.in", "r", stdin);
    freopen("a.out", "w", stdout);
    scanf("%d%d", &n, &m);
    For(i, 1, m) {
        scanf("%d%d", &a[i].L, &a[i].R);
        ++a[i].R;
    }
    std::sort(a+1, a+1+m, [](const pii_t& x, const pii_t& y){ return x.R < y.R; });
    if (n <= 2000) { Sub1{}.work(); return 0; }
    For(i, 1, m) {
        f.update1(a[i].R, f.inquire(a[i].L, a[i].R).sum() + 1);
        if (2 < a[i].L) f.update2(2, a[i].L - 1);
    }
    printf("%d\n", f.inquire(2, n+1)[1]);
    return 0;
}
