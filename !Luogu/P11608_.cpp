#include <stdio.h>
#include <vector>
#include <array>
#include <algorithm>
constexpr int N = 1000005;
using LL = long long;
#include <cstdint>
#include <type_traits>
class FastIS{static constexpr size_t P=1<<20;char r[P];char*u,*G;FILE*I;public:FastIS(FILE*Y=stdin):u(nullptr),G(nullptr),I(Y){}__attribute__((always_inline))inline int get(){return(u==G)&&(G=(u=r)+fread(r,1,P,I),u==G)?EOF:*u++;}template<typename A,typename=typename std::enable_if<std::is_arithmetic<A>::value>::type>FastIS&operator>>(A&j){if _GLIBCXX17_CONSTEXPR(std::is_same<A,bool>::value){int a=get();while(~a&&a!=(char)(48)&&a!=(char)(49))a=get();j=static_cast<bool>(a&1);}else if _GLIBCXX17_CONSTEXPR(std::is_same<A,char>::value){j=get();while(j==(char)(32)||j==(char)(13)||j==(char)(10))j=get();}else if _GLIBCXX17_CONSTEXPR(std::is_integral<A>::value){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<A>::value){int a=get();while(~a&&(a<(char)(48)||a>(char)(57)))a=get();for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));}else{int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));j=m?-j:j;}}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<A>::value){int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;using E=typename std::conditional<std::is_same<A,float>::value,std::uint32_t,std::uint64_t>::type;E Q;for(Q=0;a>=(char)(48)&&a<=(char)(57);a=get())Q=(Q<<3)+(Q<<1)+(a^(char)(48));j=Q;if(a==(char)(46)){E a=1;for(a=get();a>=(char)(48)&&a<=(char)(57);a=get())j+=(a^(char)(48))*(1./(a*=10));}if(m)j=-j;}else{throw;}return*this;}FastIS&operator>>(char*i){int a=get();while(~a&&(a==(char)(32)||a==(char)(13)||a==(char)(10)))a=get();while(~a&&(a!=(char)(32)&&a!=(char)(13)&&a!=(char)(10)))*i++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double x=1e-9;char r[P],*b;const char*l;FILE*w;int o;public:FastOS(FILE*Y=stdout):b(r),l(r+P),w(Y),o(-1){setvbuf(w,nullptr,_IONBF,0);}~FastOS(){flush();fclose(w);}FastOS&flush(){fwrite(r,b-r,1,w);return*this;}FastOS&put(int g){*b++=g;if(b==l)fwrite(r,P,1,w),b=r;return*this;}FastOS&setprecision(const int&t){o=t;return*this;}template<typename a,typename=typename std::enable_if<std::is_arithmetic<a>::value>::type>FastOS&operator<<(a j){if _GLIBCXX17_CONSTEXPR(std::is_same<a,char>::value)put(j);else if _GLIBCXX17_CONSTEXPR(std::is_integral<a>::value){if _GLIBCXX17_CONSTEXPR(std::is_signed<a>::value)if(j<0)put((char)(45)),j=-j;static uint8_t i[40];i[*i=1]=j%10;while(j/=10)i[++(*i)]=j%10;while(*i)put(i[(*i)--]|(char)(48));}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<a>::value){if(j<0)put((char)(45)),j=-j;using E=typename std::conditional<std::is_same<a,float>::value,std::uint32_t,std::uint64_t>::type;E v=static_cast<E>(j);a B=j-v;*this<<v;if(o>0||(o<0&&B>x)){put((char)(46));for(int R=0;(o>0&&R<o)||o<0&&B>x;R++){int X=static_cast<int>(B*=10);put(X|(char)(48));B-=X;}}}else{throw;}return*this;}FastOS&operator<<(char*i){while(*i)put(*i++);return*this;}}fout;

struct Node {
    LL k, b;
    inline bool operator< (const Node &rhs) const {
        return k < rhs.k;
    }
} a[N];
std::vector<LL> ans;

