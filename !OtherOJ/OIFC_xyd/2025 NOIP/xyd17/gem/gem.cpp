#include <stdio.h>
#include <string.h>
#include <vector>
#include <tuple>
#include <queue>
#include <cstdint>
#include <string>
#include <chrono>
#include <random>
#include <algorithm>
constexpr int N = 2005;
constexpr double EPS = 1e-7;
class FastIS{static constexpr size_t P=1<<20;char r[P];char*u,*G;FILE*I;public:FastIS(FILE*Y=stdin):u(nullptr),G(nullptr),I(Y){}__attribute__((always_inline))inline int get(){return(u==G)&&(G=(u=r)+fread(r,1,P,I),u==G)?EOF:*u++;}template<typename A,typename=typename std::enable_if<std::is_arithmetic<A>::value>::type>FastIS&operator>>(A&j){if _GLIBCXX17_CONSTEXPR(std::is_same<A,bool>::value){int a=get();while(~a&&a!=(char)(48)&&a!=(char)(49))a=get();j=static_cast<bool>(a&1);}else if _GLIBCXX17_CONSTEXPR(std::is_same<A,char>::value){j=get();while(j==(char)(32)||j==(char)(13)||j==(char)(10))j=get();}else if _GLIBCXX17_CONSTEXPR(std::is_integral<A>::value){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<A>::value){int a=get();while(~a&&(a<(char)(48)||a>(char)(57)))a=get();for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));}else{int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));j=m?-j:j;}}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<A>::value){int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;using E=typename std::conditional<std::is_same<A,float>::value,std::uint32_t,std::uint64_t>::type;E Q;for(Q=0;a>=(char)(48)&&a<=(char)(57);a=get())Q=(Q<<3)+(Q<<1)+(a^(char)(48));j=Q;if(a==(char)(46)){E a=1;for(a=get();a>=(char)(48)&&a<=(char)(57);a=get())j+=(a^(char)(48))*(1./(a*=10));}if(m)j=-j;}else throw;return*this;}FastIS&operator>>(char*i){int a=get();while(~a&&(a==(char)(32)||a==(char)(13)||a==(char)(10)))a=get();while(~a&&(a!=(char)(32)&&a!=(char)(13)&&a!=(char)(10)))*i++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double x=1e-9;char r[P],*b;const char*l;FILE*w;int o;public:FastOS(FILE*Y=stdout):b(r),l(r+P),w(Y),o(-1){setvbuf(w,nullptr,_IONBF,0);}~FastOS(){flush();fclose(w);}FastOS&flush(){fwrite(r,b-r,1,w);return*this;}FastOS&put(int g){*b++=g;if(b==l)fwrite(r,P,1,w),b=r;return*this;}FastOS&setprecision(const int&t){o=t;return*this;}template<typename a,typename=typename std::enable_if<std::is_arithmetic<a>::value>::type>FastOS&operator<<(a j){if _GLIBCXX17_CONSTEXPR(std::is_same<a,char>::value)put(j);else if _GLIBCXX17_CONSTEXPR(std::is_integral<a>::value){if _GLIBCXX17_CONSTEXPR(std::is_signed<a>::value)if(j<0)put((char)(45)),j=-j;static uint8_t i[40];i[*i=1]=j%10;while(j/=10)i[++(*i)]=j%10;while(*i)put(i[(*i)--]|(char)(48));}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<a>::value){if(j<0)put((char)(45)),j=-j;using E=typename std::conditional<std::is_same<a,float>::value,std::uint32_t,std::uint64_t>::type;E v=static_cast<E>(j);a B=j-v;*this<<v;if(o>0||(o<0&&B>x)){put((char)(46));for(int R=0;(o>0&&R<o)||(o<0&&B>x);R++){int X=static_cast<int>(B*=10);put(X|(char)(48));B-=X;}}}else throw;return*this;}FastOS&operator<<(char*i){while(*i)put(*i++);return*this;}FastOS&operator<<(const std::string&s){return*this<<s.c_str();}}fout;
int v[N], n, K;
std::vector<std::tuple<int,int,int,double>> g[N];
inline void build(double x) {
    for (int i = 1; i <= n; i++)
        for (auto& [j, a, b, s] : g[i])
            s = a * x + b;
}
class Solve {
    virtual bool check() = 0;
public:
    inline void operator()() {
        build(0);
        if (!check()) return puts("-1"), void();
        double l = 0, r = 1e9, x;
        while (l + EPS < r) {
            x = (l + r) / 2;
            build(x);
            if (check()) l = x;
            else r = x - EPS;
        }
        printf("%.8lf\n", l);
    }
};
template<typename T>
struct PriorityQueue : public std::priority_queue<T, std::vector<T>, std::greater<T>> {
    PriorityQueue(int n) { this->c.reserve(n); }
};
class BruteForce : public Solve {
    double dis[N];
    bool vis[N];
    inline bool check() override {
        using Pdi = std::pair<double, int>;
        // __gnu_pbds::priority_queue<Pdi, std::greater<Pdi>> pq;
        PriorityQueue<Pdi> pq(n);
        for (int s = 1; s <= n; s++) {
            memset(dis, 0x7f, sizeof dis);
            memset(vis, 0, sizeof vis);
            pq.push(Pdi(0, s));
            dis[s] = 0;
            while (!pq.empty()) {
                auto [s, u] = pq.top(); pq.pop();
                if (vis[u]) continue;
                vis[u] = true;
                for (const auto &[v, a, b, w] : g[u]) {
                    if (!vis[v] && dis[v] > s + w) {
                        dis[v] = s + w;
                        pq.push(Pdi(dis[v], v));
                    }
                }
            }
            int cnt = 0;
            for (int i = 1; i <= n; i++)
                if (dis[i] <= v[i]) ++cnt;
            if (cnt >= K)
                return true;
        }
        return false;
    }
};
class Std {
    double dis[N];
    bool vis[N];
    struct Pdi { double w; int id; inline bool operator< (const Pdi &rhs) const { return w > rhs.w; } };
    inline bool check(int s, double x) {
        std::priority_queue<Pdi> pq;
        memset(dis, 0x7f, sizeof dis);
        memset(vis, 0, sizeof vis);
        pq.push({0, s});
        dis[s] = 0;
        while (!pq.empty()) {
            auto tp = pq.top(); pq.pop();
            double s = tp.w; int u = tp.id;
            if (vis[u]) continue;
            vis[u] = true;
            for (const auto &[v, a, b, _] : g[u]) {
                double w = a * x + b;
                if (!vis[v] && dis[v] > s + w) {
                    dis[v] = s + w;
                    pq.push({dis[v], v});
                }
            }
        }
        int cnt = 0;
        for (int i = 1; i <= n; i++)
            if (dis[i] <= v[i]) ++cnt;
        return cnt >= K;
    }
    int arr[N];
public:
    inline void operator()() {
        bool flg = false;
        for (int i = 1; i <= n; i++)
            if (check(i, 0)) {flg = true; break;}
        if (!flg) return puts("-1"), void();
        std::iota(arr, arr+n, 1);
        std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        std::shuffle(arr, arr+n, rng);
        double ans = 0.;
        for (int i = 0; i < n; i++) {
            if (!check(arr[i], ans)) continue;
            double l = ans, r = 1e9, x;
            while (l + EPS < r) {
                x = (l + r) / 2;
                if (check(arr[i], x)) l = x;
                else r = x - EPS;
            }
            ans = l;
        }
        printf("%.8lf\n", ans);
    }
};
class K_2 : public Solve {
    inline bool check() override {
        for (int i = 1; i <= n; i++)
            for (const auto &[j, a, b, w] : g[i])
                if (w <= v[j]) return true;
        return false;
    }
};
class TreePts : public Solve {
    int cnt;
    void dfs(int u, int fa, double w) {
        if (w <= v[u]) ++cnt;
        for (const auto [v, a, b, w1] : g[u]) {
            if (v == fa) continue;
            dfs(v, u, w + w1);
        }
    }
    inline bool check() override {
        for (int i = 1; i <= n; i++) {
            cnt = 0;
            dfs(i, i, 0);
            if (cnt >= K) return true;
        }
        return false;
    }
};
static BruteForce solveBF;
static K_2 solveK_2;
static TreePts solveTree;
static Std solve;
int main() {
    freopen("gem.in", "r", stdin);
    freopen("gem.out", "w", stdout);
    int m, id;
    fin >> n >> m >> K >> id;
    if (K == 1) return puts("999999999.9999999"), 0;
    for (int i = 1; i <= n; i++)
        fin >> v[i];
    for (int i = 1, u, v, a, b; i <= m; i++) {
        fin >> u >> v >> a >> b;
        g[u].emplace_back(v, a, b, .0);
    }
    switch (id) {
    case 4: case 5:
        solveK_2(); break;
    case 6: case 7: case 8:
        solveTree(); break;
    case 1: case 2: case 3: case 9: case 10: case 11: case 12:
        solveBF(); break;
    default:
        solve();
    }
    return 0;
}