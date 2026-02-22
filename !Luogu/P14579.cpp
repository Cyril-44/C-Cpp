#include <bits/stdc++.h>
#define Inline [[gnu::always_inline]] inline
class FastIS{static constexpr size_t h=1<<20;char o[h];char*t,*v;FILE*K;public:FastIS(FILE*B=stdin):t(nullptr),v(nullptr),K(B){}~FastIS(){fclose(K);}Inline int get(){return(t==v)&&(v=(t=o)+fread(o,1,h,K),t==v)?EOF:*t++;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();while(~a&&(a<'0'||a>'9'))a=get();for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,bool>::value&&!std::is_same<j,char>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;for(d=0;a>='0'&&a<='9';a=get())d=(d<<3)+(d<<1)+(a^'0');d=L?-d:d;return*this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastIS&>::type inline operator>>(j&d){int a=get();bool L=false;while(~a&&(a<'0'||a>'9')&&(a^'-'))a=get();if(a=='-')a=get(),L=true;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G I;for(I=0;a>='0'&&a<='9';a=get())I=(I<<3)+(I<<1)+(a^'0');d=I;if(a=='.'){G w=1;for(a=get();a>='0'&&a<='9';a=get())d+=(a^'0')*(1./(w*=10));}if(L)d=-d;return*this;}inline FastIS&operator>>(bool&U){int a=get();while(~a&&a!='0'&&a!='1')a=get();U=static_cast<bool>(a&1);return*this;}inline FastIS&operator>>(char&n){int a=get();while(a==' '||a=='\r'||a=='\n')a=get();n=a;return*this;}inline FastIS&operator>>(char*n){int a=get();while(~a&&(a==' '||a=='\r'||a=='\n'))a=get();while(~a&&(a!=' '&&a!='\r'&&a!='\n'))*n++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t h=1<<20;static constexpr double R=1e-9;char o[h],*O;const char*y;FILE*q;int l;public:FastOS(FILE*B=stdout):O(o),y(o+h),q(B),l(-1){setvbuf(q,nullptr,_IONBF,0);}~FastOS(){flush();fclose(q);}FastOS&flush(){fwrite(o,O-o,1,q);return*this;}FastOS&put(int H){*O++=H;if(O==y)fwrite(o,h,1,q),O=o;return*this;}FastOS&setprecision(const int&S){l=S;return*this;}template<typename j>typename std::enable_if<std::is_unsigned<j>::value&&std::is_integral<j>::value,FastOS&>::type inline operator<<(j d){static int n[32];n[*n=1]=d%10;while(d/=10)n[++(*n)]=d%10;while(*n)put(n[(*n)--]|'0');return*this;}template<typename j>typename std::enable_if<std::is_signed<j>::value&&std::is_integral<j>::value&&!std::is_same<j,char>::value,FastOS&>::type inline operator<<(const j&d){if(d<0)put('-')<<(typename std::make_unsigned<j>::type)(-d);else*this<<(typename std::make_unsigned<j>::type)(d);return *this;}template<typename j>typename std::enable_if<std::is_floating_point<j>::value,FastOS&>::type inline operator<<(j d){if(d<0)put('-'),d=-d;using G=typename std::conditional<std::is_same<j,float>::value,std::uint32_t,std::uint64_t>::type;G z=static_cast<G>(d);j O=d-z;*this<<z;if(l>0||l<0&&O>R){put('.');for(int J=0;l>0&&J<l||l<0&&O>R;J++){int N=static_cast<int>(O*=10);put(N|'0');O-=N;}}return*this;}FastOS&operator<<(const char&H){put(H);return*this;}}fout;
template<typename Tp>
struct MaxFlow {
    MaxFlow(int n_, int s, int t) : g(new EdgeList[n_+1]), dis(new int[n_+1]), head(new int[n_+1]), que(new int[n_+1]), n(n_), S(s), T(t), modified(false), maxflow() {}
    ~MaxFlow() { delete[] g; delete[] dis; delete[] head; }
    inline void addedg(int fr, int to, Tp c) {
        modified = true;
        g[fr].emplace_back(to, c, (int)g[to].size());
        g[to].emplace_back(fr, Tp(0), (int)g[fr].size() - 1);
    }
    inline bool bfs() {
        modified = true;
        memset(dis, 0, sizeof(int) * (n+1));
        memset(head, 0, sizeof(int) * (n+1));
        int hd = 0, tl = 0;
        que[tl++] = S; dis[S] = 1;
        while (hd ^ tl) {
            int u = que[hd++];
            for (const auto &[v, cap, bak] : g[u])
                if (cap && !dis[v]) {
                    dis[v] = dis[u] + 1;
                    if (v == T) return true;
                    que[tl++] = v;
                }
        }
        return false;
    }
    Tp dfs(int u, Tp infl) {
        if (u == T) return infl;
        Tp outfl{};
        for (int &i = head[u]; i ^ (int)(g[u].size()); i++) {
            auto &[v, cap, bak] = g[u][i];
            if (cap && dis[v] == dis[u] + 1) {
                Tp fl = dfs(v, std::min(cap, infl));
                if (fl) {
                    infl -= fl, outfl += fl;
                    cap -= fl, std::get<1>(g[v][bak]) += fl;
                    if (!infl) return outfl;
                } else dis[v] = 0;
            }
        }
        return outfl;
    }
    inline Tp operator()() {
        if (!modified) return maxflow;
        while (bfs()) maxflow += dfs(S, std::numeric_limits<Tp>::max());
        return maxflow;
    }
    using EdgeList = std::vector<std::tuple<int, Tp, int>>;
    EdgeList *g;
    int *dis, *head, *que, n, S, T;
    bool modified;
    Tp maxflow;
};
template<typename Tp>
struct MCapFlow {
    MCapFlow(int n_) : mf(n_+2, n_+1, n_+2), deg(new int[n_+1]()), n(n_), S(n_+1), T(n_+2), aval() {}
    inline int addedg(int fr, int to, Tp lwr, Tp upr) {
        mf.addedg(fr, to, upr - lwr);
        deg[fr] += lwr, deg[to] -= lwr;
        return (int)mf.g[to].size() - 1;
    }
    Tp getaval() {
        if (aval) return aval;
        for (int i = 0; i <= n; i++) // 如果 in-out > 0 连 T 否则连 S
            if (deg[i] > 0) mf.addedg(i, T, deg[i]);
            else if (deg[i] < 0) mf.addedg(S, i, -deg[i]);
        aval = mf();
        for (const auto &[v, cap, bak] : mf.g[S])
            if (cap) return -1; // S 到 x 必须满流
        return aval;
    }
    Tp getaval(int s, int t) {
        if (aval) return aval;
        int id = addedg(t, s, static_cast<Tp>(0), std::numeric_limits<Tp>::max()); // 加入 t-->s [0, +oo] 的边
        if (getaval() == -1) return -1;
        return std::get<1>(mf.g[s][id]); // 此时的可行流是 t-->s 的流量
    }
    Tp getmx(int s, int t) {
        if (aval) throw;
        addedg(t, s, static_cast<Tp>(0), std::numeric_limits<Tp>::max()); // 加入 t-->s [0, +oo] 的边
        int idt = (int)mf.g[t].size() - 1, ids = (int)mf.g[s].size() - 1;
        if (getaval() == -1) return -1;
        int res = std::get<1>(mf.g[s][ids]); // 此时的可行流是 t-->s 的流量
        std::get<1>(mf.g[t][idt]) = std::get<1>(mf.g[s][ids]) = static_cast<Tp>(0); // 删除 t-->s的边
        mf.S = s, mf.T = t, mf.maxflow = 0, mf.modified = true;
        return res + mf();
    }
    Tp getmn(int s, int t) {
        if (aval) throw;
        addedg(t, s, static_cast<Tp>(0), std::numeric_limits<Tp>::max()); // 加入 t-->s [0, +oo] 的边
        int idt = (int)mf.g[t].size() - 1, ids = (int)mf.g[s].size() - 1;
        if (getaval() == -1) return -1;
        int res = std::get<1>(mf.g[s][ids]); // 此时的可行流是 t-->s 的流量
        std::get<1>(mf.g[t][idt]) = std::get<1>(mf.g[s][ids]) = static_cast<Tp>(0); // 删除 t-->s的边
        mf.S = t, mf.T = s, mf.maxflow = 0, mf.modified = true;
        return res - mf();
    }
    MaxFlow<Tp> mf;
    std::vector<std::tuple<int,int,Tp,Tp>> edgs;
    Tp *deg; // deg 记录的是 入lwr - 出lwr
    int n, S, T;
    Tp aval;
    MaxFlow<Tp>* operator->() { return &mf; }
};
int main() {
    int n, m, s, t;
    fin >> n >> m >> s >> t;
    MCapFlow<int> mcf(n);
    for (int u, v, l, r; m--; ) {
        fin >> u >> v >> l >> r;
        mcf.addedg(u, v, l, r);
    }
    int ans = mcf.getmx(s, t);
    if (ans == -1) puts("N");
    else printf("%d\n", ans);
    return 0;
}