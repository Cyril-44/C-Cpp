#include <stdio.h>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <string>
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) ((void)0)
#endif
constexpr int N = 100005;
class FastIS{static constexpr size_t P=1<<20;char r[P];char*u,*G;FILE*I;public:FastIS(FILE*Y=stdin):u(nullptr),G(nullptr),I(Y){}__attribute__((always_inline))inline int get(){return(u==G)&&(G=(u=r)+fread(r,1,P,I),u==G)?EOF:*u++;}template<typename A,typename=typename std::enable_if<std::is_arithmetic<A>::value>::type>FastIS&operator>>(A&j){if _GLIBCXX17_CONSTEXPR(std::is_same<A,bool>::value){int a=get();while(~a&&a!=(char)(48)&&a!=(char)(49))a=get();j=static_cast<bool>(a&1);}else if _GLIBCXX17_CONSTEXPR(std::is_same<A,char>::value){j=get();while(j==(char)(32)||j==(char)(13)||j==(char)(10))j=get();}else if _GLIBCXX17_CONSTEXPR(std::is_integral<A>::value){if _GLIBCXX17_CONSTEXPR(std::is_unsigned<A>::value){int a=get();while(~a&&(a<(char)(48)||a>(char)(57)))a=get();for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));}else{int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;for(j=0;a>=(char)(48)&&a<=(char)(57);a=get())j=(j<<3)+(j<<1)+(a^(char)(48));j=m?-j:j;}}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<A>::value){int a=get();bool m=false;while(~a&&(a<(char)(48)||a>(char)(57))&&(a^(char)(45)))a=get();if(a==(char)(45))a=get(),m=true;using E=typename std::conditional<std::is_same<A,float>::value,std::uint32_t,std::uint64_t>::type;E Q;for(Q=0;a>=(char)(48)&&a<=(char)(57);a=get())Q=(Q<<3)+(Q<<1)+(a^(char)(48));j=Q;if(a==(char)(46)){E a=1;for(a=get();a>=(char)(48)&&a<=(char)(57);a=get())j+=(a^(char)(48))*(1./(a*=10));}if(m)j=-j;}else throw;return*this;}FastIS&operator>>(char*i){int a=get();while(~a&&(a==(char)(32)||a==(char)(13)||a==(char)(10)))a=get();while(~a&&(a!=(char)(32)&&a!=(char)(13)&&a!=(char)(10)))*i++=a,a=get();return*this;}}fin;class FastOS{static constexpr size_t P=1<<20;static constexpr double x=1e-9;char r[P],*b;const char*l;FILE*w;int o;public:FastOS(FILE*Y=stdout):b(r),l(r+P),w(Y),o(-1){setvbuf(w,nullptr,_IONBF,0);}~FastOS(){flush();fclose(w);}FastOS&flush(){fwrite(r,b-r,1,w);return*this;}FastOS&put(int g){*b++=g;if(b==l)fwrite(r,P,1,w),b=r;return*this;}FastOS&setprecision(const int&t){o=t;return*this;}template<typename a,typename=typename std::enable_if<std::is_arithmetic<a>::value>::type>FastOS&operator<<(a j){if _GLIBCXX17_CONSTEXPR(std::is_same<a,char>::value)put(j);else if _GLIBCXX17_CONSTEXPR(std::is_integral<a>::value){if _GLIBCXX17_CONSTEXPR(std::is_signed<a>::value)if(j<0)put((char)(45)),j=-j;static uint8_t i[40];i[*i=1]=j%10;while(j/=10)i[++(*i)]=j%10;while(*i)put(i[(*i)--]|(char)(48));}else if _GLIBCXX17_CONSTEXPR(std::is_floating_point<a>::value){if(j<0)put((char)(45)),j=-j;using E=typename std::conditional<std::is_same<a,float>::value,std::uint32_t,std::uint64_t>::type;E v=static_cast<E>(j);a B=j-v;*this<<v;if(o>0||(o<0&&B>x)){put((char)(46));for(int R=0;(o>0&&R<o)||(o<0&&B>x);R++){int X=static_cast<int>(B*=10);put(X|(char)(48));B-=X;}}}else throw;return*this;}FastOS&operator<<(char*i){while(*i)put(*i++);return*this;}FastOS&operator<<(const std::string&s){return*this<<s.c_str();}}fout;
constexpr int64_t Res[] = {515472268187439ll, 123456797654321ll, 976543212345679ll};
namespace DSU {
    struct Node {
        int fa, sign;
        int64_t val;
        bool solved;
        inline int& operator*() { return fa; }
    } dat[N];
    // real_i = sign * real_{fa(i)} + x_i
    inline void init(int n) {
        for (int i = 1; i <= n; i++)
            dat[i] = {i, 1, 0, false};
    }
    inline int getfa(int u) {
        if (*dat[u] == u) return u;
        int tp = *dat[u];
        debug("    [GETFA] [Trace] [Get] a_%d = %ca_%d + %lld\n", u, dat[u].sign>0?'+':'-', tp, dat[u].val);
        *dat[u] = getfa(tp);
        if (tp == *dat[u] && dat[u].solved == dat[tp].solved) return tp;
        dat[u].val += dat[u].sign * dat[tp].val;
        dat[u].sign *= dat[tp].sign;
        dat[u].solved = dat[tp].solved;
        if (dat[u].solved) debug("    [GETFA] [Trace Back] [*Solved] a_%d = %lld\n", u, dat[u].val);
        else debug("    [GETFA] [Trace Back] [Get] a_%d = %ca_%d + %lld\n", u, dat[u].sign>0?'+':'-', *dat[u], dat[u].val);
        return *dat[u];
    }
    inline bool merge(int u, int v, int sum) {
        debug("   [MERGE] [Resolving] a_%d + a_%d = %d\n", u, v, sum);
        int fu = getfa(u), fv = getfa(v);
        if (fu != fv) { // s->sign r->real   s_u * r_u + x_u + s_v * r_v + x_v = sum
            debug("    [MERGE] [Found diff] a_%d\n");
            if (dat[fu].solved && dat[fv].solved)
                return dat[u].val + dat[v].val == sum;
            if (dat[fu].solved) std::swap(u, v), std::swap(fu, fv);
            dat[fu] = {fv, dat[u].sign * -dat[v].sign, dat[u].sign * (sum - dat[u].val - dat[v].val), dat[v].solved};
            if (dat[fu].solved) debug("   [MERGE] [*Solved] a_%d = %lld\n", fu, dat[fu].val);
            else debug("   [MERGE] [Get] a_%d = %ca_%d + %lld\n", fu, dat[fu].sign>0 ? '+' : '-', fv, dat[fu].val);
        } else { // kx + b = sum;
            int k = dat[u].sign + dat[v].sign;
            int64_t b = dat[u].val + dat[v].val;
            debug("   [MERGE] [Found same] a_%d [Equation] %d = (a_%d = %ca_%d + %lld) + (a_%d = %ca_%d + %lld)\n", fu, sum, 
                u, (dat[u].sign>0)?'+':'-', fu, dat[u].val,
                v, (dat[v].sign>0)?'+':'-', fv, dat[v].val);
            if (k == 0) return b == sum;
            if (sum - b & 1) return false;
            int64_t solve = (sum - b) / k;
            if (dat[fu].solved) return dat[fu].val == solve;
            dat[fu].solved = true;
            dat[fu].val = solve;
            debug("   [MERGE] [*Solved] a_%d = %lld\n", fu, dat[fu].val);
        }
        return true;
    }
}
int main() {
    freopen("equation.in", "r", stdin);
    freopen("equation.out", "w", stdout);
#ifdef DEBUG
    freopen("equation.err", "w", stderr);
#endif
    int T, M, W, n, Q, t, x, y, v, tot_write_count=0, wish_write_count=0, tot_query_count=0;
    int64_t L;
    auto write = [&L, &tot_write_count] (int res, int64_t ans = 0) {
        ++tot_write_count;
        switch (res) {
        case 0:
            fout << ans << '\n';
            L = ans + Res[0];
            break;
        case 1:
            fout << "Ambiguous\n";
            L = Res[1];
            break;
        case 2:
            fout << "Impossible\n";
            L = Res[2];
        }
    };
    fin >> T >> M >> W;
    while (T--) {
        L = 0;
        fin >> n >> Q;
        DSU::init(n);
        bool failed = false;
        while (Q--) {
            debug("[Query %d>%d]\n", T, ++tot_query_count);
            fin >> t >> x >> y >> v;
            t = (t + M * L) & 1;
            x = (x - 1 + M * L) % n + 1;
            y = (y - 1 + M * L) % n + 1;
            v = (v + M * L) % (W + 1);
            if (t) { // If Ax = v, Query Ay
                ++wish_write_count;
                if (failed) { write(2); continue; } 
                int fx = DSU::getfa(x), fy = DSU::getfa(y);
                debug("  [OUTPUT %d] %d = (a_%d = %s %lld) + (a_%d = %s %lld)\n", wish_write_count, v, 
                        x, DSU::dat[x].solved ? "" : ((DSU::dat[x].sign>0?"+a_":"-a_") + std::to_string(fx) + " +").c_str(), DSU::dat[x].val, 
                        y, DSU::dat[y].solved ? "" : ((DSU::dat[y].sign>0?"+a_":"-a_") + std::to_string(fy) + " +").c_str(), DSU::dat[y].val);
                switch ((DSU::dat[x].solved << 1 | DSU::dat[y].solved)) {
                case 0:
                    if (fx == fy) { // s(x) * X + x(x) = v
                        int64_t solve = DSU::dat[x].sign * (v - DSU::dat[x].val);
                        debug(" [FINAL 001] Found ans: a_%d=%ca_%d+%lld=%d ==> a_%d=%lld ==> a_%d=%lld\n", x, DSU::dat[x].sign>0?'+':'-', fx, DSU::dat[x].val, v, fx, solve, y, DSU::dat[y].sign * solve + DSU::dat[y].val);
                        write(0, DSU::dat[y].sign * solve + DSU::dat[y].val);
                    } else {
                        debug(" [FINAL 000] **AMBIGUOUS** solve not found.\n");
                        write(1);
                    }
                    break;
                case 1:
                    debug(" [FINAL 01-] Found ans: a_%d=%lld\n", y, DSU::dat[y].val);
                    write(0, DSU::dat[y].val);
                    break;
                case 2:
                if (DSU::dat[x].val != v) {
                        debug(" [FINAL 100] **IMPOSSIBLE** %lld doesn't match %d!\n", DSU::dat[x].val, v);
                        write(2);
                    } else {
                        debug(" [FINAL 101] **AMBIGUOUS**\n");
                        write(1);
                    }
                    break;
                case 3:
                    if (DSU::dat[x].val != v) {
                        debug(" [FINAL 110] **IMPOSSIBLE** %lld doesn't match %d!\n", DSU::dat[x].val, v);
                        write(2);
                    }
                    else {
                        debug(" [FINAL 111] Found ans: solved %lld\n", DSU::dat[y].val);
                        write(0, DSU::dat[y].val);
                    }
                }
            } else { // Let Ax + Ay = v
                if (failed) continue;
                if (!DSU::merge(x, y, v)) {
                    failed = true;
                    debug(" [FAIL] a_%d + a_%d = %lld, mismatched %d\n", x, y, DSU::dat[x].val + DSU::dat[y].val, v);
                }
            }
        }
        debug("[Tot write count] %d, [Wish] %d\n", tot_write_count, wish_write_count);
    }
    return 0;
}
