#include <complex>
#include <stdio.h>
#include <vector>
#include <cassert>
#include <cstring>
#include <bits/c++config.h>
#pragma GCC optimize(2)
#pragma GCC optimize("unroll-loops")
constexpr int N = 1000005;
constexpr auto MOD = 421969921;
using LL = long long;
std::vector<int> g[N];
#include <type_traits>
#ifndef _GLIBCXX17_CONSTEXPR
#define _GLIBCXX17_CONSTEXPR
#endif
class FastIS{static constexpr size_t P=1<<20;char r[P];char*u,*G;FILE*I;public:FastIS(FILE*Y=stdin):u(nullptr),G(nullptr),I(Y){}__attribute__((always_inline))inline int get(){return(u==G)&&(G=(u=r)+fread(r,1,P,I),u==G)?EOF:*u++;}template<typename A,typename=typename std::enable_if<std::is_arithmetic<A>::value>::type>FastIS&operator>>(A&j){if _GLIBCXX17_CONSTEXPR(std::is_same<A,bool>::value){int a=get();while(~a&&a!=(char)(48)&&a!=(char)(49))a=get();j=static_cast<bool>(a&1);}else if _GLIBCXX17_CONSTEXPR(std::is_same<A,char>::value){j=get();while(j==(char)(32)||j==(char)(13)||j==(char)(10))j=get();}else if _GLIBCXX17_CONSTEXPR(std::is_integral<A>::value){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<A>::value){int a=get();while(~a&&(a<(char)(48)||a>(char)(57)))a=get();for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));}else{int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));j=m?-j:j;}}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<A>::value){int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;using E=typename std::conditional<std::is_same<A,float>::value,std::uint32_t,std::uint64_t>::type;E Q;for(Q=0;a>=(char)(48)&&a<=(char)(57);a=get())Q=(Q<<3)+(Q<<1)+(a^(char)(48));j=Q;if(a==(char)(46)){E a=1;for(a=get();a>=(char)(48)&&a<=(char)(57);a=get())j+=(a^(char)(48))*(1./(a*=10));}if(m)j=-j;}else{throw;}return*this;}FastIS&operator>>(char*i){int a=get();while(~a&&(a==(char)(32)||a==(char)(13)||a==(char)(10)))a=get();while(~a&&(a!=(char)(32)&&a!=(char)(13)&&a!=(char)(10)))*i++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double x=1e-9;char r[P],*b;const char*l;FILE*w;int o;public:FastOS(FILE*Y=stdout):b(r),l(r+P),w(Y),o(-1){setvbuf(w,nullptr,_IONBF,0);}~FastOS(){flush();fclose(w);}FastOS&flush(){fwrite(r,b-r,1,w);return*this;}FastOS&put(int g){*b++=g;if(b==l)fwrite(r,P,1,w),b=r;return*this;}FastOS&setprecision(const int&t){o=t;return*this;}template<typename a,typename=typename std::enable_if<std::is_arithmetic<a>::value>::type>FastOS&operator<<(a j){if _GLIBCXX17_CONSTEXPR(std::is_same<a,char>::value)put(j);else if _GLIBCXX17_CONSTEXPR(std::is_integral<a>::value){if _GLIBCXX17_CONSTEXPR(std::is_signed<a>::value)if(j<0)put((char)(45)),j=-j;static uint8_t i[40];i[*i=1]=j%10;while(j/=10)i[++(*i)]=j%10;while(*i)put(i[(*i)--]|(char)(48));}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<a>::value){if(j<0)put((char)(45)),j=-j;using E=typename std::conditional<std::is_same<a,float>::value,std::uint32_t,std::uint64_t>::type;E v=static_cast<E>(j);a B=j-v;*this<<v;if(o>0||(o<0&&B>x)){put((char)(46));for(int R=0;(o>0&&R<o)||o<0&&B>x;R++){int X=static_cast<int>(B*=10);put(X|(char)(48));B-=X;}}}else{throw;}return*this;}FastOS&operator<<(char*i){while(*i)put(*i++);return*this;}}fout;

int n;
namespace Tmp {
using namespace std;

using cd = std::complex<double>;
const double PI = acos(-1.0);
static bool removed[N];
static int sz[N];
static long long ans[N]; // ans[d] = 无序点对数，距离为 d

// ---------- FFT ----------
void fft(vector<cd> & a, bool invert) {
    int n = (int)a.size();
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len/2; ++j) {
                cd u = a[i+j];
                cd v = a[i+j+len/2] * w;
                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) {
        for (cd & x : a) x /= n;
    }
}