namespace Splay {
enum {L, R, FA};
struct Node {
    int s[3];
    int sz;
    LL val, add;
    inline void pull(LL t) { val += t, add += t; }
    inline int& operator[](int x) { return s[x]; }
    Node& operator()(int);
};
std::array<Node, N> tr;
int tot = 0;
inline Node& Node::operator()(int x) { return tr[s[x]]; }
inline int alloc(LL val, int fa = 0) {
    ++tot;
    tr[tot].val = val, tr[tot].add = 0;
    tr[tot].sz = 1;
    tr[tot][FA] = fa, tr[tot][L] = tr[tot][R] = 0;
    return tot;
}
inline bool type(int u) { return tr[u](FA)[R] == u; }
inline void pushup(int u) { tr[u].sz = tr[u](L).sz + 1 + tr[u](R).sz; }
inline void pushdown(int u) {
    if (!tr[u].add) return; 
    if (tr[u][L]) tr[u](L).pull(tr[u].add);
    if (tr[u][R]) tr[u](R).pull(tr[u].add);
    tr[u].add = 0;
}
inline void rotate(int u) {
    pushdown(tr[u][FA]), pushdown(u);
    int anc = tr[u](FA)[FA];
    bool tp = type(u);
    if (anc) tr[anc][type(tr[u][FA])] = u;
    tr[u](FA)[tp] = tr[u][!tp];
    if (tr[u][!tp]) tr[u](!tp)[FA] = tr[u][FA];
    tr[u](FA)[FA] = u, tr[u][!tp] = tr[u][FA], tr[u][FA] = anc;
    pushup(tr[u][!tp]), pushup(u);
}
int root = 0;
inline void splay(int u, int goal = 0) {
    while (tr[u][FA] ^ goal) {
        if (tr[u](FA)[FA] != goal) rotate(type(u) == type(tr[u][FA]) ? tr[u][FA] : u);
        rotate(u);
    }
    if (!goal) root = u;
}

namespace Utils {
inline int rank(int ord) {
    int u = root;
    while (true) {
        pushdown(u);
        if (tr[u](L).sz + 1 == ord) return u;
        u = tr[u](L).sz < ord ? tr[u][L] : tr[u][R];
    }
}
bool atend;
inline int search(const ::Node &a) {
    int u = root, ans = -1, pre = 0;
    auto check = [&](int u) {
        return tr[u].val <= (LL)(pre + tr[u](L).sz) * a.k + a.b; // 注意左边的哨兵
    };
    while (u) {
        pushdown(u);
        if (check(u)) ans = u, u = tr[u][L];
        else pre += tr[u](L).sz + 1, u = tr[u][R];
    }
    if (ans == -1) {
        ans = root;
        while (tr[ans][R]) ans = tr[ans][R];
        atend = true;
        splay(ans);
        return tr[ans].sz;
    }
    atend = false;
    splay(ans);
    return tr[ans](L).sz;
}
inline void insert(LL val) { // 在当前 splay 的 root 的左边插一个节点
    if (!root) { root = alloc(val); return; }
    if (atend) {
        tr[root][R] = alloc(val, root);
        splay(tr[root][R]);
        return;
    }
    int lfront = tr[root][L];
    if (!lfront) {
        tr[root][L] = alloc(val, root);
        splay(tr[root][L]);
        return;
    }
    while (tr[lfront][R]) lfront = tr[lfront][R];
    splay(lfront, root);
    tr[lfront][R] = alloc(val, lfront);
    splay(tr[lfront][R]);
}
inline void addright(LL k) { // 在当前 root 的右孩子区间加一个数
    int u = tr[root][R];
    if (!u) return;
    tr[u].pull(k);
}
inline void printall(int u = root) {
    if (!u) return;
    pushdown(u);
    printall(tr[u][L]);
    printf("%lld ", tr[u].val);
    printall(tr[u][R]);
}
void getall(int u = root) {
    if (!u) return;
    pushdown(u);
    getall(tr[u][L]);
    ans.push_back(tr[u].val);
    getall(tr[u][R]);
}
} // namespace Utils
} // namespace Splay
using namespace Splay::Utils;
int main() {
    int n;
    fin >> n;
    for (int i = 1; i <= n; i++)
        fin >> a[i].k >> a[i].b;
    std::sort(a+1, a+1 + n);
/*
  f[i][j] = min(f[i-1][j], f[i-1][j-1] + (j-1) * a[i].k + a[i].b);
  g[i][j] = f[i][j] - f[i][j-1]
  每次一定是在已知最优集合中加一个当前天最优的，所以一定存在一个单调性
  g[i][j] = (j < k) g[i-1][j]
            (j = k) (j-1) * a[i].k + a[i].b
            (j > k) g[i-1][j-1] + a[i].k
  k 就是最小的 j 满足 g[i-1][j] <= (k-1) * a[i].k + a[i].b
*/
    for (int i = 1; i <= n; i++) { // 当前一轮：从 g[i-1][1..j-1] 转移到 g[i][1..j]
        int rk = search(a[i]);
        printf("rk = %d\n", rk);
        insert(rk * a[i].k + a[i].b);
        addright(a[i].k);
        printall(); puts("=======");
    }
    ans.reserve(n);
    getall();
    for (int i = 1; i < n; i++) ans[i] += ans[i-1];
    for (LL i : ans) fout << i << '\n';
    return 0;
}