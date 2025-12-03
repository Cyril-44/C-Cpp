#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <type_traits>
#include <cstdint>
#include <string>
#include <random>
#include <chrono>
class FastIS{static constexpr size_t P=1<<20;char r[P];char*u,*G;FILE*I;public:FastIS(FILE*Y=stdin):u(nullptr),G(nullptr),I(Y){}__attribute__((always_inline))inline int get(){return(u==G)&&(G=(u=r)+fread(r,1,P,I),u==G)?EOF:*u++;}template<typename A,typename=typename std::enable_if<std::is_arithmetic<A>::value>::type>FastIS&operator>>(A&j){if _GLIBCXX17_CONSTEXPR(std::is_same<A,bool>::value){int a=get();while(~a&&a!=(char)(48)&&a!=(char)(49))a=get();j=static_cast<bool>(a&1);}else if _GLIBCXX17_CONSTEXPR(std::is_same<A,char>::value){j=get();while(j==(char)(32)||j==(char)(13)||j==(char)(10))j=get();}else if _GLIBCXX17_CONSTEXPR(std::is_integral<A>::value){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<A>::value){int a=get();while(~a&&(a<(char)(48)||a>(char)(57)))a=get();for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));}else{int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));j=m?-j:j;}}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<A>::value){int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;using E=typename std::conditional<std::is_same<A,float>::value,std::uint32_t,std::uint64_t>::type;E Q;for(Q=0;a>=(char)(48)&&a<=(char)(57);a=get())Q=(Q<<3)+(Q<<1)+(a^(char)(48));j=Q;if(a==(char)(46)){E a=1;for(a=get();a>=(char)(48)&&a<=(char)(57);a=get())j+=(a^(char)(48))*(1./(a*=10));}if(m)j=-j;}else throw;return*this;}FastIS&operator>>(char*i){int a=get();while(~a&&(a==(char)(32)||a==(char)(13)||a==(char)(10)))a=get();while(~a&&(a!=(char)(32)&&a!=(char)(13)&&a!=(char)(10)))*i++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double x=1e-9;char r[P],*b;const char*l;FILE*w;int o;public:FastOS(FILE*Y=stdout):b(r),l(r+P),w(Y),o(-1){setvbuf(w,nullptr,_IONBF,0);}~FastOS(){flush();fclose(w);}FastOS&flush(){fwrite(r,b-r,1,w);return*this;}FastOS&put(int g){*b++=g;if(b==l)fwrite(r,P,1,w),b=r;return*this;}FastOS&setprecision(const int&t){o=t;return*this;}template<typename a,typename=typename std::enable_if<std::is_arithmetic<a>::value>::type>FastOS&operator<<(a j){if _GLIBCXX17_CONSTEXPR(std::is_same<a,char>::value)put(j);else if _GLIBCXX17_CONSTEXPR(std::is_integral<a>::value){if _GLIBCXX17_CONSTEXPR(std::is_signed<a>::value)if(j<0)put((char)(45)),j=-j;static uint8_t i[40];i[*i=1]=j%10;while(j/=10)i[++(*i)]=j%10;while(*i)put(i[(*i)--]|(char)(48));}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<a>::value){if(j<0)put((char)(45)),j=-j;using E=typename std::conditional<std::is_same<a,float>::value,std::uint32_t,std::uint64_t>::type;E v=static_cast<E>(j);a B=j-v;*this<<v;if(o>0||(o<0&&B>x)){put((char)(46));for(int R=0;(o>0&&R<o)||(o<0&&B>x);R++){int X=static_cast<int>(B*=10);put(X|(char)(48));B-=X;}}}else throw;return*this;}FastOS&operator<<(char*i){while(*i)put(*i++);return*this;}FastOS&operator<<(const std::string&s){return*this<<s.c_str();}}fout;
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) ((void)0)
#endif
#define umin(x, y) ((x) = std::min((x), (y)))
#define umax(x, y) ((x) = std::max((x), (y)))
constexpr int N = 600005;
struct Node { int op, t, x, y, ty; } a[N];
int ans[N];
int pos[N];
namespace BIT {
    int tr[N];
    inline void init() { memset(tr, -0x3f, sizeof(int) * (pos[0] + 1)); }
    inline void upd(int p, int x) {
        for (; p <= pos[0]; p += p & -p) umax(tr[p], x);
    }
    inline int query(int p) {
        int mx = -0x7fffffff;
        for (; p > 0; p -= p & -p) umax(mx, tr[p]);
        return mx;
    }
}
void cdq(int l, int r) {
    if (l >= r) return;
    int mid = l + r >> 1;
    pos[0] = 0;
    for (int i = l; i <= r; i++)
        pos[++pos[0]] = a[i].y;
    std::sort(pos+1, pos+1 + pos[0]);
    pos[0] = std::unique(pos+1, pos+1 + pos[0]) - pos;
    for (int i = l; i <= r; i++)
        a[i].ty = std::lower_bound(pos+1, pos + pos[0], a[i].y) - pos;
    std::nth_element(a+l, a+mid + 1, a+r + 1, [](const Node &a, const Node &b){return a.t < b.t;});
    std::sort(a+l, a+mid+1, [](const Node &a, const Node &b){return a.x < b.x;});
    std::sort(a+mid+1, a+r+1, [](const Node &a, const Node &b){return a.x < b.x;});
#define Update(L, DL, R, DR, CP, OP, VL, VR)          \
    BIT::init();                                      \
    for (int pl = L, pr = R; pr CP DR; OP pr) {       \
        for (; pl CP DL && a[pl].x CP a[pr].x; OP pl) \
            if (a[pl].op == 1) {                      \
                BIT::upd((VR 1 > 0) ? a[pl].ty :      \
                        pos[0] - a[pl].ty,            \
                    VL a[pl].x VR a[pl].y);           \
                debug("%d ==> (%d, %d) %d\n", a[pl].ty, a[pl].x, a[pl].y, VL a[pl].x VR a[pl].y); \
            }                                         \
        if (a[pr].op == 2) {                          \
            umin(ans[a[pr].t],                        \
                VL a[pr].x VR a[pr].y - BIT::query(   \
                (VR 1 > 0) ? a[pr].ty :               \
                  pos[0] - a[pr].ty));                \
            debug("[%d] %d (%d, %d) <== %d = (%d) - (%d)\n", a[pr].t, a[pr].ty, a[pr].x, a[pr].y, VL a[pr].x VR a[pr].y - BIT::query((VR 1 > 0) ? a[pr].ty : pos[0] - a[pr].ty), VL a[pl].x VR a[pr].y, BIT::query((VR 1 > 0) ? a[pr].ty : pos[0] - a[pr].ty)); \
        }                                             \
    }
    Update(l, mid, mid+1, r, <=, ++, +, +) // 处理 xl<=xr && yl<=yr
    Update(l, mid, mid+1, r, <=, ++, +, -) // 处理 xl<=xr && yl>=yr
    Update(mid, l, r, mid+1, >=, --, -, +) // 处理 xl>=xr && yl<=yr
    Update(mid, l, r, mid+1, >=, --, -, -) // 处理 xl>=xr && yl>=yr
    cdq(l, mid), cdq(mid+1, r);
}
int main() {
    std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> Gen(1, 100);
    Gen(rng);
    int n, m;
    fin >> n >> m;
    for (int i = 1; i <= n; i++) {
        fin >> a[i].x >> a[i].y;
        a[i].op = 1, a[i].t = i;
    }
    memset(ans, 0x7f, sizeof ans);
    for (int i = n + 1; i <= n + m; i++) {
        fin >> a[i].op >> a[i].x >> a[i].y;
        a[i].t = i;
        if (a[i].op == 1) ans[i] = -1;
    }
    cdq(1, n + m);
    for (int i = n+1; i <= n+m; i++)
        if (~ans[i]) printf("%d\n", ans[i]);
    return 0;
}
