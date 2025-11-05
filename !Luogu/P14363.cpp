#include <bits/stdc++.h>
using namespace std;
class FastIS{static constexpr size_t P=1<<20;char t[P];char*y,*I;FILE*a;public:FastIS(FILE*G=stdin):y(nullptr),I(nullptr),a(G){}~FastIS(){fclose(a);}__attribute__((always_inline)) int b(){return(y==I)&&(I=(y=t)+fread(t,1,P,a),y==I)?EOF:*y++;}template<typename F>typename std::enable_if<std::is_unsigned<F>::value&&std::is_integral<F>::value,FastIS&>::type inline operator>>(F&K){int w=b();while(~w&&(w<'0'||w>'9'))w=b();for(K=0;w>='0'&&w<='9';w=b())K=(K<<3)+(K<<1)+(w^'0');return*this;}template<typename F>typename std::enable_if<std::is_signed<F>::value&&std::is_integral<F>::value&&!std::is_same<F,bool>::value&&!std::is_same<F,char>::value,FastIS&>::type inline operator>>(F&K){int w=b();bool d=false;while(~w&&(w<'0'||w>'9')&&(w^'-'))w=b();if(w=='-')w=b(),d=true;for(K=0;w>='0'&&w<='9';w=b())K=(K<<3)+(K<<1)+(w^'0');K=d?-K:K;return*this;}template<typename F>typename std::enable_if<std::is_floating_point<F>::value,FastIS&>::type inline operator>>(F&K){int w=b();bool d=false;while(~w&&(w<'0'||w>'9')&&(w^'-'))w=b();if(w=='-')w=b(),d=true;using u=typename std::conditional<std::is_same<F,float>::value,std::uint32_t,std::uint64_t>::type;u Z;for(Z=0;w>='0'&&w<='9';w=b())Z=(Z<<3)+(Z<<1)+(w^'0');K=Z;if(w=='.'){u D=1;for(w=b();w>='0'&&w<='9';w=b())K+=(w^'0')*(1./(D*=10));}if(d)K=-K;return*this;}inline FastIS&operator>>(bool&F){int w=b();while(~w&&w!='0'&&w!='1')w=b();F=static_cast<bool>(w&1);return*this;}inline FastIS&operator>>(char&O){int w=b();while(w==' '||w=='\r'||w=='\n')w=b();O=w;return*this;}inline FastIS&operator>>(char*O){int w=b();while(~w&&(w==' '||w=='\r'||w=='\n'))w=b();while(~w&&(w!=' '&&w!='\r'&&w!='\n'))*O++=w,w=b();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double X=1e-9;char t[P],*A;const char*k;FILE*B;int e;public:FastOS(FILE*G=stdout):A(t),k(t+P),B(G),e(-1){setvbuf(B,nullptr,_IONBF,0);}~FastOS(){E();fclose(B);}FastOS&E(){fwrite(t,A-t,1,B);return*this;}FastOS&S(int U){*A++=U;if(A==k)fwrite(t,P,1,B),A=t;return*this;}FastOS&L(const int&V){e=V;return*this;}template<typename F>typename std::enable_if<std::is_unsigned<F>::value&&std::is_integral<F>::value,FastOS&>::type inline operator<<(F K){static int O[32];O[*O=1]=K%10;while(K/=10)O[++(*O)]=K%10;while(*O)S(O[(*O)--]|'0');return*this;}template<typename F>typename std::enable_if<std::is_signed<F>::value&&std::is_integral<F>::value&&!std::is_same<F,char>::value,FastOS&>::type inline operator<<(const F&K){if(K<0)S('-')<<(typename std::make_unsigned<F>::type)(-K);else*this<<(typename std::make_unsigned<F>::type)(K);}template<typename F>typename std::enable_if<std::is_floating_point<F>::value,FastOS&>::type inline operator<<(F K){if(K<0)S('-'),K=-K;using u=typename std::conditional<std::is_same<F,float>::value,std::uint32_t,std::uint64_t>::type;u Q=static_cast<u>(K);F q=K-Q;*this<<Q;if(e>0||e<0&&q>X){S('.');for(int a=0;e>0&&a<e||e<0&&q>X;a++){int M=static_cast<int>(q*=10);S(M|'0');q-=M;}}return*this;}FastOS&operator<<(const char&U){S(U);return*this;}}fout;
constexpr int N = 200005, L = 5000005;
char a[L], b[L];
string s[N][2], t[N][2];
constexpr uint64_t B1 = 31, B2 = 37, P1 = 5e16 + 9, P2 = 4e17 + 7;
using Hash_t = pair<uint64_t, uint64_t>;
inline Hash_t gethash(int st, int ed) {
    Hash_t hash;
    for (int i = st; i <= ed; i++) {
        hash.first = (hash.first * B1 + (a[i] - 'a')) % P1;
        hash.second = (hash.second * B2 + (b[i] - 'a')) % P2;
    }
    return hash;
}
struct Trie {
    map<int, int> son;
    int cnt;
} tr[L << 1];
int tot = 0;
int main() {
    int n, q;
    fin >> n >> q;
    map<Hash_t, pair<int,int>> mp;
    for (int i = 1; i <= n; i++) {
        fin >> a >> b;
        s[i][0] = a, s[i][1] = b;
    }
    for (int i = 1; i <= q; i++) {
        fin >> a >> b;
        t[i][0] = a, t[i][1] = b;
    }
    for (int i = 1; i <= n; i++) {
        int l = -1, m = strlen(a), r = m - 1;
        while (++l < m)
            if (a[l] ^ b[l])
                break;
        if (l == m) continue;
        while (a[r] == b[r])
            --r;
        Hash_t hash = gethash(l, r);
        if (!mp[hash].first) mp[hash].first = ++tot;
        if (!mp[hash].second) mp[hash].second = ++tot;
        int u = mp[hash].first, now;
        for (int i = l-1; i >= 0; i--) {
            now = a[i] - 'a';
            if (!tr[u].son.count(now)) tr[u].son[now] = ++tot;
            u = tr[u].son[now];
        }
        ++tr[u].cnt;
        u = mp[hash].second;
        for (int i = r+1; i < m; i++) {
            now = a[i] - 'a';
            if (!tr[u].son.count(now)) tr[u].son[now] = ++tot;
            u = tr[u].son[now];
        }
        ++tr[u].cnt;
    }
    return 0;
}