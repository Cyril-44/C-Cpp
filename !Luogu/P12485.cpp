#include <bits/stdc++.h>
template<typename bb>concept IntegerWithI128=std::integral<bb>||std::same_as<bb,__int128_t>||std::same_as<bb,__uint128_t>;class FastInputStream{static constexpr size_t db=1<<20;char eb[db];char*fb=nullptr,*gb=nullptr;FILE*hb;public:FastInputStream(FILE*ib=stdin):hb(ib){}[[gnu::always_inline]]char get(){if(fb==gb){fb=eb;gb=eb+fread(eb,1,db,hb);if(fb==gb)return EOF;}return*fb++;}FastInputStream&operator>>(bool&kb){int lb=get();while(~lb&&lb!='0'&&lb!='1')lb=get();kb=(lb=='1');return*this;}FastInputStream&operator>>(char&mb){mb=get();while(mb==' '||mb=='\r'||mb=='\n'||mb=='\t')mb=get();return*this;}template<IntegerWithI128 nb>FastInputStream&operator>>(nb&ob){int pb=get();bool qb=false;while(~pb&&(pb<'0'||pb>'9')&&pb!='-')pb=get();if constexpr(std::is_signed_v<nb>||std::same_as<nb,__int128_t>){if(pb=='-'){qb=true;pb=get();}}ob=0;while(pb>='0'&&pb<='9'){ob=(ob<<3)+(ob<<1)+(pb^'0');pb=get();}if(qb)ob=-ob;return*this;}template<std::floating_point rb>FastInputStream&operator>>(rb&sb){int tb=get();bool ub=false;while(~tb&&(tb<'0'||tb>'9')&&tb!='-')tb=get();if(tb=='-'){ub=true;tb=get();}__uint128_t vb=0;while(tb>='0'&&tb<='9'){vb=(vb<<3)+(vb<<1)+(tb^'0');tb=get();}sb=static_cast<rb>(vb);if(tb=='.'){rb wb=1.0;for(tb=get();tb>='0'&&tb<='9';tb=get()){sb+=(tb^'0')*(wb/=10.0);}}if(ub)sb=-sb;return*this;}FastInputStream&operator>>(char*xb){int yb=get();while(~yb&&(yb<=' '))yb=get();while(~yb&&(yb>' '))*xb++=yb,yb=get();*xb='\0';return*this;}}fin;class FastOutputStream{static constexpr size_t Bb=1<<20;char Cb[Bb],*Db=Cb;FILE*Eb;long double Fb=5e-6;unsigned char Gb=6;public:FastOutputStream(FILE*Hb=stdout):Eb(Hb){setvbuf(Eb,nullptr,_IONBF,0);}~FastOutputStream(){flush();}void flush(){fwrite(Cb,1,Db-Cb,Eb);Db=Cb;}[[gnu::always_inline]]void put(char Kb){if(Db==Cb+Bb)flush();*Db++=Kb;}unsigned char precision()const{return Gb;}FastOutputStream&precision(int Mb){Gb=Mb;Fb=0.5;while(Mb--)Fb*=0.1;return*this;}FastOutputStream&operator<<(char Nb){put(Nb);return*this;}FastOutputStream&operator<<(const char*Ob){while(*Ob)put(*Ob++);return*this;}FastOutputStream&operator<<(const std::string&Pb){for(char c:Pb)put(c);return*this;}template<IntegerWithI128 Qb>FastOutputStream&operator<<(Qb Rb){if(Rb==0){put('0');return*this;}Qb Sb=Rb;if constexpr(std::is_signed_v<Qb>||std::same_as<Qb,__int128_t>){if(Sb<0){put('-');Sb=-Sb;}}static char Tb[64];int Ub=0;while(Sb){Tb[Ub++]=static_cast<char>(Sb%10)^'0';Sb/=10;}while(Ub)put(Tb[--Ub]);return*this;}template<std::floating_point Vb>FastOutputStream&operator<<(Vb Wb){Wb+=Fb;if(Wb<0){put('-');Wb=-Wb;}__uint128_t Xb=static_cast<__uint128_t>(Wb);*this<<Xb;Vb Yb=Wb-static_cast<Vb>(Xb);if(Gb>0){put('.');for(unsigned Zb=Gb;Zb;--Zb){Yb*=10;int _b=static_cast<int>(Yb);put(_b^'0');Yb-=_b;}}return*this;}}fout;
#ifdef CLANGD
constexpr int N = 104;
#else
constexpr int N = 1000004;
#endif
constexpr int INF = 1e9;
int a[N], pre[N], n;
std::set<int> p[N];
inline int pp(int x) { return pre[*p[x].begin()]; }
struct SegTr { // Maintain v - s[v] - pre[p[v]]
    struct Data { // <=0 的最大值和 >0 的最小值
        int max, maxpos, min, minpos, cnt; // cnt: 处于 S 集合中数的个数
        void put(int val, int pos) {
            maxpos = minpos = pos, max = -INF, min = INF;
            if (val > 0) min = val, cnt = 0;
            else max = val, cnt = 1;
        }
        friend Data operator+(const Data& l, const Data& r) {
            Data res;
            if (l.max >= r.max) res.max = l.max, res.maxpos = l.maxpos;
            else res.max = r.max, res.maxpos = r.maxpos;
            if (l.min <= r.min) res.min = l.min, res.minpos = l.minpos;
            else res.min = r.min, res.minpos = r.minpos;
            res.cnt = l.cnt + r.cnt;
            return res;
        }
    };
    struct Node {
        Data dat; int add;
        void pull(int ad) { add += ad, dat.min += ad, dat.max += ad; }
    } tr[N << 2];
    void pushdown(int u) {
        if (tr[u].add) {
            tr[u<<1].pull(tr[u].add);
            tr[u<<1|1].pull(tr[u].add);
            tr[u].add = 0;
        }
    }
    void pushup(int u) { tr[u].dat = tr[u<<1].dat + tr[u<<1|1].dat; }
    int P, L, R, X;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].pull(X);
        int mid = l + r >> 1; pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    void mdf(int u, int l, int r) {
        if (l == r) return tr[u].dat.put(X, l);
        int mid = l + r >> 1; pushdown(u);
        if (P <= mid) mdf(u<<1, l, mid);
        else mdf(u<<1|1, mid+1, r);
        pushup(u);
    }
    Data inq(int u, int l, int r) {
        if (L <= l && r <= R) return tr[u].dat;
        int mid = l + r >> 1; pushdown(u);
        if (R <= mid) return inq(u<<1, l, mid);
        if (mid < L) return inq(u<<1|1, mid+1, r);
        return inq(u<<1, l, mid) + inq(u<<1|1, mid+1, r);
    }
