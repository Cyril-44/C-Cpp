#include <bits/stdc++.h>
constexpr int N = 3000005;
#ifdef CLANGD
#define N 35
#endif
template<typename bb>concept IntegerWithI128=std::integral<bb>||std::same_as<bb,__int128_t>||std::same_as<bb,__uint128_t>;class FastInputStream{static constexpr size_t db=1<<20;char eb[db];char*fb=nullptr,*gb=nullptr;FILE*hb;public:FastInputStream(FILE*ib=stdin):hb(ib){}[[gnu::always_inline]]char get(){if(fb==gb){fb=eb;gb=eb+fread(eb,1,db,hb);if(fb==gb)return EOF;}return*fb++;}FastInputStream&operator>>(bool&kb){int lb=get();while(~lb&&lb!='0'&&lb!='1')lb=get();kb=(lb=='1');return*this;}FastInputStream&operator>>(char&mb){mb=get();while(mb==' '||mb=='\r'||mb=='\n'||mb=='\t')mb=get();return*this;}template<IntegerWithI128 nb>FastInputStream&operator>>(nb&ob){int pb=get();bool qb=false;while(~pb&&(pb<'0'||pb>'9')&&pb!='-')pb=get();if constexpr(std::is_signed_v<nb>||std::same_as<nb,__int128_t>){if(pb=='-'){qb=true;pb=get();}}ob=0;while(pb>='0'&&pb<='9'){ob=(ob<<3)+(ob<<1)+(pb^'0');pb=get();}if(qb)ob=-ob;return*this;}template<std::floating_point rb>FastInputStream&operator>>(rb&sb){int tb=get();bool ub=false;while(~tb&&(tb<'0'||tb>'9')&&tb!='-')tb=get();if(tb=='-'){ub=true;tb=get();}__uint128_t vb=0;while(tb>='0'&&tb<='9'){vb=(vb<<3)+(vb<<1)+(tb^'0');tb=get();}sb=static_cast<rb>(vb);if(tb=='.'){rb wb=1.0;for(tb=get();tb>='0'&&tb<='9';tb=get()){sb+=(tb^'0')*(wb/=10.0);}}if(ub)sb=-sb;return*this;}FastInputStream&operator>>(char*xb){int yb=get();while(~yb&&(yb<=' '))yb=get();while(~yb&&(yb>' '))*xb++=yb,yb=get();*xb='\0';return*this;}}fin;class FastOutputStream{static constexpr size_t Bb=1<<20;char Cb[Bb],*Db=Cb;FILE*Eb;long double Fb=5e-6;unsigned char Gb=6;public:FastOutputStream(FILE*Hb=stdout):Eb(Hb){setvbuf(Eb,nullptr,_IONBF,0);}~FastOutputStream(){flush();}void flush(){fwrite(Cb,1,Db-Cb,Eb);Db=Cb;}[[gnu::always_inline]]void put(char Kb){if(Db==Cb+Bb)flush();*Db++=Kb;}unsigned char precision()const{return Gb;}FastOutputStream&precision(int Mb){Gb=Mb;Fb=0.5;while(Mb--)Fb*=0.1;return*this;}FastOutputStream&operator<<(char Nb){put(Nb);return*this;}FastOutputStream&operator<<(const char*Ob){while(*Ob)put(*Ob++);return*this;}FastOutputStream&operator<<(const std::string&Pb){for(char c:Pb)put(c);return*this;}template<IntegerWithI128 Qb>FastOutputStream&operator<<(Qb Rb){if(Rb==0){put('0');return*this;}Qb Sb=Rb;if constexpr(std::is_signed_v<Qb>||std::same_as<Qb,__int128_t>){if(Sb<0){put('-');Sb=-Sb;}}static char Tb[64];int Ub=0;while(Sb){Tb[Ub++]=static_cast<char>(Sb%10)^'0';Sb/=10;}while(Ub)put(Tb[--Ub]);return*this;}template<std::floating_point Vb>FastOutputStream&operator<<(Vb Wb){Wb+=Fb;if(Wb<0){put('-');Wb=-Wb;}__uint128_t Xb=static_cast<__uint128_t>(Wb);*this<<Xb;Vb Yb=Wb-static_cast<Vb>(Xb);if(Gb>0){put('.');for(unsigned Zb=Gb;Zb;--Zb){Yb*=10;int _b=static_cast<int>(Yb);put(_b^'0');Yb-=_b;}}return*this;}}fout;
int a[N];
struct WBLT {
    using Pii = std::pair<int,int>;
    struct Node {
        uint64_t v[30];
        int cnt, tag, ls, rs, w, refcnt;
        void pull(int r) { // Advance r
            if (cnt <= r) {
                for (int i = 0; i < cnt; i++) v[i] = 0;
                cnt = 0;
            } else {
                for (int i = 0; i+r < cnt; i++) v[i] = v[i + r];
                for (int i = cnt - r; i < cnt; i++) v[i] = 0;
                cnt -= r;
            }
            tag += r;
        }
    } tr[N * 4];
    int bin[N * 4], top;
    void recycle(int u) {
        if (!--tr[u].refcnt) {
            bin[++bin[0]] = u;
            if (tr[u].ls) recycle(tr[u].ls), recycle(tr[u].rs);
        }
    }
    int alloc() {
        int u = bin[0] ? bin[bin[0]--] : ++top;
        tr[u] = {}; tr[u].refcnt = 1;
        return u;
    }
    int clone(int u) {
        int v = alloc();
        tr[v] = tr[u];
        if (tr[u].ls) ++tr[tr[u].ls].refcnt, ++tr[tr[u].rs].refcnt;
        return v;
    }
    void pushup(int u)  {
        tr[u].w = tr[tr[u].ls].w + tr[tr[u].rs].w;
        tr[u].cnt = std::max(tr[tr[u].ls].cnt, tr[tr[u].rs].cnt);
        for (int i = 0; i < tr[u].cnt; i++)
            tr[u].v[i] = tr[tr[u].ls].v[i] + tr[tr[u].rs].v[i];
    }
    [[gnu::always_inline]] bool isBalanced(int x, int y) { return x < 3*y; }
    Pii cut(int u) {
        int ls = clone(tr[u].ls), rs = clone(tr[u].rs);
        tr[ls].pull(tr[u].tag), tr[rs].pull(tr[u].tag);
        recycle(u);
        return {ls, rs};
    }
    int merge(int ul, int ur) {
        if (!ul || !ur) return ul | ur;
        if (!isBalanced(tr[ul].w, tr[ur].w)) {
            auto [ul1, ul2] = cut(ul);
            if (!isBalanced(tr[ul2].w + tr[ur].w, tr[ul1].w)) {
                auto [ul2a, ul2b] = cut(ul2);
                return merge(merge(ul1, ul2a), merge(ul2b, ur));
            }
            return merge(ul1, merge(ul2, ur));
        }
        if (!isBalanced(tr[ur].w, tr[ul].w)) {
            auto [ur1, ur2] = cut(ur);
            if (!isBalanced(tr[ul].w + tr[ur1].w, tr[ur2].w)) {
                auto [ur1a, ur1b] = cut(ur1);
                return merge(merge(ul, ur1a), merge(ur1b, ur2));
            }
            return merge(merge(ul, ur1), ur2);
        }
        int u = alloc();
        tr[u].ls = ul, tr[u].rs = ur;
        pushup(u);
        return u;
    }
    Pii split(int u, int sz) { // Split out [1, sz] [sz+1, n]
        assert(tr[u].w >= sz);
        if (sz == 0) return {0, u};
        if (sz == tr[u].w) return {u, 0};
        auto [ls, rs] = cut(u);
        if (sz <= tr[ls].w) {
            auto [l1, l2] = split(ls, sz);
            return {l1, merge(l2, rs)};
        }
        auto [r1, r2] = split(rs, sz - tr[ls].w);
        return {merge(ls, r1), r2};
    }
    int build(int l, int r) {
        int u = alloc();
        if (l == r) {
            tr[u].w = 1;
            for (int x = a[l]; x;) tr[u].v[tr[u].cnt++] = x, x >>= 1;
        } else {
            int mid = l + r >> 1;
            tr[u].ls = build(l, mid);
            tr[u].rs = build(mid+1, r);
            pushup(u);
        }
        return u;
    }
    int debug(int u) {
        if (!tr[u].ls) {
            fprintf(stderr, "%llu ", tr[u].v[0]);
            return u;
        }
        auto [ls, rs] = cut(u);
        ls = debug(ls), rs = debug(rs);
        return merge(ls, rs);
    }
    int rt;
} f;
int main() {
    int n, m; fin >> n;
    for (int i = 1; i <= n; i++) fin >> a[i];
    f.rt = f.build(1, n);
    fin >> m;
    for (int op, l, r, x; m--; ) {
        fin >> op >> l >> r;
        auto [tp, rs] = f.split(f.rt, r);
        auto [ls, u] = f.split(tp, l-1);
        if (op == 1) {
            fin >> x;
            int xu = f.clone(u);
            f.rt = f.merge(ls, f.merge(u, rs));
            auto [xtp, xrs] = f.split(f.rt, x+r-l);
            auto [xls, xdrop] = f.split(xtp, x-1);
            ls = xls, u = xu, rs = xrs;
        }
        else if (op == 2)  f.tr[u].pull(1);
        else fout << f.tr[u].v[0] << '\n';
        f.rt = f.merge(ls, f.merge(u, rs));
        f.rt = f.debug(f.rt); fprintf(stderr, "\n");
    }
}