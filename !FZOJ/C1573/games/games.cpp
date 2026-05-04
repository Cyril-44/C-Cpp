#include <cstdio>
#include <cassert>
#include <algorithm>
struct FastI {
    char buf[1 << 20], *p1, *p2;
    FastI() : buf{}, p1{}, p2{} {}
    inline char get() {
        if (__builtin_expect(p1 == p2, 0)) *buf = '\0', p2 = (p1=buf) + fread(buf, 1, sizeof buf, stdin);
        return *p1++;
    }
    template<typename T> inline void operator()(T &x) {
        char ch = get();
        bool flg = false;
        while ((ch < '0' || ch > '9') && (ch ^ '-')) ch = get();
        if (ch == '-') flg = true, ch = get();
        for (x = 0; ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
        if (flg) x = -x;
    }
} in;
struct FastO {
    char buf[1 << 20], *p1;
    const char *p2;
    FastO() : buf{}, p1(buf), p2(buf + sizeof(buf)) {}
    ~FastO() { fwrite(buf, p1 - buf, 1, stdout); }
    inline void put(char c) {
        if (__builtin_expect(p1 == p2, 0)) fwrite(buf, sizeof buf, 1, stdout), p1 = buf;
        *p1++ = c;
    }
    inline void operator()(int x) {
        static int sta[20];
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) put(sta[sta[0]--] | '0');
    }
} out;
int n;
#define X second
#define Y first
constexpr int N = 300005;
using pii_t = std::pair<int64_t,int64_t>;
pii_t a[N];
using std::abs;
inline void sub1() {
    out.put((a[1].Y >= abs(a[1].X)) | '0');
    out.put('\n');
}
int main() {
    int T; in(T);
    for (int tc = 1; tc <= T; tc++) {
        in(n);
        for (int i = 0; i <= n; i++)
            in(a[i].X), in(a[i].Y);
        for (int i = 1; i <= n; i++) {
            a[i].X -= a[0].X;
            a[i].Y -= a[0].Y;
        }
        a[0] = {0, 0};
        switch (T) {
        // case (int)3e6: sub1(); break;
        default: {
/* Lemma 1: 答案一定为 n/n-1。
 * 考虑一定 y 从小往大去抓捕，然后上下分成两段，也就是 x 极差 <= 1 和 > 1 的上下两段。
 * 上面一段只要能到达最底下，上面的就都搞完了。
 * 下面只需要对于每一个点，抓捕它之后，另一个点尽力逃脱都逃不走就是 n。
 */
            for (int i = 1; i <= n; i++)
                if (a[i].Y < abs(a[i].X))
                    goto br;
            std::sort(a+1, a+1 + n); // 见 line 37,38，先 Y 再 X
            int64_t lx = a[n].X, rx = a[n].X;
            int pos;
            for (pos = n-1; pos > 0; pos--) {
                lx = std::min(a[pos].X, lx);
                rx = std::max(a[pos].X, rx);
                if (rx - lx > 1) break;
            }
            int64_t mxIdx1[2]{a[pos].X, 0}, mxIdx2[2]{a[pos].X, 0};
            int64_t mxVal1[2]{a[pos].X - a[pos].Y}, mxVal2[2]{a[pos].X + a[pos].Y};
            for (int i = pos - 1; i > 0; i--) { // Any j (>i) satisfies |xj-xi| > yj-2yi ==> answer=n-1
                int64_t mx1 = (mxIdx1[0] == a[i].X) ? (mxIdx1[1] ? mxVal1[1] : -(1ll<<60)) : mxVal1[0],
                        mx2 = (mxIdx2[0] == a[i].X) ? (mxIdx2[1] ? mxVal2[1] : -(1ll<<60)) : mxVal2[0];
                if (a[i+1].Y - 2*a[i].Y < 0 ||  // check min(yj-2yi)<0           ==>
                    a[i].X - 2*a[i].Y < mx1 ||  //  <== xi-2yi < xj-yj __________|
                    a[i].X + 2*a[i].Y < mx2)    //  Or  xi+2yi < xj+yj 
                    goto br;
                int64_t cur1 = a[i].X - a[i].Y, cur2 = a[i].X + a[i].Y;
                if (cur1 > mxVal1[0]) {
                    if (mxIdx1[0] == a[i].X) mxVal1[0] = cur1;
                    else mxIdx1[1] = mxIdx1[0], mxVal1[1] = mxVal1[0], mxIdx1[0] = a[i].X, mxVal1[0] = cur1;
                } else if (cur1 > mxVal2[0] && a[i].X != mxIdx1[0])
                    mxIdx1[1] = a[i].X, mxVal1[1] = cur1;
                if (cur2 > mxVal2[0]) {
                    if (mxIdx2[0] == a[i].X) mxVal2[0] = cur2;
                    else mxIdx2[1] = mxIdx2[0], mxVal2[1] = mxVal2[0], mxIdx2[0] = a[i].X, mxVal2[0] = cur2;
                } else if (cur2 > mxVal2[0] && a[i].X != mxIdx2[0])
                    mxIdx2[1] = a[i].X, mxVal2[1] = cur2;
                assert(mxIdx1[0] != mxIdx1[1]), assert(mxIdx2[0] != mxIdx2[1]);
            }
            // 捕捉了最后一个极差 > 1 的点的时候，前面一定是最多走了 a[pos].Y 步
            for (int i = pos + 1; i <= n; i++)
                if (a[i].X != a[pos].X && a[i].Y - 2*a[pos].Y < abs(a[i].X - a[pos].X))
                    goto br;
            out(n), out.put('\n'); break;
        } br: out(n-1), out.put('\n'); break; }
    }
    return 0;
}