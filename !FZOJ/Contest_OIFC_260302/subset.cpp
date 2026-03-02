#include <cstdio>
#include <algorithm>
#include <vector>
#include <cassert>
#include <numeric>
namespace FastI {
    char buf[1 << 20], *p1{}, *p2{};
    inline char gc() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1 = buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    inline void in(int &x) {
        char ch = gc();
        signed char flg = 1;
        while ((ch < '0' || ch > '9') && ch != '-') ch = gc();
        if (ch == '-') ch = gc(), flg = -1;
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch & 0xf);
        x *= flg;
    }
} using FastI::in;
struct FastO {
    char buf[1 << 20], *p3; const char *p4;
    FastO() : p3(buf), p4(buf + sizeof(buf)) {}
    inline void flush() { fwrite(buf, p3 - buf, 1, stdout), p3 = buf; }
    ~FastO() { flush(); }
    inline void operator()(char ch) {
        if (__builtin_expect(p3 == p4, 0)) fwrite(buf, sizeof buf, 1, stdout), p3 = buf;
        *p3++ = ch;
    }
    inline void operator()(int x) {
        if (x < 0) (*this)('-'), x = -x;
        static int sta[16];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) (*this)((char)(sta[sta[0]--] | '0'));
    }
} out;

constexpr int N = 20;
int a[1 << N], ans[N+1];
inline bool solve0(int n) {
    std::vector<int> arr(a, a+(1<<n));
    std::inplace_merge(arr.begin(), arr.begin()+1, arr.end());
    int sum = arr.front() + arr.back();
    static int que[1 << N];
    for (int i = 1; i <= n; i++) {
        int val = arr[1] - arr[0];
        if (val > (int)1e8) return false;
        std::vector<int> a1, a2; a1.reserve(1 << n-i+1), a2.reserve(1 << n-i+1);
        int hd=0, tl=0;
        for (int x : arr) {
            if ((hd^tl) && que[hd] == x) { ++hd; continue; }
            que[tl++] = x + val;
            a1.push_back(x);
            a2.push_back(x + val);
        }
        if (hd^tl) return false;
        if (std::count(a2.begin(), a2.end(), 0))
            ans[i] = -val, arr.swap(a2);
        else ans[i] = val, arr.swap(a1);
    }
    return arr == std::vector<int>{0};
}
int main() {
    freopen("subset.in", "r", stdin);
    freopen("subset.out", "w", stdout);
    int Tid, T; in(Tid), in(T);
    while (T--) {
        int n, k; in(n), in(k);
        for (int i = k; !(i >> n); i++) in(a[i]);
        std::sort(a+k, a+(1<<n));
        if (k == 0) assert(solve0(n));
        else if (n == 1) ans[1] = a[1];
        else if (Tid == 5 && false) {
            auto attempt = [&] (int val) {
                for (int i = 1; !(i >> n); i++) {
                    a[0] = a[i] - val; if (solve0(n)) return true;
                    a[0] = a[i] + val; if (solve0(n)) return true;
                }
                return false;
            };
            assert(attempt(a[2] - a[1]) || attempt(a[(1<<n)-1] - a[(1<<n)-2]));
        } else {
            long long sum = 0;
            for (int i = 1; !(i >> n); i++) sum += a[i];
            // (sum+a[0]) % 2^{n-1} = 0  ===>  -sum===a[0] (mod 2^{n-1})
            constexpr int MX = (int)1e8 + (int)5e6;
            const int MOD = 1 << n-1;
            /* int base = (MOD - sum % MOD) % MOD;
            ans[n] = (int)-2e9;
            for (int r = base, l = base - MOD; r <= MX || l >= -MX; r += MOD, l -= MOD) {
                a[0] = l; if (solve0(n)) break;
                a[0] = r; if (solve0(n)) break;
            }
            assert(ans[n] != (int)-2e9); */
            long long tp;
            assert((tp = (a[1] + a[(1<<n)-1]) * 1ll * MOD - sum, (int)-1e8 <= tp && tp <= (int)1e8 && (a[0] = tp, solve0(n)))
                || (tp = (a[1] + a[(1<<n)-2]) * 1ll * MOD - sum, (int)-1e8 <= tp && tp <= (int)1e8 && (a[0] = tp, solve0(n)))
                || (tp = (a[2] + a[(1<<n)-1]) * 1ll * MOD - sum, (int)-1e8 <= tp && tp <= (int)1e8 && (a[0] = tp, solve0(n))));
        }
        for (int i = 1; i <= n; i++)
            out(ans[i]), out(' ');
        out('\n');
    }
    return 0;
}