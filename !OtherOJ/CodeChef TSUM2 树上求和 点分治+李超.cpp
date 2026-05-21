#include <bits/stdc++.h>
constexpr int N = 50005;
using i64 = long long;
constexpr i64 INF = 1e18;
std::vector<int> g[N];
int w[N];
bool ban[N];
template<typename b>concept IntegerWithI128=std::integral<b>||std::same_as<b,__int128_t>||std::same_as<b,__uint128_t>;class FastIS{static constexpr size_t d=1<<20;char e[d];char*f=nullptr,*g=nullptr;FILE*h;public:inline FastIS(FILE*i=stdin):h(i){}[[gnu::always_inline]]inline char get(){if(f==g){f=e;g=e+fread(e,1,d,h);if(f==g)return EOF;}return*f++;}inline FastIS&operator>>(bool&k){int l=get();while(~l&&l!='0'&&l!='1')l=get();k=(l=='1');return*this;}inline FastIS&operator>>(char&m){m=get();while(m==' '||m=='\r'||m=='\n'||m=='\t')m=get();return*this;}template<IntegerWithI128 n>inline FastIS&operator>>(n&o){int p=get();bool q=false;while(~p&&(p<'0'||p>'9')&&p!='-')p=get();if constexpr(std::is_signed_v<n>||std::same_as<n,__int128_t>){if(p=='-'){q=true;p=get();}}o=0;while(p>='0'&&p<='9'){o=(o<<3)+(o<<1)+(p^'0');p=get();}if(q)o=-o;return*this;}template<std::floating_point r>inline FastIS&operator>>(r&s){int t=get();bool u=false;while(~t&&(t<'0'||t>'9')&&t!='-')t=get();if(t=='-'){u=true;t=get();}__uint128_t v=0;while(t>='0'&&t<='9'){v=(v<<3)+(v<<1)+(t^'0');t=get();}s=static_cast<r>(v);if(t=='.'){r w=1.0;for(t=get();t>='0'&&t<='9';t=get()){s+=(t^'0')*(w/=10.0);}}if(u)s=-s;return*this;}inline FastIS&operator>>(char*x){int y=get();while(~y&&(y<=' '))y=get();while(~y&&(y>' '))*x++=y,y=get();*x='\0';return*this;}}fin;class FastOutputStream{static constexpr size_t d=1<<20;char e[d],*f=e;FILE*g;int h=-1;public:inline FastOutputStream(FILE*i=stdout):g(i){setvbuf(g,nullptr,_IONBF,0);}inline~FastOutputStream(){flush();}inline void flush(){fwrite(e,1,f-e,g);f=e;}[[gnu::always_inline]]inline void put(char l){if(f==e+d)flush();*f++=l;}inline FastOutputStream&setprecision(int n){h=n;return*this;}inline FastOutputStream&operator<<(char o){put(o);return*this;}inline FastOutputStream&operator<<(const char*p){while(*p)put(*p++);return*this;}FastOutputStream&operator<<(const std::string&q){for(char r:q)put(r);return*this;}template<IntegerWithI128 s>inline FastOutputStream&operator<<(s t){if(t==0){put('0');return*this;}s u=t;if constexpr(std::is_signed_v<s>||std::same_as<s,__int128_t>){if(u<0){put('-');u=-u;}}static char v[64];int w=0;while(u){v[w++]=static_cast<char>(u%10)^'0';u/=10;}while(w)put(v[--w]);return*this;}template<std::floating_point x>inline FastOutputStream&operator<<(x y){if(y<0){put('-');y=-y;}__uint128_t z=static_cast<__uint128_t>(y);*this<<z;x A=y-static_cast<x>(z);if(h>=0||A>1e-12){put('.');int B=(h>=0)?h:6;while(B--){A*=10;int C=static_cast<int>(A);put(C^'0');A-=C;}}return*this;}}fout;
int cursz;
namespace Centroid {
    int sz[N], mnsz, centroid;
    void dfs1(int u, int fa) {
        sz[u] = 1;
        for (int v : g[u]) if (!ban[v] && v != fa)
            dfs1(v, u), sz[u] += sz[v];
    }
    void dfs2(int u, int fa) {
        int mx = cursz - sz[u];
        for (int v : g[u]) if (!ban[v] && v != fa)
            dfs2(v, u), mx = std::max(mx, sz[v]);
        if (mx < mnsz) mnsz = mx, centroid = u;
    }
    inline int getCentroid(int u) {
        dfs1(u, 0); cursz = sz[u];
        mnsz = N; dfs2(u, 0);
        return centroid;
    }
} using Centroid::getCentroid;
struct SegSegTr {
    constexpr static int MXN = 50000;
    int tot;
    struct Line {
        i64 k, b;
        inline i64 operator()(int x) const { return (i64)k * x + b; }
    } tr[(MXN << 2) + 1];
    inline void init(int n) {
        std::fill(tr+1, tr+1+(n<<2), Line{0, -INF});
        tot = n;
    }
    inline void pull(int u, int l, int r, Line s) {
        while (l != r) {
            int mid = l + r >> 1;
            if (tr[u](mid) < s(mid)) std::swap(s, tr[u]);
            if (tr[u](l) < s(l)) u = u<<1, r = mid;
            else if (tr[u](r) < s(r)) u=u<<1|1, l = mid+1;
            else return;
        }
        if (tr[u](l) < s(l)) tr[u] = s;
    }
    inline void insert(i64 k, i64 b) { pull(1, 1, tot, Line{k, b}); }
    inline i64 inquireMxval(int pos) {
        i64 mx = tr[1](pos);
        int u = 1, l = 1, r = tot;
        while (l != r) {
            int mid = l + r >> 1;
            if (pos <= mid) u = u<<1, r = mid;
            else u = u<<1|1, l = mid+1;
            mx = std::max(mx, tr[u](pos));
        }
        return mx;
    }
} f;
i64 ans;
void dfsIns(int u, int fa, int dep, int sum, i64 val) { // 插入，{v} 的顺序是 ... -> fa -> u -> child -> ...
    ++dep; sum += w[u], val += (i64)dep * w[u];
    f.insert(sum, val); // 插入 sum * x + val
    for (int v : g[u]) if (!ban[v] && v != fa)
        dfsIns(v, u, dep, sum, val);
}
void dfsUpd(int u, int fa, int dep, int sum, i64 val) { // 统计贡献，{v} 的顺序是 ... -> child -> u -> fa -> ...
    ++dep; sum += w[u]; val += sum;
    ans = std::max(ans, val); // 从 u ---...---> root 不要漏掉了
    ans = std::max(ans, val + f.inquireMxval(dep));
    // fprintf(stderr, "  [%d]\t{%d}\t%d+%d\n", u, ans, val, f.inquireMxval(dep));
    for (int v : g[u]) if (!ban[v] && v != fa)
        dfsUpd(v, u, dep, sum, val);
}
/* 核心就是将贡献拆成 x ---...---> u ---...---> y 其中 x 和 y 都有 u 为祖先。
 * 会发现这样拆完之后，贡献变成 revVal_L + sum_L * count(u...v) + Val_R(include u)。
 * 容易注意到一次函数， k = sum_L, b = revVal_L。这样如果固定一定经过 u 的路径，可以在 O(sz log sz) 时间内统计答案。
 * 经典 Trick：点分治。对于每个子树递归时使用重心做为根，构造为点分树之后树高就是 log n 的，每一层都是 n log sz，加起来和 O(n log^2 n) 是一个量级。
 */
