#include <bits/stdc++.h>
constexpr int N = 200004, K = 18;
using uint = uint32_t;
template<typename bb>concept IntegerWithI128=std::integral<bb>||std::same_as<bb,__int128_t>||std::same_as<bb,__uint128_t>;class FastInputStream{static constexpr size_t db=1<<20;char eb[db];char*fb=nullptr,*gb=nullptr;FILE*hb;public:FastInputStream(FILE*ib=stdin):hb(ib){}[[gnu::always_inline]]char get(){if(fb==gb){fb=eb;gb=eb+fread(eb,1,db,hb);if(fb==gb)return EOF;}return*fb++;}FastInputStream&operator>>(bool&kb){int lb=get();while(~lb&&lb!='0'&&lb!='1')lb=get();kb=(lb=='1');return*this;}FastInputStream&operator>>(char&mb){mb=get();while(mb==' '||mb=='\r'||mb=='\n'||mb=='\t')mb=get();return*this;}template<IntegerWithI128 nb>FastInputStream&operator>>(nb&ob){int pb=get();bool qb=false;while(~pb&&(pb<'0'||pb>'9')&&pb!='-')pb=get();if constexpr(std::is_signed_v<nb>||std::same_as<nb,__int128_t>){if(pb=='-'){qb=true;pb=get();}}ob=0;while(pb>='0'&&pb<='9'){ob=(ob<<3)+(ob<<1)+(pb^'0');pb=get();}if(qb)ob=-ob;return*this;}template<std::floating_point rb>FastInputStream&operator>>(rb&sb){int tb=get();bool ub=false;while(~tb&&(tb<'0'||tb>'9')&&tb!='-')tb=get();if(tb=='-'){ub=true;tb=get();}__uint128_t vb=0;while(tb>='0'&&tb<='9'){vb=(vb<<3)+(vb<<1)+(tb^'0');tb=get();}sb=static_cast<rb>(vb);if(tb=='.'){rb wb=1.0;for(tb=get();tb>='0'&&tb<='9';tb=get()){sb+=(tb^'0')*(wb/=10.0);}}if(ub)sb=-sb;return*this;}FastInputStream&operator>>(char*xb){int yb=get();while(~yb&&(yb<=' '))yb=get();while(~yb&&(yb>' '))*xb++=yb,yb=get();*xb='\0';return*this;}}fin;class FastOutputStream{static constexpr size_t Bb=1<<20;char Cb[Bb],*Db=Cb;FILE*Eb;long double Fb=5e-6;unsigned char Gb=6;public:FastOutputStream(FILE*Hb=stdout):Eb(Hb){setvbuf(Eb,nullptr,_IONBF,0);}~FastOutputStream(){flush();}void flush(){fwrite(Cb,1,Db-Cb,Eb);Db=Cb;}[[gnu::always_inline]]void put(char Kb){if(Db==Cb+Bb)flush();*Db++=Kb;}unsigned char precision()const{return Gb;}FastOutputStream&precision(int Mb){Gb=Mb;Fb=0.5;while(Mb--)Fb*=0.1;return*this;}FastOutputStream&operator<<(char Nb){put(Nb);return*this;}FastOutputStream&operator<<(const char*Ob){while(*Ob)put(*Ob++);return*this;}FastOutputStream&operator<<(const std::string&Pb){for(char c:Pb)put(c);return*this;}template<IntegerWithI128 Qb>FastOutputStream&operator<<(Qb Rb){if(Rb==0){put('0');return*this;}Qb Sb=Rb;if constexpr(std::is_signed_v<Qb>||std::same_as<Qb,__int128_t>){if(Sb<0){put('-');Sb=-Sb;}}static char Tb[64];int Ub=0;while(Sb){Tb[Ub++]=static_cast<char>(Sb%10)^'0';Sb/=10;}while(Ub)put(Tb[--Ub]);return*this;}template<std::floating_point Vb>FastOutputStream&operator<<(Vb Wb){Wb+=Fb;if(Wb<0){put('-');Wb=-Wb;}__uint128_t Xb=static_cast<__uint128_t>(Wb);*this<<Xb;Vb Yb=Wb-static_cast<Vb>(Xb);if(Gb>0){put('.');for(unsigned Zb=Gb;Zb;--Zb){Yb*=10;int _b=static_cast<int>(Yb);put(_b^'0');Yb-=_b;}}return*this;}}fout;
uint A, B, C, D;
struct Mat {
    uint a[2][2]{{0, (uint)1e9}, {(uint)1e9, 0}};
    friend Mat operator*(const Mat& x, const Mat& y) {
        return {{{std::min(x.a[0][0] + y.a[0][0], x.a[0][1] + y.a[1][0]), std::min(x.a[0][0] + y.a[0][1], x.a[0][1] + y.a[1][1])},
                 {std::min(x.a[1][0] + y.a[0][0], x.a[1][1] + y.a[1][0]), std::min(x.a[1][0] + y.a[0][1], x.a[1][1] + y.a[1][1])}}};
    }
    uint operator()() const { return (A^a[0][0]) + (B^a[0][1]) + (C^a[1][0]) + (D^a[1][1]); }
};
uint n, m, curTm;
class MultiTr { // 矩阵历史树
    Mat mul[N*2][K+1]; uint fa[N*2][K+1], tm[N*2], tot = 0;
public:
    uint addleaf(uint u, uint val, uint t=curTm) { // 在节点 u 下面接一个时间为 t、矩阵为 val 的节点，并返回节点编号
        uint v = ++tot; tm[v] = t; fa[v][0] = u; mul[v][0] = {{{val>>24, val<<8>>24}, {val<<16>>24, val<<24>>24}}};
        for (int k = 1; k <= K; k++) fa[v][k] = fa[fa[v][k-1]][k-1], mul[v][k] = mul[fa[v][k-1]][k-1] * mul[v][k-1];
        return v;
    }
    uint inquire(uint v, uint t) { return inquire(v, t, curTm); }
    uint inquire(uint v, uint t1, uint t2) { // 查询叶子 v 到根的链上，时间 (t1, t2] 的矩阵乘积
        Mat res;
        for (int k = K; k >= 0; k--)
            if (tm[fa[v][k]] > t2)
                v = fa[v][k];
        if (tm[v] > t2) v = fa[v][0];
        for (int k = K; k >= 0; k--)
            if (tm[fa[v][k]] > t1)
                res = mul[v][k] * res, v = fa[v][k];
        if (tm[v] > t1) res = mul[v][0] * res;
        return res();
    }
} multr;
class PosLeaves { // 在版本 t 时，位置 i 所对应的矩阵历史树的叶子节点编号
    struct Node { uint lc, rc, val; } tr[N * K];
    uint rt[N], tot, P, X;
    void clone(uint &u) { tr[++tot] = tr[u], u = tot; }
    void upd(uint &u, uint l, uint r) { // set arr[P] = X
        clone(u);
        if (l == r) return void(tr[u].val = X);
        uint mid = l + r >> 1;
        P <= mid ? upd(tr[u].lc, l, mid) : upd(tr[u].rc, mid+1, r);
    }
    uint inq(uint u, uint l, uint r) {
        if (l == r) return tr[u].val;
        uint mid = l + r >> 1;
        return P <= mid ? inq(tr[u].lc, l, mid) : inq(tr[u].rc, mid+1, r);
    }
    void build(uint &u, uint l, uint r) {
        clone(u);
        if (l == r) return;
        uint mid = l + r >> 1;
        build(tr[u].lc, l, mid);
        build(tr[u].rc, mid+1, r);
    }
public:
    void inherit(uint fr, uint t=curTm) { rt[t] = rt[fr]; }
    void apply(uint p, uint x, uint t=curTm) { P=p, X=x; upd(rt[t], 1, n); }
    uint inquire(uint p, uint t=curTm) { P=p; return inq(rt[t], 1, n); }
    void build() { build(rt[0], 1, n); }
} poslv;
class SegTr { // 在版本 t 时，每个机器人所在的 pos 和迁移到 pos 的 time，以及当前的评分
    struct Node {
        uint lc, rc;
        struct Tag {
            uint p1, t1, r, p, t; // 懒惰标记，表示下传的时候需要将子节点迁移到 (p1, t1) 并结算，然后在 t1~t 时间内的贡献是 r，最后迁移到 (p, t)
            void pull(const Tag &tag) {
                if (p) {
                    r ^= multr.inquire(poslv.inquire(p), t, tag.t1) ^ tag.r;
                    p = tag.p, t = tag.t;
                }
                else *this = tag;
            }
        } tag;
    } tr[N * K * 8];
    uint rt[N], tot, L, R, C, T, P;
    void clone(uint &u) { tr[++tot] = tr[u], u = tot; }
    void pushdown(uint &u) {
        clone(tr[u].lc), clone(tr[u].rc);
        if (tr[u].tag.p) {
            tr[tr[u].lc].tag.pull(tr[u].tag);
            tr[tr[u].rc].tag.pull(tr[u].tag);
            tr[u].tag = {};
        }
    }
    void upd(uint u, uint l, uint r) { // 将x bot [L, R] 位置改为 C 于时间 T
        if (L <= l && r <= R) return tr[u].tag.pull({C, T, 0, C, T});
        uint mid = l + r >> 1; pushdown(u);
        if (L <= mid) upd(tr[u].lc, l, mid);
        if (mid < R) upd(tr[u].rc, mid+1, r);
    }
    uint inq(uint u, uint l, uint r) { // 查询
        if (l == r) return tr[u].tag.r ^ multr.inquire(poslv.inquire(tr[u].tag.p), tr[u].tag.t);
        uint mid = l + r >> 1; pushdown(u);
        return P <= mid ? inq(tr[u].lc, l, mid) : inq(tr[u].rc, mid+1, r);
    }
    void build(uint& u, uint l, uint r) {
        clone(u);
        if (l == r) return void(fin >> tr[u].tag.p);
        uint mid = l + r >> 1;
        build(tr[u].lc, l, mid), build(tr[u].rc, mid+1, r);
    }
public:
    void inherit(uint fr) { rt[curTm] = rt[fr]; }
    void apply(uint l, uint r, uint c, uint t=curTm) { L=l, R=r, C=c, T=t; clone(rt[t]); upd(rt[t], 1, m); }
    uint inquire(uint p, uint t=curTm) { P=p; clone(rt[t]); return inq(rt[t], 1, m); }
    void build() { build(rt[0], 1, m); }
} f;
int main() {
    uint q, typ, lastans = 0;
    fin >> n >> m >> q >> typ >> A >> B >> C >> D;
    poslv.build(); f.build();
    for (curTm = 1; curTm <= q; curTm++) {
        uint ti; char op; fin >> ti >> op;
        poslv.inherit(ti); f.inherit(ti);
        switch (op) {
        case 't': {
            uint l, r, c; fin >> l >> r >> c;
            if (typ) l ^= lastans, r ^= lastans, c ^= lastans;
            f.apply(l, r, c);
        } break;
        case 'e': {
            uint c, v; fin >> c >> v;
            if (typ) c ^= lastans, v ^= lastans;
            poslv.apply(c, multr.addleaf(poslv.inquire(c), v));
        } break;
        case 'q': {
            uint x; fin >> x;
            if (typ) x ^= lastans;
            fout << (lastans = f.inquire(x)) << '\n';
        }
        }
    }
    return 0;
}