public:
    void build(int u=1, int l=1, int r=n) {
        if (l == r) return tr[u].dat.put(std::min(l - pre[n], 1), l);
        int mid = l + r >> 1;
        build(u<<1, l, mid);
        build(u<<1|1, mid+1, r);
        pushup(u);
    }
    void update(int v, int pre_pp, int now_pp) { // Assume (v - s[v] - pre[p[v]]) has changed and fix.
        L=v; R=v; Data cur = inq(1, 1, n);
        int pre_v = cur.cnt ? cur.max : cur.min;
        int now_v = pre_v - (now_pp - pre_pp);
        P=v, X=now_v, mdf(1, 1, n);
        if (v == n) return;
        if (pre_v > 0 && now_v <= 0) { // v entered S, --s_[v+1, n]
            L=v+1, R=n; Data res = inq(1, 1, n);
            if (res.max == 0) { // (x - s[x] - pp[x]) quit S, ++s_x
                int x = res.maxpos;
                P=x, X=1, mdf(1, 1, n);
                L=v+1, R=x-1, X=1; if (L<=R) upd(1, 1, n);
            } else {
                L=v+1, R=n, X=1, upd(1, 1, n);
            }
        }
        if (pre_v <= 0 && now_v > 0) { // v 离开 S, ++s_v
            L=v+1, R=n; Data res = inq(1, 1, n);
            if (res.min == 1) {
                int x = res.minpos;
                P=x, X=0, mdf(1, 1, n);
                L=v+1, R=x-1, X=-1; if (L<=R) upd(1, 1, n);
            } else {
                L=v+1, R=n, X=-1, upd(1, 1, n);
            }
        }
    }
    int search(int num) {
        int u=1, l=1, r=n;
        while (l < r) {
            int mid = l + r >> 1; pushdown(u);
            if (tr[u<<1].dat.cnt >= num) u=u<<1, r=mid;
            else num -= tr[u<<1].dat.cnt, u=u<<1|1, l=mid+1;
        }
        return l;
    }
} f;
#define Oper(pos, val, op) do {             \
    if (~val) {                             \
        int pre_pp = pp(val);               \
        p[val].op(pos);                     \
        int now_pp = pp(val);               \
        if (now_pp != pre_pp)               \
            f.update(val, pre_pp, now_pp);  \
    }                                       \
} while (0)
int main() {
    int q;
    fin >> n >> q;
    std::fill(p+1, p+1+n, std::set<int>{n+1});
    for (int i = 1; i <= n; i++) {
        fin >> a[i];
        pre[i] = pre[i-1] + !a[i];
    }
    pre[n+1] = pre[n];
    f.build();
    for (int _q = 1; _q <= q; _q++) {
        int x, k, y;
        fin >> x >> k >> y;
        Oper(x, a[x], erase); a[x] = k; Oper(x, k, insert);
        fout << (a[y] == 0 ? f.search(pre[y]) : a[y]) << '\n';
        // assert(f.tr[1].dat.cnt == pre[n]);
    }
    return 0;
}
