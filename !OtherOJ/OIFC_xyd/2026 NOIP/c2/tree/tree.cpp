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
constexpr int N = 200005, MOD = (int)1e9 + 7;
#ifdef CLANGD
#define N 25
#endif
std::vector<std::pair<int,int>> edgs[N];
int cnt2[N];
struct Mint {
    Mint& operator+=(const Mint o) { if ((val += o.val) >= MOD) val -= MOD; return *this; }
    Mint& operator-=(const Mint o) { if ((val -= o.val) < 0) val += MOD; return *this; }
    Mint& operator*=(const Mint o) { val = 1ull * val * o.val % MOD; return *this;  }
    friend Mint operator+(Mint x, const Mint y) { return x += y; }
    friend Mint operator-(Mint x, const Mint y) { return x -= y; }
    friend Mint operator*(Mint x, const Mint y) { return x *= y; }
    Mint pow(int n) const {
        Mint b = *this, res = 1;
        for (; n; n >>= 1, b *= b) if (n&1) res*=b;
        return res;
    }
    Mint(int v=0) : val(v) {}
private: int val;
} prod[N];
int main() {
    usefile("tree");
    int Tid, T;
    read(Tid, T);
    while (T--) {
        int n, q;
        read(n, q);
        prod[0] = 1;
        for (int i = 1; i <= n; i++) {
            int m; read(m);
            edgs[i].assign(m-1, {});
            for (auto &[x, y] : edgs[i]) read(x, y);
            prod[i] = prod[i-1] * (m-1);
            cnt2[i] = cnt2[i-1] + ((m-1)%2==0);
        }
        for (int l, r; q--; ) {
            read(l, r);
            Mint p = prod[r] * prod[l-1].pow(MOD-2);
            if (!(cnt2[r] - cnt2[l-1])) printf("%d\n", (p+1) * Mint(2).pow(MOD-2));
            else printf("%d\n", p * Mint(2).pow(MOD-2));
        }
    }
}