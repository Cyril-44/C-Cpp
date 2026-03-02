#include <cstdio>
#include <algorithm>
#include <set>
#include <cassert>
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
    std::multiset<int> st;
    for (int i = 0; !(i >> n); i++) st.insert(a[i]);
    for (int i = 1; i <= n; i++) {
        int val = *std::next(st.begin()) - *st.begin();
        std::multiset<int> st2;
        for (auto it = st.begin(); it != st.end(); ++it) {
            st2.insert(*it + val);
            if (val == 0) {
                if (*std::next(it) != *it) return false;
                it = st.erase(it);
            } else {
                auto del = st.find(*it + val);
                if (del == st.end()) return false;
                st.erase(del);
            }
        }
        if (st2.count(0)) {
            ans[i] = -val;
            st.swap(st2);
        }
        else ans[i] = val;
    }
    return true;
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
        else {
            auto attempt1 = [&](int val) {
                std::multiset<int> st;
                for (int i = 1; !(i >> n); i++) {
                    if (st.count(a[i])) st.erase(st.find(a[i]));
                    else st.insert(a[i] + val);
                }
                fprintf(stderr, "Missing %d +- %d (%d or %d)\n", *st.begin(), val, *st.begin() + val, *st.begin() - val);
                return st.size() == 1 && ((a[0] = *st.begin() + val, solve0(n)) || (a[0] = *st.begin() - val, solve0(n)));
            };
            auto attempt2 = [&](int val) {
                std::multiset<int> st;
                for (int i = (1<<n) - 1; i >= 1; i--)
                    if (st.count(a[i])) st.erase(st.find(a[i]));
                    else st.insert(a[i] - val);
                fprintf(stderr, "Missing %d +- %d (%d or %d)\n", *st.begin(), val, *st.begin() + val, *st.begin() - val);
                return st.size() == 1 && ((a[0] = *st.begin() + val, solve0(n)) || (a[0] = *st.begin() - val, solve0(n)));
            };
            int v1 = a[2] - a[1], v2 = a[(1<<n) - 1] - a[(1<<n) - 2];
            assert(attempt1(v1) || attempt1(v2) || attempt2(v1) || attempt2(v2));
        }
        for (int i = 1; i <= n; i++)
            out(ans[i]), out(' ');
        out('\n');
    }
    return 0;
}