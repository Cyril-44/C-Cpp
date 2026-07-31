#include <cstdio>
#include <cstring>
#include <algorithm>
#include <queue>
constexpr int N = 1000005;
int a[N], p[N], end[N], len[N];
int n;
bool jmpable[N];
using Jmp = std::pair<int,int>;
namespace FastI {
    char buf[1 << 20], *p1, *p2;
    [[gnu::always_inline]] inline char gc() {
        if (p1 == p2) [[unlikely]] p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++; 
    }
    inline void in(int &x) {
        char ch = gc();
        while (ch < '0' || ch > '9') ch = gc();
        for (x = 0; ch >= '0' && ch <= '9'; ch = gc())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
} using FastI::in;
namespace FastO {
    char buf[1 << 20], *p1=buf;
    const char* p2 = buf + sizeof(buf);
    [[gnu::always_inline]] inline void pc(char x) {
        if (p1 == p2) [[unlikely]] fwrite(buf, sizeof buf, 1, stdout), p1 = buf;
        *p1++ = x;
    }
    struct Cleaner { ~Cleaner() { fwrite(buf, p1-buf, 1, stdout); } } _cleaner;
    inline void out(int x) {
        static int sta[15];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) pc(sta[sta[0]--] | '0');
        pc('\n');
    }
} using FastO::out;
struct BIT {
    int tr[N];
    void upd(int p, int x) { for (; p <= n; p += p & -p) tr[p] += x; }
    int sum(int p) { int res = 0; for (; p > 0; p -= p & -p) res += tr[p]; return res; }
} f1, f2;
inline void solve() {
    in(n);
    for (int i = 1; i <= n; i++) {
        in(a[i]);
        f1.upd(i, 1), f2.upd(i, 1);
        p[a[i]] = i;
    }
    if (n <= 2) {
        printf("%d\n", n);
        return;
    }
    memset(jmpable, 0, sizeof(bool) * (n + 2));
    end[n] = 0, len[n] = 0;
    end[n-1] = n, len[n-1] = 2;
    // std::vector<Jmp> jmps{{n-1, n}};
    for (int i = n - 2; i >= 1; i--) {
        if ((p[i] > p[i+1]) == (p[i+1] > p[i+2])) end[i] = end[i+1];
        else end[i] = i+1;
        len[i] = end[i] - i + 1;
    }
    for (int i = 1; i < n; i++) {
        if (len[i] == (p[i] > p[i+1] ? f1.sum(p[i]) : f2.sum(n - p[i] + 1)))
            jmpable[i] = true;
        f1.upd(p[i], -1);
        f2.upd(n - p[i] + 1, -1);
    }
    f1.upd(p[n], -1), f2.upd(n - p[n] + 1, -1);
    // std::reverse(jmps.begin(), jmps.end());
    int so = 1, ans = 1;
    std::priority_queue<int, std::vector<int>, std::greater<int>> jmps;
    for (int i = 2; i <= n; i++) {
        if (jmpable[i]) {
            if (len[i] == 2) {
                if (jmpable[i+1]) {
                    if (len[i+1] == 2) ++so, ++ans;
                    else jmps.push(len[i+1]), i = end[i+1], ans += 2;
                } else {
                    if (so) jmps.push(2), i++, --so, ++ans;
                    else i++, so+=2, ans+=2;
                }
            } else if (len[i] == 3) {
                if (jmpable[i+2]) {
                    if (len[i+2] == 2) {
                        if (so) jmps.push(3), i+=2, --so, ++ans;
                        else jmps.push(2), i+=3, ++so, ans+=3;
                    } else {
                        if (so) jmps.push(3), i+=2, --so, ++ans;
                        else jmps.push(len[i+2]), so++, ans+=3, i = end[i+2];
                    }
                } else {
                    if (so) jmps.push(3), i+=2, --so, ++ans;
                    else {
                        int fi = jmps.top(); jmps.pop();
                        if (fi > 2) jmps.push(len[i]), i = end[i], ans += 2, jmps.push(fi - 1);
                        else jmps.push(len[i]), i = end[i], ans += 2, so += 2;
                    }
                }
            } else {
                if (jmpable[end[i]] && len[i] < len[end[i]]) --end[i], --len[i];
                if (so) jmps.push(len[i]), i = end[i], --so, ++ans;
                else {
                    int fi = jmps.top(); jmps.pop();
                    if (fi > 2) jmps.push(len[i]), i = end[i], ans += 2, jmps.push(fi - 1);
                    else jmps.push(len[i]), i = end[i], ans += 2, so += 2;
                }
            }
        }
        else ++so, ++ans;
    }
    out(ans);
}
int main() {
    int T = 1;
    in(T);
    while (T--) solve();
    return 0;
}