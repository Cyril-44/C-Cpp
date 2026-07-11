#include <bits/stdc++.h>
using namespace std;

using ll = long long;

namespace FastIO {
    static const int S = 1 << 20;
    char ibuf[S], obuf[S];
    int ipos = 0, ilen = 0, opos = 0;
    inline char gc() {
        if (ipos == ilen) {
            ilen = fread(ibuf, 1, S, stdin);
            ipos = 0;
            if (ilen == 0) return EOF;
        }
        return ibuf[ipos++];
    }
    template <typename T>
    inline bool read(T &x) {
        char c;
        T sign = 1;
        x = 0;
        c = gc();
        if (c == EOF) return false;
        while (c != '-' && (c < '0' || c > '9')) {
            c = gc();
            if (c == EOF) return false;
        }
        if (c == '-') {
            sign = -1;
            c = gc();
        }
        while (c >= '0' && c <= '9') {
            x = x * 10 + (c - '0');
            c = gc();
        }
		x *= sign;
        return true;
    }
    inline void pc(char c) {
        if (opos == S) {
            fwrite(obuf, 1, opos, stdout);
            opos = 0;
        }
        obuf[opos++] = c;
    }
    template <typename T>
    inline void write(T x) {
        if (x == 0) {
            pc('0');
            return;
        }
        if (x < 0) {
            pc('-');
            x = -x;
        }
        char s[30];
        int cnt = 0;
        while (x > 0) {
            s[cnt++] = char('0' + x % 10);
            x /= 10;
        }
        while (cnt--) pc(s[cnt]);
    }
    template <typename T>
    inline void writeln(T x) {
        write(x);
        pc('\n');
    }
    inline void flush() {
        fwrite(obuf, 1, opos, stdout);
        opos = 0;
    }
}

using namespace FastIO;

int main() {
    int n, q;
    read(n);
    read(q);

    ll ans = 114514;
    writeln(ans);

    flush();
    return 0;
}
