#include <bits/stdc++.h>
constexpr int N = 100005;
#ifdef CLANGD
#define N 5
#endif
int a[N], n, q, t;
template<typename T>constexpr inline T modInv(T x,T y){assert(x!=0);T u=0,v=1,a=x,m=y,t;while(a!=0){t=m/a;std::swap(a,m-=t*a);std::swap(u-=t*v,v);}assert(m==1);return u;}template<class Mod,typename Mod::value_type Default=0>requires std::integral<typename Mod::value_type>class MB{using Int=Mod::value_type;Int v;template<typename T>constexpr Int nrm(T x){if constexpr(std::is_unsigned_v<T>)return x<T(mod())?x:x%T(mod());else{Int res=-mod()<x&&x<mod()?x:x%mod();return(res<0?res+mod():res);}}public:static constexpr Int mod(){return Mod::value;}constexpr MB():v(Default){}template<typename T>constexpr MB(const T&r){v=nrm(r);}template<typename T>explicit constexpr operator T()const{return static_cast<T>(v);}constexpr Int operator()()const{return v;}constexpr MB&operator+=(const MB&r){if((v+=r.v)>=mod())v-=mod();return*this;}constexpr MB&operator-=(const MB&r){if((v-=r.v)<0)v+=mod();return*this;}constexpr MB&operator*=(const MB&r){if constexpr(std::is_same_v<Int,int>)v=nrm((uint64_t)v*r.v);else if constexpr(std::is_same_v<Int,int64_t>)v=nrm((unsigned __int128)v*r.v);else v=nrm(v*r.v);return*this;}constexpr MB&operator/=(const MB&r){return*this*=MB(modInv(r.v,mod()));}template<std::integral T>constexpr MB&operator^=(T n){if(n<0)assert(v!=0),v=1/v,n=-n;MB tmp=*this;for(*this=1;n;n>>=1){if(n&1)*this*=tmp;tmp*=tmp;}return*this;}constexpr MB operator-()const{return MB(-v);}constexpr MB&operator++(){return*this+=1;}constexpr MB&operator--(){return*this-=1;}constexpr MB operator++(int){MB tmp=*this;++*this;return tmp;}constexpr MB operator--(int){MB tmp=*this;--*this;return tmp;}constexpr bool operator!()const{return!v;}constexpr friend MB operator+(MB l,const MB&r){return l+=r;}constexpr friend MB operator-(MB l,const MB&r){return l-=r;}constexpr friend MB operator*(MB l,const MB&r){return l*=r;}constexpr friend MB operator/(MB l,const MB&r){return l/=r;}constexpr friend bool operator==(MB l,const MB&r){return l.v==r.v;}constexpr friend bool operator!=(MB l,const MB&r){return l.v!=r.v;}template<std::integral T>constexpr friend MB operator^(MB l,const T r){return l^=r;}template<typename IS>friend IS&operator>>(IS&is,MB&l){is>>l.v;l.v=l.nrm(l.v);return is;}template<typename OS>friend OS&operator<<(OS&os,const MB&r){return os<<r.v;}};
constexpr auto MOD = 10007;
using Mint = MB<std::integral_constant<std::decay_t<decltype(MOD)>, MOD>>;

struct Vec { Mint m[10]{}; };
struct Mat {
    Mint m[10][10];
    friend Mat operator*(const Mat &a, const Mat &b) {
        Mat c{};
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                for (int k = 0; k < 10; k++)
                    c.m[i][j] += c.m[i][k] * c.m[k][j];
        return c;
    }
};
Vec operator*(const Mat &a, const Vec &b) {
    Vec c{};
    for (int i = 0; i < 10; i++)
        for (int j = 0; j < 10; j++)
            c.m[i] += a.m[i][j] * b.m[j];
    return c;
}
struct SegTr {
    Mat tr[N << 2];
    int L, R;
    Mat inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1;
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return inq(u<<1|1, mid+1, r) * inq(u<<1, l, mid);
    }
    void build(int u=1, int l=1, int r=n) {
        if (l == r) {
            
            return;
        }
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        tr[u] = tr[u<<1|1] * tr[u<<1];
    }
    
} f;
int main() {
    scanf("%d%d%d", &n, &q, &t);
    for (int i = 1; i <= n; i++)
        scanf("%d", &a[i]);
    
    return 0;
}