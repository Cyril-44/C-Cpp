#include <bits/stdc++.h>
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 998244353;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;

#ifdef CLANGD
constexpr int N = 54;
#else
constexpr int N = 500004;
#endif
std::vector<int> g[N];
int a[N], n;
class SegTr {
    struct Node {
        Mint val, mul = 1;
        int ls, rs;
        void pull(Mint m) { val *= m, mul *= m; }
    } tr[N * 40];
    int top, rt[N];
    void pushup(int u) { tr[u].val = tr[tr[u].ls].val + tr[tr[u].rs].val; }
    void pushdown(int u) {
        if (tr[u].mul != 1) {
            if (tr[u].ls) tr[tr[u].ls].pull(tr[u].mul);
            if (tr[u].rs) tr[tr[u].rs].pull(tr[u].mul);
            tr[u].mul = 1;
        }
    }
    int P; Mint X, Y;
    void upd(int& u, int l, int r) {
        if (!u) u = ++top;
        if (l == r) { tr[u].val = X; return; }
        int mid = l + r >> 1; pushdown(u);
        if (P <= mid) upd(tr[u].ls, l, mid);
        else upd(tr[u].rs, mid+1, r);
        pushup(u);
    }
    void merge(int& u, int v, int l, int r) {
        if (!u && !v) return;
        if (!u) return tr[u = v].pull(X);
        if (!v) return tr[u].pull(Y+1);
        if (l == r) { tr[u].val = tr[u].val * (tr[v].val + Y+1) + tr[v].val * X; return; }
        int mid = l + r >> 1; pushdown(u), pushdown(v);
        merge(tr[u].ls, tr[v].ls, l, mid);
        merge(tr[u].rs, tr[v].rs, mid+1, r);
        pushup(u);
    }
    Mint inq(int u, int l, int r) {
        if (!u) return 0;
        if (l == r) return tr[u].val;
        int mid = l + r >> 1; pushdown(u);
        return P <= mid ? inq(tr[u].ls, l, mid) : inq(tr[u].rs, mid+1, r);
    }
public:
    void apply(int u, int p, Mint v) { P=p, X=v; upd(rt[u], 1, n); }
    void merge(int u, int v) { X = (*this)(u, a[u]), Y = (*this)(v, a[v]); merge(rt[u], rt[v], 1, n); }
    Mint operator()(int u) { return tr[rt[u]].val; }
    Mint operator()(int u, int pos) { P=pos; return inq(rt[u], 1, n); }
} f;
Mint ans;
void dfs(int u, int fa) {
    f.apply(u, a[u], 1);
    for (int v : g[u]) if (v != fa) {
        dfs(v, u);
        if (a[u] == a[v]) {
            Mint fix = f(u, a[u]) * (f(v, a[v]) + 1);
            f.merge(u, v);
            f.apply(u, a[u], fix);
        } else {
            f.apply(u, a[v], f(u, a[v]) + (f(u, a[u]) + f(u, a[v])) * (f(v, a[u]) + f(v, a[v])));
        }
    }
    ans += f(u);
}
int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    for (int u, v, i = 1; i < n; i++) {
        scanf("%d%d", &u, &v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    dfs(1, 0);
    printf("%d\n", ans());
    return 0;
}