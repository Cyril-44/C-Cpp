#include <cassert>
#include <cstdio>
#include <cstring>
#include <algorithm>
constexpr int N = 600005, MOD = 17171717;
char s1[N], s2[N];
int sa[N], rk[N], h[N];
inline void getSA(int n, char *s, int *sa, int *_rk, int *h, int sigma = 127) {
    static int tsa[N], tmp[N], cnt[N];
    _rk[n+1] = tmp[n+1] = 0;
    int *rk = _rk, *nrk = tmp;
    auto radixSort = [&]() {
        for (int i = 1; i <= n; i++) cnt[rk[i]]++;
        for (int i = 1; i <= sigma; i++) cnt[i] += cnt[i-1];
        for (int i = n; i >= 1; i--) sa[cnt[rk[tsa[i]]]--] = tsa[i];
        memset(cnt, 0, sizeof(int) * (sigma + 1));
    };
    for (int i = 1; i <= n; i++) rk[i] = s[i], tsa[i] = i;
    radixSort();
    for (int w = 1; w < n; w <<= 1) {
        int top = 0;
        for (int i = n - w + 1; i <= n; i++) tsa[++top] = i;
        for (int i = 1; i <= n; i++) if (sa[i] > w) tsa[++top] = sa[i] - w;
        radixSort();
        sigma = 0;
        for (int i = 1; i <= n; i++)
            nrk[sa[i]] = sigma += !(rk[sa[i]] == rk[sa[i-1]] && rk[sa[i]+w] == rk[sa[i-1]+w]);
        std::swap(rk, nrk);
    }
    for (int i = 1, w = 0; i <= n; i++) {
        if (rk[i] == 1) continue;
        if (w) --w;
        while (i+w <= n && s[i+w] == s[sa[rk[i]-1]+w]) ++w;
        h[rk[i]] = w;
    }
    if (rk != _rk) memcpy(_rk, rk, sizeof(int) * (n+1));
}
struct ModInt {
    ModInt& operator+=(const ModInt x) { if ((val += x.val) >= MOD) val -= MOD; return *this; }
    ModInt& operator-=(const ModInt x) { if ((val -= x.val) < 0) val += MOD; return *this; }
    ModInt& operator*=(const ModInt x) { val = 1ull*val*x.val%MOD; return *this; }
    friend ModInt operator+(ModInt x, const ModInt y) { return x += y; }
    friend ModInt operator-(ModInt x, const ModInt y) { return x -= y; }
    friend ModInt operator*(ModInt x, const ModInt y) { return x *= y; }
    ModInt(int v=0) : val(v) {}
    template<class T> explicit operator T() const { return static_cast<T>(val); }
private: int val;
};
int main() {
    scanf("%s", s1+1);
    int n = strlen(s1+1), idx1 = n+1, idx2 = 0;
    s1[idx1] = '#';
    for (int i = 1, j; i <= n; i = j) {
        for (j = i+1; j <= n && s1[i] == s1[j]; j++);
        if (j-i <= 1) {
            if (s1[idx1] != '#') s1[++idx1] = '#';
        } else {
            int cnt = j-i;
            for (int _ = cnt>>1; _--; s1[++idx1] = s1[i]);
            if (cnt & 1) {
                s1[++idx1] = '#';
                for (int _ = cnt>>1; _--; s1[++idx1] = s1[i]);
            } else if (cnt > 2) {
                s2[++idx2] = '#';
                for (int _ = (cnt>>1)-1; _--; s2[++idx2] = s1[i]);
            }
        }
    }
    if (s1[idx1] == '#') s1[idx1--] = '\0';
    memcpy(s1 + idx1+1, s2 + 1, idx2);
    auto calc = [&](int m, char* s, int *sa, int *rk, int *h) {
        getSA(m, s, sa, rk, h);
        for (int i = 2; i <= m; i++)
            if (sa[i] <= n) h[i] = std::min(h[i], n - sa[i] + 1);
            else if (sa[i-1] <= n) h[i] = std::min(h[i], n - sa[i-1] + 1);
        // for (int i = 1; i <= m; i++) printf("(%d) %*s\n", h[i], m-sa[i]+1, &s[sa[i]]);
        // for (int i = 2; i <= m; i++) printf("%d%c", h[i], " \n"[i==m]);
        // for (int i = 2; i <= m; i++) printf("%d%c", sa[i], " \n"[i==m]);
        /* for (int i = 1; i <= n; i++) {
            int p = rk[i];
            int now = n - i + 1;
            for (int j = p+1; j <= m; j++) {
                now = std::min(now, h[j]);
                if (sa[j] > n) ans += now;
            }
            now = n - i + 1;
            for (int j = p-1; j >= 1; j--) {
                now = std::min(now, h[j+1]);
                if (sa[j] > n) ans += now;
            }
        } */
        static int sta[N], l[N], r[N], cnt[N]; // 获取 l: 左边第一个比 h[i] 小/等于的下标+1，r: 右边第一个比 h[i] 小的下标-1，cnt: 要统计答案的位置。
        for (int i = 1; i <= m; i++) cnt[i] = (sa[i] > n) + cnt[i-1];
        for (int i = 2; i <= m; i++) {
            while (sta[0] && h[sta[sta[0]]] > h[i]) --sta[0];
            l[i] = sta[0] ? sta[sta[0]] + 1 : 2;
            sta[++sta[0]] = i;
        }
        sta[0] = 0;
        for (int i = m; i >= 2; i--) {
            while (sta[0] && h[sta[sta[0]]] >= h[i]) --sta[0];
            r[i] = sta[0] ? sta[sta[0]] - 1 : m;
            sta[++sta[0]] = i;
        }
        ModInt ans = 0;
        /*  for (i = 2..m) if (!sat[i-1])
              for (j = i..m) if (sat[j])
                ans += min{ a_{i..j} }
            for (i = 2..m) if (!sat[i])
              for (j = 2..i) if (sat[j-1])
                ans += min{ a_{j..i} }
        */
        auto copies = [&](int l, int r) { return cnt[r] - cnt[l-1]; };
        auto selves = [&](int l, int r) { return r - l + 1 - (cnt[r] - cnt[l-1]); };
        for (int i = 2; i <= m; i++) {
            // fprintf(stderr, "[%d,%d] %d * (%d*%d + %d*%d)\n", l[i], r[i], h[i], selves(l[i]-1, i-1), copies(i, r[i]), copies(l[i]-1, i-1), selves(i, r[i]));
            ans += ModInt(h[i]) * selves(l[i]-1, i-1) * copies(i, r[i]);
            ans += ModInt(h[i]) * copies(l[i]-1, i-1) * selves(i, r[i]);
        }
        return ans;
    };
    printf("%d\n", calc(idx1 + idx2, s1, sa, rk, h));
    return 0;
}