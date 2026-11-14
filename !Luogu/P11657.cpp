#include <bits/stdc++.h>
template<typename bb>concept IntegerWithI128=std::integral<bb>||std::same_as<bb,__int128_t>||std::same_as<bb,__uint128_t>;class FastInputStream{static constexpr size_t db=1<<20;char eb[db];char*fb=nullptr,*gb=nullptr;FILE*hb;public:FastInputStream(FILE*ib=stdin):hb(ib){}[[gnu::always_inline]]char get(){if(fb==gb){fb=eb;gb=eb+fread(eb,1,db,hb);if(fb==gb)return EOF;}return*fb++;}FastInputStream&operator>>(bool&kb){int lb=get();while(~lb&&lb!='0'&&lb!='1')lb=get();kb=(lb=='1');return*this;}FastInputStream&operator>>(char&mb){mb=get();while(mb==' '||mb=='\r'||mb=='\n'||mb=='\t')mb=get();return*this;}template<IntegerWithI128 nb>FastInputStream&operator>>(nb&ob){int pb=get();bool qb=false;while(~pb&&(pb<'0'||pb>'9')&&pb!='-')pb=get();if constexpr(std::is_signed_v<nb>||std::same_as<nb,__int128_t>){if(pb=='-'){qb=true;pb=get();}}ob=0;while(pb>='0'&&pb<='9'){ob=(ob<<3)+(ob<<1)+(pb^'0');pb=get();}if(qb)ob=-ob;return*this;}template<std::floating_point rb>FastInputStream&operator>>(rb&sb){int tb=get();bool ub=false;while(~tb&&(tb<'0'||tb>'9')&&tb!='-')tb=get();if(tb=='-'){ub=true;tb=get();}__uint128_t vb=0;while(tb>='0'&&tb<='9'){vb=(vb<<3)+(vb<<1)+(tb^'0');tb=get();}sb=static_cast<rb>(vb);if(tb=='.'){rb wb=1.0;for(tb=get();tb>='0'&&tb<='9';tb=get()){sb+=(tb^'0')*(wb/=10.0);}}if(ub)sb=-sb;return*this;}FastInputStream&operator>>(char*xb){int yb=get();while(~yb&&(yb<=' '))yb=get();while(~yb&&(yb>' '))*xb++=yb,yb=get();*xb='\0';return*this;}}fin;class FastOutputStream{static constexpr size_t Bb=1<<20;char Cb[Bb],*Db=Cb;FILE*Eb;long double Fb=5e-6;unsigned char Gb=6;public:FastOutputStream(FILE*Hb=stdout):Eb(Hb){setvbuf(Eb,nullptr,_IONBF,0);}~FastOutputStream(){flush();}void flush(){fwrite(Cb,1,Db-Cb,Eb);Db=Cb;}[[gnu::always_inline]]void put(char Kb){if(Db==Cb+Bb)flush();*Db++=Kb;}unsigned char precision()const{return Gb;}FastOutputStream&precision(int Mb){Gb=Mb;Fb=0.5;while(Mb--)Fb*=0.1;return*this;}FastOutputStream&operator<<(char Nb){put(Nb);return*this;}FastOutputStream&operator<<(const char*Ob){while(*Ob)put(*Ob++);return*this;}FastOutputStream&operator<<(const std::string&Pb){for(char c:Pb)put(c);return*this;}template<IntegerWithI128 Qb>FastOutputStream&operator<<(Qb Rb){if(Rb==0){put('0');return*this;}Qb Sb=Rb;if constexpr(std::is_signed_v<Qb>||std::same_as<Qb,__int128_t>){if(Sb<0){put('-');Sb=-Sb;}}static char Tb[64];int Ub=0;while(Sb){Tb[Ub++]=static_cast<char>(Sb%10)^'0';Sb/=10;}while(Ub)put(Tb[--Ub]);return*this;}template<std::floating_point Vb>FastOutputStream&operator<<(Vb Wb){Wb+=Fb;if(Wb<0){put('-');Wb=-Wb;}__uint128_t Xb=static_cast<__uint128_t>(Wb);*this<<Xb;Vb Yb=Wb-static_cast<Vb>(Xb);if(Gb>0){put('.');for(unsigned Zb=Gb;Zb;--Zb){Yb*=10;int _b=static_cast<int>(Yb);put(_b^'0');Yb-=_b;}}return*this;}}fout;
constexpr int N = 4000005;
#ifdef CLANGD
#define N 45
#endif
int n;
struct SegTr {
    struct Node { // pre: 右括号前缀最多剩多少，suf: 左括号后缀最多剩多少
        int pre, suf, ans, sum;
    } tr[N << 2], revtr[N << 2];
    Node promote(const Node &ls, const Node &rs, int l, int r, int mid, Node *tr) { // [0,mp): [l,mid] 拆开的区间范围
        Node res;
        res.pre = std::max(ls.pre, ls.sum + rs.pre);
        res.suf = std::max(rs.suf, rs.sum + ls.suf);
        res.ans = std::max(ls.ans, rs.ans);
        const int len = std::min(ls.suf, rs.pre);
        res.ans = std::max(res.ans, pres(tr, mid+1, r, len) - sufs(tr, l, mid, len) + 1);
    }
    bool inv[N << 2];
    void invert(int u) { inv[u] = !inv[u]; std::swap(tr[u], revtr[u]); }
    void pushdown(int u) {
        if (inv[u]) {
            invert(u<<1), invert(u<<1|1);
            inv[u] = false;
        }
    }
    int pres(Node *tr, int L, int R, int x) { // 找到最后一个满足 [l, p] 的 pre = x 的 p
        int ans = -1;
        Node now;
        for (auto [u, l, r] : nodes) {
            if (L > r || R < l) continue;
            if ()
            while (l < r) {
                int mid = l + r >> 1;
                pushdown(u);
                if (tr[u<<1].pre <= x) x -= tr[u<<1].sum, l = mid + 1, u = u<<1|1;
                else r = mid, u = u<<1;
            }
        }
        return l-1;
    }
    int sufs(Node *tr, int u, int l, int r, int x) {
        int ans = 0;
        std::reverse(nodes.begin(), nodes.end());
        Node now;
        for (auto [u, l, r] : nodes) {

        }
        while (l < r) {
            int mid = l + r >> 1;
            pushdown(u);
            if (tr[u<<1|1].suf <= x) x -= tr[u<<1|1].sum, r = mid, u = u<<1;
            else l = mid + 1, u = u<<1|1;
        }
        return l+1;
    }
    void pushup(Node *tr, int u, int l, int r) {
        int mid = l + r >> 1;
        tr[u].pre = std::max(tr[u<<1].pre, sum[u<<1] + tr[u<<1|1].pre);
        tr[u].suf = std::max(tr[u<<1|1].suf, sum[u<<1|1] + tr[u<<1].suf);
        tr[u].ans = std::max(tr[u<<1].ans, tr[u<<1|1].ans);
        const int len = std::min(tr[u<<1].suf, tr[u<<1|1].pre);
        tr[u].ans = std::max(tr[u].ans, pres(tr, u<<1|1, mid+1, r, len) - sufs(tr, u<<1, l, mid, len) + 1);
    }
    void pushup(int u, int l, int r) {
        sum[u] = sum[u<<1] + sum[u<<1|1];
        pushup(tr, u, l, r), pushup(revtr, u, l, r);
    }
    void build(int u=1, int l=1, int r=n) {
        if (l == r) {
            bool flg; fin >> flg;
            sum[u] = flg ? 1 : -1;
            (flg ? tr[u].pre : tr[u].suf) = 1;
            (flg ? revtr[u].pre : revtr[u].suf) = 1;
            return;
        }
        int mid = l + r >> 1;
        build(u<<1, l, mid), build(u<<1|1, mid+1, r);
        pushup(u, l, r);
    }
    int L, R;
    void upd(int u, int l, int r) {
        if (L <= l && r <= R) invert(u);
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        if (mid < R) upd(u<<1|1, mid+1, r);
        pushup(u, l, r);
    }
    std::vector<std::tuple<int,int,int>> nodes;
    void loc(int u, int l, int r) {
        if (L <= l && r <= R) { nodes.emplace_back(u, l, r); return; }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) loc(u<<1, l, mid);
        if (mid < R) loc(u<<1|1, mid+1, r);
    }
    void flip(int l, int r) { L=l, R=r; upd(1, 1, n); }
    int inquire(int l, int r) {
        L=l, R=r, nodes.clear(); loc(1, 1, n);
        int ans = -1;
        for (auto [u, l, r] : nodes) {
            if (ans == -1) {
                
            }
            ans = std::max(ans, tr[u].ans);
        }
    }
} f;
int main() {
    int q;
    fin >> n >> q;
    f.build();
    for (int op, l, r, last = 0; q--;) {
        fin >> op >> l >> r;
        l = (l + last) % n + 1, r = (r + last) % n + 1;
        if (op == 1) fout << (last = f.inquire(l, r)) << '\n';
        else f.flip(l, r);
    }
}