void solve(int u) {
    u = getCentroid(u);
    f.init(cursz);
    // static size_t rounds = 0, times = 0; fprintf(stderr, "%zur\t#%d\tΔ=%d\t$%zu\n", rounds += cursz, u, cursz, ++times);
    for (int v : g[u]) if (!ban[v]) {
        dfsUpd(v, u, 1, w[u], w[u]);
        dfsIns(v, u, 0, 0, 0);
    }
    ans = std::max(ans, (i64)w[u]); // 不要漏掉自己
    ans = std::max(ans, w[u] + f.inquireMxval(1)); // 从 root ---...---> x 不要漏掉
    // fprintf(stderr, "  ![%d]\t{%d}\t%d(+%d)\n", u, ans, w[u], f.inquireMxval(1));
    std::reverse(g[u].begin(), g[u].end());
    f.init(cursz);
    for (int v : g[u]) if (!ban[v]) {
        dfsUpd(v, u, 1, w[u], w[u]);
        dfsIns(v, u, 0, 0, 0);
    }
    std::reverse(g[u].begin(), g[u].end());
    ban[u] = true;
    for (int v : g[u]) if (!ban[v]) solve(v);
}
inline void solveSingle() {
    int n; fin >> n;
    memset(ban+1, 0, sizeof(bool) * n);
    for (int i = 1; i <= n; i++) {
        g[i].clear();
        fin >> w[i];
    }
    for (int u, v, i = 1; i < n; i++) {
        fin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    ans = -INF;
    solve(1);
    fout << ans << '\n';
}
int main() {
#ifndef ONLINE_JUDGE
    // freopen("CodeChef TSUM2.in", "r", stdin);
    // freopen("CodeChef TSUM2.out", "w", stdout);
#endif
    int T; fin >> T;
    while (T--) solveSingle();
    return 0;
}