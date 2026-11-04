#include <bits/stdc++.h>
namespace TL {
#define infile(filename) freopen(filename".in","r",stdin)
#define outfile(filename) freopen(filename".out","w",stdout)
#define usefile(filename) infile(filename),outfile(filename)
using namespace std; typedef long long ll; typedef unsigned long long ull; typedef __int128 I;
namespace IO {
	const int BUF=1<<20; static char ch[BUF]={},out[BUF]={},*l=ch,*r=ch,*o=out;
#define FASTIO
#ifdef FASTIO
	inline char gc() { return (l==r&&(r=(l=ch)+fread(ch,1,BUF,stdin),l==r))?EOF:*l++; }
#else
	inline char gc() { return getchar(); }
#endif
	inline void flush() { fwrite(out,1,o-out,stdout),o=out; }
	inline void putc(char ch) { if(o==out+BUF) flush(); *o++=ch; }
	struct flusher{~flusher(){flush();}}_;
}; using IO::gc; using IO::putc;
template <typename T> void read(T &a) { static char fushu,ch; a=fushu=0; do ch=gc(); while(ch!='-'&&(ch<48||ch>57)); if(ch=='-') ch=gc(),fushu=1; do a=(a<<1)+(a<<3)+(ch^48),ch=gc(); while(ch>47&&ch<58); if(fushu) a=-a; }
template <typename T,typename ...Args> void read(T &a,Args &...args) { read(a),read(args...); }
template <typename T> void write(T a) { static char que[114]={},*p=que; if(!a) putc(48); if(a<0) putc('-'),a=-a; while(a) *p++=(a%10)^48,a/=10; while(p!=que) putc(*--p); putc(32); }
template <typename T,typename ...Args> void write(T a,Args ...args) { write(a),write(args...); }
template <typename T> void ckmax(T &a,T b) { if(a<b) a=b; }
template <typename T> void ckmin(T &a,T b) { if(b<a) a=b; }
} using TL::read; using TL::write;
constexpr int N = 100005, MOD = (int)1e9 + 7;
#ifdef CLANGD
#define N 15
#endif
std::vector<int> g[N];
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this;  }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    template<typename T> explicit operator T() const { return static_cast<T>(val); }
    Mint pow(int n) const {
        Mint b = *this, res = 1;
        for (; n; n >>= 1, b *= b) if (n&1) res*=b;
        return res;
    }
    Mint(int v=0) : val(v) {}
private: int val;
};
int n;
struct SegTr {
    struct Node { // f[p][q] 表示 1 选(p=1)/不选(p=0) 和 2 的点的状态
        Mint fbase, cnt, nonlvcnt; uint8_t flg; // 用 base 来记录 f[0][0], flg 记录增量，最低位记录 2 是否为叶子。(有结论：增量只可能是以下六种情况)
        int operator[](int p) const { return (flg>>2<<2) >> p*2 & 3; }
        constexpr static uint8_t GetFlg(int f01, int f10, int f11) { return f01 | f10<<2 | f11<<4; }
        friend Node operator+(const Node &a, const Node &b) {
            Node res{};
            res.cnt = a.cnt + (a.cnt - 1) * (b.cnt - 2);
            if (b.flg&1) res.nonlvcnt = (a.cnt - 1) * (b.nonlvcnt - 1) + a.nonlvcnt;
            else res.nonlvcnt = (a.cnt - 1) * (b.nonlvcnt - 1) + 1;
            res.flg = (a.flg&1) & (b.flg&1);
            Mint base = (a.cnt - 1) * b.fbase; // 一个都不选
            for (int s = 0; s < 4; s++) {
                Mint now = base;
                switch (b.flg >> 2) { // 这里面的节点数量均要去除点 1, 2 状态的影响
                case GetFlg(0, 0, 0): // 没有额外贡献
                break;
                case GetFlg(0, 0, 1): // 两边都选额外贡献 + 1，额外贡献 = 最大匹配
                    now += a.fbase+a[s];
                break;
                case GetFlg(1, 0, 1): // 上端选额外贡献 + 1, 额外贡献 = 非叶子节点数量
                    now += a.nonlvcnt - !(s&1) - (!(s&2) & !(a.flg&1));
                break;
                case GetFlg(0, 1, 1): // 下端选额外贡献 + 1, 额外贡献 = 非根节点数量
                    now += a.cnt-1 - !(s&2);
                break;
                case GetFlg(1, 1, 1): // 哪一端选额外贡献都 + 1, 额外贡献 = min(边数，节点数量)
                    now += a.cnt - std::max(2 - __builtin_popcount(s), 1);
                break;
                case GetFlg(1, 1, 2): // 两端都选额外贡献 + 2, 额外贡献 = 节点数量
                    now += a.cnt - 2 + __builtin_popcount(s);
                break;
                }
                if (s) assert(int(now-res.fbase) <= 2), res.flg |= int(now - res.fbase) << s*2;
                else res.fbase = now;
            }
            return res;
        }
    } tr[N << 2];
    void build(int x=1, int l=1, int r=n) {
        if (l != r) {
            int mid = l + r >> 1;
            build(x<<1, l, mid), build(x<<1|1, mid+1, r);
            tr[x] = tr[x<<1] + tr[x<<1|1];
            // fprintf(stderr, "[%d,%d]: ", l, r);
            // for (int s = 0; s < 4; s++) fprintf(stderr, "%d%c", tr[x].fbase + tr[x][s], " \n"[s==3]);
            return;
        }
        int m; read(m); tr[x].cnt = m;
        for (int i = 1; i <= m; i++) g[i].clear();
        for (int u, v, i = 1; i < m; i++) {
            read(u, v);
            g[u].push_back(v);
            g[v].push_back(u);
        }
        static int fa[N], que[N], f[N][2];
        tr[x].nonlvcnt = tr[x].flg = 0;
        for (int hd = 1, tl = que[1] = 1; hd <= tl; ++hd) {
            int u = que[hd];
            for (int v : g[u]) if (v != fa[u])
                que[++tl] = v, fa[v] = u;
            if (u == 1 || g[u].size() != 1) tr[x].nonlvcnt += 1;
            if (u == 2 && g[u].size() == 1) tr[x].flg |= 1;
        }
        for (int s = 0, f0; s < 4; s++) {
            memset(f, 0, sizeof(int) * (m+1) * 2);
            for (int i = m; i >= 2; i--) {
                int v = que[i], u = fa[v];
                f[u][1] += std::max(f[v][0], f[v][1]);
                if ((v>2 || (s>>v-1&1)) && (u>2 || (s>>u-1&1)))
                    f[u][1] = std::max(f[u][1], f[u][0] + f[v][0] + 1);
                f[u][0] += std::max(f[v][0], f[v][1]);
            }
            const int now = std::max(f[1][0], f[1][1]);
            // fprintf(stderr, "%d%c", now, " \n"[s==3]);
            if (s) assert(now - f0 <= 2), tr[x].flg |= (now - f0) << s*2;
            else tr[x].fbase = f0 = now;
        }
    }
    int L, R;
    Node inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u];
        int mid = l + r >> 1;
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
    inline Mint inquire(int l, int r) { L=l, R=r; auto res = inq(1, 1, n); return res.fbase + std::max({res[1], res[2], res[3]}); }
} f;
int main() {
    usefile("tree");
    int Tid, T;
    read(Tid, T);
    while (T--) {
        int q;
        read(n, q);
        f.build();
        for (int l, r; q--; ) {
            read(l, r);
            printf("%d\n", f.inquire(l, r));
        }
    }
}