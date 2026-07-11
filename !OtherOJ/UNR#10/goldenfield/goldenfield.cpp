#include <bits/stdc++.h>
template<typename bb>concept IntegerWithI128=std::integral<bb>||std::same_as<bb,__int128_t>||std::same_as<bb,__uint128_t>;class FastInputStream{static constexpr size_t db=1<<20;char eb[db];char*fb=nullptr,*gb=nullptr;FILE*hb;public:FastInputStream(FILE*ib=stdin):hb(ib){}[[gnu::always_inline]]char get(){if(fb==gb){fb=eb;gb=eb+fread(eb,1,db,hb);if(fb==gb)return EOF;}return*fb++;}FastInputStream&operator>>(bool&kb){int lb=get();while(~lb&&lb!='0'&&lb!='1')lb=get();kb=(lb=='1');return*this;}FastInputStream&operator>>(char&mb){mb=get();while(mb==' '||mb=='\r'||mb=='\n'||mb=='\t')mb=get();return*this;}template<IntegerWithI128 nb>FastInputStream&operator>>(nb&ob){int pb=get();bool qb=false;while(~pb&&(pb<'0'||pb>'9')&&pb!='-')pb=get();if constexpr(std::is_signed_v<nb>||std::same_as<nb,__int128_t>){if(pb=='-'){qb=true;pb=get();}}ob=0;while(pb>='0'&&pb<='9'){ob=(ob<<3)+(ob<<1)+(pb^'0');pb=get();}if(qb)ob=-ob;return*this;}template<std::floating_point rb>FastInputStream&operator>>(rb&sb){int tb=get();bool ub=false;while(~tb&&(tb<'0'||tb>'9')&&tb!='-')tb=get();if(tb=='-'){ub=true;tb=get();}__uint128_t vb=0;while(tb>='0'&&tb<='9'){vb=(vb<<3)+(vb<<1)+(tb^'0');tb=get();}sb=static_cast<rb>(vb);if(tb=='.'){rb wb=1.0;for(tb=get();tb>='0'&&tb<='9';tb=get()){sb+=(tb^'0')*(wb/=10.0);}}if(ub)sb=-sb;return*this;}FastInputStream&operator>>(char*xb){int yb=get();while(~yb&&(yb<=' '))yb=get();while(~yb&&(yb>' '))*xb++=yb,yb=get();*xb='\0';return*this;}}fin;class FastOutputStream{static constexpr size_t Bb=1<<20;char Cb[Bb],*Db=Cb;FILE*Eb;long double Fb=5e-6;unsigned char Gb=6;public:FastOutputStream(FILE*Hb=stdout):Eb(Hb){setvbuf(Eb,nullptr,_IONBF,0);}~FastOutputStream(){flush();}void flush(){fwrite(Cb,1,Db-Cb,Eb);Db=Cb;}[[gnu::always_inline]]void put(char Kb){if(Db==Cb+Bb)flush();*Db++=Kb;}unsigned char precision()const{return Gb;}FastOutputStream&precision(int Mb){Gb=Mb;Fb=0.5;while(Mb--)Fb*=0.1;return*this;}FastOutputStream&operator<<(char Nb){put(Nb);return*this;}FastOutputStream&operator<<(const char*Ob){while(*Ob)put(*Ob++);return*this;}FastOutputStream&operator<<(const std::string&Pb){for(char c:Pb)put(c);return*this;}template<IntegerWithI128 Qb>FastOutputStream&operator<<(Qb Rb){if(Rb==0){put('0');return*this;}Qb Sb=Rb;if constexpr(std::is_signed_v<Qb>||std::same_as<Qb,__int128_t>){if(Sb<0){put('-');Sb=-Sb;}}static char Tb[64];int Ub=0;while(Sb){Tb[Ub++]=static_cast<char>(Sb%10)^'0';Sb/=10;}while(Ub)put(Tb[--Ub]);return*this;}template<std::floating_point Vb>FastOutputStream&operator<<(Vb Wb){Wb+=Fb;if(Wb<0){put('-');Wb=-Wb;}__uint128_t Xb=static_cast<__uint128_t>(Wb);*this<<Xb;Vb Yb=Wb-static_cast<Vb>(Xb);if(Gb>0){put('.');for(unsigned Zb=Gb;Zb;--Zb){Yb*=10;int _b=static_cast<int>(Yb);put(_b^'0');Yb-=_b;}}return*this;}}fout;
constexpr int N = 1000005;
#define For(i, s, t) for(int i = (s); i <= (t); i++)
#define roF(i, t, s) for(int i = (t); i >= (s); i--)
#define dbgarr(arr) do { For(i, 1, n) { fprintf(stderr, "%d ", arr[i]); } fprintf(stderr, "\n"); } while (0)
#define ast(pos) assert(cov[pos] == (pos))
int a[N], b[N], c[N], dif[N], cov[N], lc[N], rc[N];
int64_t sa[N], sb[N], sc[N];
int flg[N]; // 1: 当前<左边<右边，-1: 当前<右边<=左边
int main() {
    int n, q;
    fin >> n >> q;
    For(i, 1, n) fin >> a[i];
    For(i, 2, n) dif[i] = a[i] - a[i-1];
    For(i, 3, n) if (dif[i-1] < 0 && dif[i] > 0) flg[i-1] = -dif[i-1] < dif[i] ? 1 : -1;
    memcpy(b, a, sizeof a);
    std::iota(cov+1, cov+1+n, 1);
    For(i, 2, n-1) {
        if (flg[i] == 1) { // 应当往左扩展 (- - - - - +)
            ast(i+1), ast(i);
            cov[i] = i+1, b[i] = b[i+1];
            int j = i-1;
            while (j >= 2 && b[j] < b[i+1] && dif[j] < 0)
                b[j] = b[i+1], ast(j), cov[j] = i+1, --j;
            if (b[j] > b[i+1]) b[j+1] = b[j];
        } else if (flg[i] == -1) { // 应当向右扩展 (- + + + + +)
            ast(i+1), ast(i);
            cov[i] = i-1; b[i] = b[i-1];
            int j = i+1;
            while (j <= n-1 && b[j] < b[i-1] && dif[j+1] > 0)
                b[j] = b[i-1], ast(j), cov[j] = i-1, ++j;
            if (b[j] > b[i-1]) b[j-1] = b[j];
        }
    }
    memcpy(lc, cov, sizeof cov);
    memcpy(rc, cov, sizeof cov);
    For(i, 2, n-1)
        if (b[i-1] > b[i] && b[i+1] > b[i])
            c[i] = -b[i] + std::max(b[i-1], b[i+1]), lc[i] = cov[i-1], rc[i] = cov[i+1];
    For(i, 1, n) sa[i] = sa[i-1] + a[i];
    For(i, 1, n) sb[i] = sb[i-1] + b[i];
    For(i, 1, n) sc[i] = sc[i-1] + c[i];
    // dbgarr(cov); dbgarr(b); dbgarr(c); dbgarr(lc); dbgarr(rc);
    auto inquire = [n](int l, int r) {
        int64_t ans;
        if (cov[r] < l-1 || cov[l] > r+1) ans = sa[r] - sa[l-1];
        else {
            int p = std::lower_bound(cov+l, cov+r+1, l-1) - cov,
                q = std::upper_bound(cov+l, cov+r+1, r+1) - cov - 1;
            ans = sa[p-1] - sa[l-1] + sb[q] - sb[p-1] + sa[r] - sa[q];
            // fprintf(stderr, "<%d %d> ", p, q);
        }
        int cp = std::lower_bound(lc+l, lc+r+1, l-1) - lc,
            cq = std::upper_bound(rc+l, rc+r+1, r+1) - rc - 1;
        if (cp <= cq) ans += sc[cq] - sc[cp-1];
        // fprintf(stderr, "c:<%d %d> (add %ld) = %ld\n", cp, cq, sc[cq] - sc[cp-1], ans);
        return ans;
    };
    while (q--) {
        int l, r;
        fin >> l >> r;
        if (r - l <= 1) fout << sa[r] - sa[l-1] << '\n';
        else fout << a[l] + a[r] + inquire(l+1, r-1) << '\n';
    }
    return 0;
}
