#include <bits/stdc++.h>
#define ull unsigned long long
using namespace std;
namespace fasti {
const int bufl = 1 << 20;
const double base1[16] = {1,    1e-1, 1e-2,  1e-3,  1e-4,  1e-5,  1e-6,  1e-7,
                          1e-8, 1e-9, 1e-10, 1e-11, 1e-12, 1e-13, 1e-14, 1e-15};
const double base2[16] = {1, 1e1, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9, 1e10, 1e11, 1e12, 1e13, 1e14, 1e15};
struct IN {
    FILE *IT;
    char ibuf[bufl], *is = ibuf, *it = ibuf;
    IN() { IT = stdin; }
    IN(char *a) { IT = fopen(a, "r"); }
    inline char getChar() {
        if (is == it) {
            it = (is = ibuf) + fread(ibuf, 1, bufl, IT);
            if (is == it) return EOF;
        }
        return *is++;
    }
    template <typename Temp> inline void getInt(Temp &a) {
        a = 0;
        int b = 0, c = getChar();
        while (c < 48 || c > 57) b ^= (c == 45), c = getChar();
        while (c >= 48 && c <= 57) a = (a << 1) + (a << 3) + c - 48, c = getChar();
        if (b) a = -a;
    }
    template <typename Temp> inline void getDouble(Temp &a) {
        a = 0;
        int b = 0, c = getChar(), d = 0;
        __int128 e = 0, f = 0;
        while (c < 48 || c > 57) b ^= (c == 45), c = getChar();
        while (c >= 48 && c <= 57) e = (e << 1) + (e << 3) + c - 48, c = getChar();
        if (c == 46) {
            c = getChar();
            while (c >= 48 && c <= 57) d++, f = (f << 1) + (f << 3) + c - 48, c = getChar();
        }
        a = e + base1[d] * f;
        if (b) a = -a;
    }
    IN &operator>>(char &a) {
        a = getChar();
        return *this;
    }
    IN &operator>>(char *a) {
        do { *a = getChar(); } while (*a <= 32);
        while (*a > 32) *++a = getChar();
        *a = 0;
        return *this;
    }
    IN &operator>>(string &a) {
        a = "";
        char b = getChar();
        while (b <= 32) b = getChar();
        while (b > 32) a += b, b = getChar();
        return *this;
    }
    IN &operator>>(int &a) {
        getInt(a);
        return *this;
    }
    IN &operator>>(long long &a) {
        getInt(a);
        return *this;
    }
    IN &operator>>(unsigned long long &a) {
        getInt(a);
        return *this;
    }
    IN &operator>>(__int128 &a) {
        getInt(a);
        return *this;
    }
    IN &operator>>(float &a) {
        getDouble(a);
        return *this;
    }
    IN &operator>>(double &a) {
        getDouble(a);
        return *this;
    }
    IN &operator>>(long double &a) {
        getDouble(a);
        return *this;
    }
};
} // namespace fasti
using fasti::IN;
IN fin;
mt19937 rd(random_device{}());
int n, m, q, fa[5000005];
ull hsh[5000005], z[5000005];
string s;
vector<vector<int>> a;
int find(int x) {
    if (x == fa[x]) return x;
    return fa[x] = find(fa[x]);
}
int main() {
    ios::sync_with_stdio(false);
    fin >> n >> m >> q;
    s.resize(n + 1), a.resize(n + 1);
    for (int i = 1; i <= n; i++) {
        fin >> s;
        s = "0" + s;
        a[i].resize(m + 2);
        for (int j = 1; j <= m; j++) { a[i][j] = (s[j] != s[j - 1]); }
        a[i][m + 1] = s[m] == '1';
    }
    for (int j = 1; j <= m + 1; j++) {
        fa[j] = j;
        z[j] = (ull)rd() * rd() + rd();
        for (int i = 1; i <= n; i++) {
            if (a[i][j]) hsh[i] += z[j];
        }
    }
    while (q--) {
        int op, x, y;
        fin >> op >> x >> y;
        if (op == 1) {
            int fx = find(x), fy = find(y + 1);
            if (fx == fy) continue;
            ull nw = (ull)rd() * rd() + rd();
            for (int i = 1; i <= n; i++) {
                hsh[i] -= (a[i][fx] ? z[fx] : 0) + (a[i][fy] ? z[fy] : 0);
                a[i][fx] ^= a[i][fy];
                hsh[i] += (a[i][fx] ? nw : 0);
            }
            fa[fy] = fx;
            z[fx] = nw;
        } else {
            if (hsh[x] == hsh[y])
                putchar('1');
            else
                putchar('0');
        }
    }
    putchar('\n');
    return 0;
}