<<<<<<< HEAD:Functions/Code Obfuscator/OBF.in.cpp
#include <string>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <bitset>
#include <vector>
class Demical {
    constexpr static size_t DIGITLEN = 1 << 17;
    constexpr static unsigned BASE = 1e9;
    constexpr static int BASE_I = 1e9;
    constexpr static int BASELEN = 9;
    bool sign;  // 0->positive 1->negative
    std::vector<unsigned> a;
    /// @brief Helper Function : fix array due to BASE
    /// @tparam T an integral type (aka. unsigned, unsigned long long)
    /// @param x the array to fix
   
template <typename T> inline void normalize(std::vector<T> &x) {
        for (size_t i = 0; i < x.size(); i++) {
            if (i+1 == x.size() && (x[i] >= BASE_I || x[i] < 0))
                x.emplace_back(); // Which is impossible
            if (x[i] >= BASE_I) x[i+1] += x[i] / BASE_I;
            if (x[i] < 0) x[i+1] -= ((-x[i] + BASE_I-1) / BASE_I);
            x[i] = (x[i] % BASE_I + BASE_I) % BASE_I;
        }
        a.resize(x.size());
        for (size_t i = 0; i < x.size(); i++)
            a[i] = static_cast<unsigned>(x[i]);
        while (!a.empty())
            if (*a.rbegin() == 0) a.pop_back();
            else break;
        if (a.empty()) sign = false;
    }
template <typename FuncType>
    inline char* read(FuncType method) {
        static char buf[DIGITLEN];
        int top(0);
        char ch = method();
        while ((ch < '0' || ch > '9') && (ch ^ '-')) ch = method();
        if (ch == '-') buf[top++] = '-', ch = method();
        while ((top ^ DIGITLEN) && ch >= '0' && ch <= '9')
            buf[top++] = ch, ch = method();
        buf[top] = '\0';
        return buf;
    }
    inline Demical(const bool &x, const std::vector<unsigned> &y) : sign(x), a(y) {}
    Demical(const char *_s, const bool &flg) : sign(false) {
        char *s = const_cast<char*>((*_s ^ '-') ? (_s) : (_s + 1));
        if (*s == '0') return;
        if (*_s == '-') sign = true;
        static char _buf[DIGITLEN];
        char* buf;
        int top;
        if (flg) {for (top = 0; s[top]; ++top); buf = s;}
        else {
            buf = _buf;
            for (top = 0; s[top]; ++top) {
                if (s[top] < '0' || s[top] > '9') throw;
                buf[top] = s[top];
            }
        }
        for (int i = 0, j = top-1; i < j; i++, --j)
            buf[i] ^= buf[j] ^= buf[i] ^= buf[j];
        a.resize((top + BASELEN - 1) / BASELEN);
        for (size_t i = 0, j = 0; i < a.size(); i++) {
            unsigned bs = 1;
            for (char k = BASELEN; (j ^ top) && k; j++, --k)
                a[i] += bs * (buf[j] ^ '0'), bs *= 10;
        }
    }
public:
    Demical() : sign(false) {}
template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, bool>::type> Demical(T x) : sign(false) {
        if (std::is_signed<T>::value && x < 0)
            sign = true, x = -x;
        while (x) {
            a.emplace_back(x % BASE);
            x /= BASE;
        }
    }
=======
>>>>>>> 565968d5a2ef961896dca4746c0dcfc5df1ed480:Functions/Code Obfuscator/OBF.in

using namespace std;
typedef long long ll;
template <class T> void in(T &x) {
    x = 0;
    char c = getchar();
    int f = 1;
    while (c < '0' || c > '9') {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9') {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
const int N = 2000010;
int n, cnt[N], len;
deque<pair<int, int>> ans;
bool divide(int l, int r) {
    // cerr<<l<<' '<<r<<endl;
    if (l > r)
        return 1;
    int m = l + r >> 1;
    for (int i = m; i >= l; i--) {
        if (cnt[i] >= 2) {
            int rto = i * 2 - l + 1;
            if (rto < m)
                break;
            if (divide(rto, r)) {
                // cerr<<l<<' '<<m<<' '<<r<<" LLL ";
                for (int j = rto - 1; j >= i; j--) {
                    ans.push_front(
                        {2, (i == j ? cnt[i] : cnt[j] + cnt[i * 2 - j])});
                    for (int k = 1; k <= cnt[j]; k++)
                        ans.push_front({1, 1000 + l});
                }
                for (int j = i - 1; j >= l; j--)
                    for (int k = 1; k <= cnt[j]; k++)
                        ans.push_front({1, 2000 + l});
                return 1;
            }
        }
        i = l + r - i;
        if (cnt[i] >= 2) {
            int lto = i * 2 - r - 1;
            if (lto > m + 1)
                break;
            if (divide(l, lto)) {
                for (int j = lto + 1; j < i; j++)
                    for (int k = 1; k <= cnt[j]; k++)
                        ans.push_back({1, 0});
                for (int j = i; j <= r; j++) {
                    for (int k = 1; k <= cnt[j]; k++)
                        ans.push_back({1, 0});
                    ans.push_back(
                        {2, (i == j ? cnt[i] : cnt[j] + cnt[i * 2 - j])});
                }
                return 1;
            }
        }
        i = l + r - i;
    }
    return 0;
}
void work() {
    in(n);
    int prev = 0;
    len = 0;
    for (int i = 1; i <= n; i++) {
        cnt[i] = 0;
        int a;
        in(a);
        if (prev == a)
            cnt[len]++;
        else
            cnt[++len] = 1, prev = a;
    }
    // for(int i=1;i<=len;i++)
    // cerr<<cnt[i]<<' '; cerr<<endl;
    if (divide(1, len)) {
        printf("Yes\n");
        printf("%d\n", (int)ans.size());
        while (!ans.empty()) {
            pair<int, int> p = ans.front();
            ans.pop_front();
            if (p.first == 1)
                printf("1\n");
            // if(p.first==1) printf("1 %d\n",p.second);
            else
                printf("2 %d\n", p.second);
        }
    } else
        printf("No\n");
    // cerr<<endl;
}
int main() {
    freopen("miao.in", "r", stdin);
    freopen("miao.out", "w", stdout);
    int t;
    in(t);
    while (t--)
        work();
}
/*
1
*/