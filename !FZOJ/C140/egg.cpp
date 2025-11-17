#include <stdio.h>
#include <string.h>
#include <vector>
#include <bits/stdc++.h>
using namespace std;
constexpr int N = 1005, M = 10005;
struct Edge {
    int to, nxt, w;
} e[M + N];
int head[N];
inline void addedg(int fr, int to, int w) {
    e[++head[0]] = Edge {to, head[fr], w};
    head[fr] = head[0];
}
constexpr double EPS = 1e-5, EPS2 = 1e-2;
int n;
double f[N];
int cnt[N];
bool vis[N];
class FastIS{static constexpr size_t P=1<<20;char t[P];char*y,*I;FILE*a;public:FastIS(FILE*G=stdin):y(nullptr),I(nullptr),a(G){}~FastIS(){fclose(a);}__attribute__((always_inline)) int b(){return(y==I)&&(I=(y=t)+fread(t,1,P,a),y==I)?EOF:*y++;}template<typename F>typename std::enable_if<std::is_unsigned<F>::value&&std::is_integral<F>::value,FastIS&>::type inline operator>>(F&K){int w=b();while(~w&&(w<'0'||w>'9'))w=b();for(K=0;w>='0'&&w<='9';w=b())K=(K<<3)+(K<<1)+(w^'0');return*this;}template<typename F>typename std::enable_if<std::is_signed<F>::value&&std::is_integral<F>::value&&!std::is_same<F,bool>::value&&!std::is_same<F,char>::value,FastIS&>::type inline operator>>(F&K){int w=b();bool d=false;while(~w&&(w<'0'||w>'9')&&(w^'-'))w=b();if(w=='-')w=b(),d=true;for(K=0;w>='0'&&w<='9';w=b())K=(K<<3)+(K<<1)+(w^'0');K=d?-K:K;return*this;}template<typename F>typename std::enable_if<std::is_floating_point<F>::value,FastIS&>::type inline operator>>(F&K){int w=b();bool d=false;while(~w&&(w<'0'||w>'9')&&(w^'-'))w=b();if(w=='-')w=b(),d=true;using u=typename std::conditional<std::is_same<F,float>::value,std::uint32_t,std::uint64_t>::type;u Z;for(Z=0;w>='0'&&w<='9';w=b())Z=(Z<<3)+(Z<<1)+(w^'0');K=Z;if(w=='.'){u D=1;for(w=b();w>='0'&&w<='9';w=b())K+=(w^'0')*(1./(D*=10));}if(d)K=-K;return*this;}inline FastIS&operator>>(bool&F){int w=b();while(~w&&w!='0'&&w!='1')w=b();F=static_cast<bool>(w&1);return*this;}inline FastIS&operator>>(char&O){int w=b();while(w==' '||w=='\r'||w=='\n')w=b();O=w;return*this;}inline FastIS&operator>>(char*O){int w=b();while(~w&&(w==' '||w=='\r'||w=='\n'))w=b();while(~w&&(w!=' '&&w!='\r'&&w!='\n'))*O++=w,w=b();return*this;}}fin;
double minusss;
inline bool spfa(int u) {
    int v; double w;
    static int q[N * M];
    int l = 1, r = 1;
    f[u] = 0, q[r++] = u; ++cnt[u];
    while (l < r) {
        u = q[l++];
        vis[u] = false;
        for (int i = head[u]; i; i = e[i].nxt) {
            int v = e[i].to;
            double w = e[i].w - minusss;
            if (f[v] > f[u] + w) {
                f[v] = f[u] + w;
                if (!vis[v]) {
                    vis[v] = true, q[r++] = v;
                    if (++cnt[v] > n) return true;
                }
            }
        }
    }
    return false;
}
bool check(double mid) {
    minusss = mid;
    memset(cnt, 0, sizeof(int) * (n+2));
    memset(vis, 0, n+2);
    memset(f, 0x7f, sizeof f);
    return spfa(n+1);
}
int main() {
    int m;
    fin >> n >> m;
    for (int u, v, w; m--; ) {
        fin >> u >> v >> w;
        addedg(u, v, w);
    }
    for (int i = 1; i <= n; i++)
        addedg(n+1, i, 0);
    double l = 0, r = 1e7+1, mid;
    while (l + EPS2 <= r) {
        mid = (l + r) / 2;
        if (check(mid)) r = mid - EPS; // 有环，即最大的环平均值 < mid
        else l = mid + EPS;
    }
    if (r > 1e7) puts("PaPaFish is laying egg!");
    else printf("%.2lf\n", l);
    return 0;
}
