#include <bits/stdc++.h>
#define LUOGU
#if defined(ONLINE_JUDGE) && !defined(LUOGU)
# pragma GCC optimize(2, 3, "inline", "unroll-loops", "fast-math", "inline-small-functions", "no-stack-protector", "delete-null-pointer-checks")
# pragma GCC target("tune=native")
#endif
#define Inline __attribute__((always_inline)) inline
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
#define Forv(i, s, t, ...) for (int i = (s), __VA_ARGS__; i <= (t); ++i)
#define roF(i, t, s) for (int i = (t); i >= (s); --i)
#define roFv(i, t, s, ...) for (int i = (t), __VA_ARGS__; i >= (s); --i)
#define Rep(c) for (int tempFor_count = c; tempFor_count; --tempFor_count)
#define Repv(c, ...) for (int tempFor_count = c, __VA_ARGS__; tempFor_count; --tempFor_count)
#define YES return cout << "Yes\n", void()
#define NO return cout << "No\n", void()
#define YESNO(j) cout << ((j) ? "Yes\n" : "No\n")
#define EXIT(s...) return (cout << s), void();
using namespace std;using pii_t=pair<int,int>;using pll_t=pair<int64_t,int64_t>;using veci_t=vector<int>;using vecl_t=vector<int64_t>;Inline int Popcnt(int x){return __builtin_popcount((unsigned)x);}Inline int Popcnt(unsigned x){return __builtin_popcount(x);}Inline int Popcnt(int64_t x){return __builtin_popcountll((uint64_t)x);}Inline int Popcnt(uint64_t x){return __builtin_popcountll(x);}Inline int Log2(int x){return 31-__builtin_clz((unsigned)x|1);}Inline int Log2(unsigned x){return 31-__builtin_clz(x|1);}Inline int Log2(int64_t x){return 63-__builtin_clzll((uint64_t)x|1);}Inline int Log2(uint64_t x){return 63-__builtin_clzll(x|1);}
namespace Solution{
// #define MULTI_TEST_CASES
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;

constexpr int N = 100005, M = 300005;

inline void globalInit() {
    
}
Mint pw2[M], ans;
int n, m;
class SumProdSegTr {
    struct Node {
        Mint sum = 0, mul = 1;
        void pull(Mint t) { sum *= t, mul *= t; }
    } tr[N << 2];
    void pushup(int u) {
        tr[u].sum = tr[u<<1].sum + tr[u<<1|1].sum;
    }
    void pushdown(int u) {
        if (tr[u].mul != 1) {
            tr[u<<1].pull(tr[u].mul);
            tr[u<<1|1].pull(tr[u].mul);
            tr[u].mul = 1;
        }
    }
    int L, R; Mint X;
    void updmul(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) updmul(u<<1, l, mid);
        if (mid < R) updmul(u<<1|1, mid+1, r);
        pushup(u);
    }
    void updadd(int u, int l, int r) {
        if (l == r) { tr[u].sum += X; return; }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) updadd(u<<1, l, mid);
        else updadd(u<<1|1, mid+1, r);
        pushup(u);
    }
    Mint inqsum(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].sum;
        int mid = l + r >> 1; Mint res = 0;
        pushdown(u);
        if (L <= mid) res += inqsum(u<<1, l, mid);
        if (mid < R) res += inqsum(u<<1|1, mid+1, r);
        return res;
    }
    void updc(int u, int l, int r) {
        if (r < L || l > R) return;
        tr[u] = Node{};
        if (l != r) {
            int mid = l + r >> 1;
            updc(u<<1, l, mid);
            updc(u<<1|1, mid+1, r);
        }
    }
public:
    void rangeMul(int l, int r, Mint x) { if (l <= r) L=l, R=r, X=x, updmul(1, 0, n); }
    void posAdd(int p, Mint x) { L=p, X=x; updadd(1, 0, n); }
    Mint inquireSum(int l, int r) { L=l, R=r; return inqsum(1, 0, n); }
    void clear(int l, int r) { L=l-1, R=r; updc(1, 0, n); }
} f;
class SegTr {
    vector<pii_t> rgs[N << 2];
    int cnt[N << 2], cntr[N << 2];
    int L, R;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) ++cnt[u];
        else {
            ++cntr[u];
            rgs[u].emplace_back(max(L, l), min(R, r));
            int mid = l + r >> 1;
            if (L <= mid) upd(u<<1, l, mid);
            if (mid < R) upd(u<<1|1, mid+1, r);
        }
    }
    void dfs(int u, int l, int r) {
        sort(rgs[u].begin(), rgs[u].end(), [](const pii_t &x, const pii_t &y) { return x.second < y.second; });
        f.posAdd(l-1, 1);
        for (auto [_l, _r] : rgs[u]) {
            f.posAdd(_r, f.inquireSum(_l-1, _r));
            f.rangeMul(l-1, _l-2, 2);
        }
        Mint ways = f.inquireSum(r, r);
        // fprintf(stderr, "cur=%d, intersect=%d, ways=%d\n", cnt[u], cntr[u], ways);
        ans += (pw2[cnt[u]]-1) * (pw2[cntr[u]] - ways) * (pw2[m-cnt[u]-cntr[u]]);
        f.clear(l, r);
        if (l != r) {
            int mid = l + r >> 1;
            dfs(u<<1, l, mid), dfs(u<<1|1, mid+1, r);
        }
    }
public:
    void add(int l, int r) { L=l, R=r; upd(1, 1, n); }
    void solve() { dfs(1, 1, n); }
} seg;

inline void solveSingleTestCase() {
    cin >> n >> m;
    pw2[0] = 1;
    For(i, 1, m) pw2[i] = pw2[i-1] + pw2[i-1];
    Repv(m, l, r) {
        cin >> l >> r;
        seg.add(l, r);
    }
    seg.solve();
    cout << ans << '\n';
}
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    Solution::globalInit();
    int testCases = 1;
#ifdef MULTI_TEST_CASES
    cin >> testCases;
#endif
    while (testCases--) Solution::solveSingleTestCase();
    return 0;
}