vector<long long> convolution(const vector<long long> &A, const vector<long long> &B) {
    int na = (int)A.size(), nb = (int)B.size();
    if (na == 0 || nb == 0) return {};
    int sz = 1;
    while (sz < na + nb - 1) sz <<= 1;
    vector<cd> fa(sz);
    for (int i = 0; i < na; ++i) fa[i] = cd(A[i], 0);
    for (int i = 0; i < nb; ++i) fa[i].imag(B[i]);
    fft(fa, false);
    vector<cd> fb(sz);
    // use complex trick to get convolution of real sequences
    for (int i = 0; i < sz; ++i) {
        int j = (sz - i) & (sz - 1);
        cd a1 = (fa[i] + conj(fa[j])) * cd(0.5, 0);
        cd a2 = (fa[i] - conj(fa[j])) * cd(0, -0.5);
        fb[i] = a1 * a2;
    }
    fft(fb, true);
    vector<long long> res(na + nb - 1);
    for (int i = 0; i < (int)res.size(); ++i) {
        long long val = (long long) (fb[i].real() + 0.5);
        res[i] = val;
    }
    return res;
}
// ---------- FFT end ----------

// ---------- Centroid decomposition ----------
void dfs_size(int u, int p) {
    sz[u] = 1;
    for (int v : g[u]) if (v != p && !removed[v]) {
        dfs_size(v, u);
        sz[u] += sz[v];
    }
}

int dfs_centroid(int u, int p, int total) {
    for (int v : g[u]) if (v != p && !removed[v]) {
        if (sz[v] > total/2) return dfs_centroid(v, u, total);
    }
    return u;
}

void collect_depths(int u, int p, int depth, vector<int> &out) {
    if ((int)out.size() <= depth) out.resize(depth+1);
    out[depth]++;
    for (int v : g[u]) if (v != p && !removed[v]) {
        collect_depths(v, u, depth+1, out);
    }
}

void decompose(int entry) {
    dfs_size(entry, -1);
    int c = dfs_centroid(entry, -1, sz[entry]);
    removed[c] = true;

    // acc: 累计已处理子树的深度计数（初始只有质心本身）
    vector<long long> acc(1);
    acc[0] = 1;

    for (int v : g[c]) if (!removed[v]) {
        vector<int> depths;
        collect_depths(v, c, 1, depths); // depths[d] = 节点数，深度从1开始
        vector<long long> b(depths.size());
        for (size_t i = 0; i < depths.size(); ++i) b[i] = depths[i];

        // 用卷积计算 acc * b，得到通过质心且一端在已处理子树，另一端在当前子树的对数
        vector<long long> conv = convolution(acc, b);
        // conv[k] 表示距离为 k 的对数（k >= 1）
        for (size_t k = 0; k < conv.size() && k < (size_t)N; ++k) {
            ans[k] += conv[k];
        }
        // 将当前子树的深度计数合并到 acc
        if (acc.size() < b.size()) acc.resize(b.size());
        for (size_t i = 0; i < b.size(); ++i) acc[i] += b[i];
    }

    // 处理完质心后，递归分解各子树
    for (int v : g[c]) if (!removed[v]) {
        decompose(v);
    }
}
// ---------- Centroid end ----------

void main() { decompose(1); }

}
using Tmp::ans;
inline LL qpow(LL b, __int128 n) {
    b %= MOD; n %= MOD - 1;
    LL res = 1;
    while (n) {
        if (n & 1) res = res * b % MOD;
        b = b * b % MOD; n >>= 1;
    }
    return res;
}
struct BruteForce {
    int f[10005];
    void dfs(int u, int pre) {
        for (int v : g[u]) {
            if (v == pre) continue;
            f[v] = f[u] + 1;
            dfs(v, u);
        }
    }
    void solve(int n, LL A, LL k) {
        LL ans = 0;
        auto all = (__int128)n * (A+1);
        for (int i = 1; i <= n; i++) {
            f[i] = 1;
            dfs(i, 0);
            for (int j = 1; j <= n; j++) {
                auto circ = (__int128)f[j] * (A+1);
                int flag = circ & 1 ? -1 : 1;
                (ans += (qpow(k-1, circ) + flag * (k-1) % MOD + MOD) * qpow(k-1, all - circ) % MOD) %= MOD;
            }
        }
        printf("%lld\n", ans);    
    }
};
int main() {
    LL A, k;
    fin >> n >> A >> k;
    for (int i = 1, u, v; i < n; i++) {
        fin >> u >> v;
        g[u].push_back(v);
        g[v].push_back(u);
    }
    // (new BruteForce) -> solve(n, A, k);

    Tmp::main();
    ans[0] = n;
    for (int i = 1; i < n; i++) ans[i] <<= 1;
    LL sum = 0;
    LL posi = (k - 1) % MOD, nega = ((1 - k) % MOD + MOD) % MOD, all = (__int128)n * (A+1);
    for (int i = 1; i <= n; i++) {
        LL circ = (__int128)i * (A+1);
        LL now = (qpow(k-1, circ) + (circ & 1 ? nega : posi)) * qpow(k-1, all - circ) % MOD;
        sum += ans[i-1] % MOD * now % MOD;
        if (sum >= MOD) sum -= MOD;
    }
    fout << sum << '\n';
    return 0;
}