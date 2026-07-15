#include <bits/stdc++.h>
struct FastI {
    char buf[1 << 20], *p1, *p2;
    FILE* f;
    FastI(const char* fn) : p1(nullptr), p2(nullptr), f(fopen(fn, "r")) {}
    ~FastI() { fclose(f); }
    char get() {
        if (__builtin_expect(p1 == p2, 0)) p2 = (p1=buf) + fread(buf, 1, sizeof buf, f);
        return *p1++;
    }
    void operator()(int &x) {
        char ch = get();
        while (ch < '0' || ch > '9') ch = get();
        for (x = 0; ch >= '0' && ch <= '9'; ch = get())
            x = (x << 3) + (x << 1) + (ch ^ '0');
    }
};
struct FastO {
    char buf[1 << 20], *p3;
    const char* p4;
    FILE* f;
    FastO(const char* fn) : p3(buf), p4(buf + sizeof buf), f(fopen(fn, "w")) {}
    void flush() { fwrite(buf, p3-buf, 1, f); }
    ~FastO() { flush(); fclose(f); }
    void put(char ch) {
        if (__builtin_expect(p3 == p4, 0)) fwrite(buf, sizeof buf, 1, f), p3 = buf;
        *p3++ = ch;
    }
    int8_t sta[10];
    void operator()(int x) {
        sta[sta[0] = 1] = x % 10;
        while (x /= 10) sta[++sta[0]] = x % 10;
        while (sta[0]) put(sta[sta[0]--] | '0');
    }
};

#define For(i, s, t) for (int i = (s); i <= (t); i++)
constexpr int N = 1000005;
int n, a[N], d[N], ans[N];
struct SegTr {
    int tr[N<<2], lz[N<<2];
    void build() { std::fill(tr+1, tr+(N<<2), 1); }
    void pushup(int u) { tr[u] = std::min(tr[u<<1], tr[u<<1|1]); }
    void pushdown(int u) {
        if (!lz[u]) return;
        lz[u<<1] += lz[u], lz[u<<1|1] += lz[u];
        tr[u<<1] += lz[u], tr[u<<1|1] += lz[u];
        lz[u] = 0;
    }
    int L;
    void upd(int u, int l, int r) {
        if (l == r) {
            if (tr[u] == 1) {
                tr[u] = l;
                ans[0] = std::max(ans[0], ++ans[l]);
            }
            else --tr[u];
            return;
        }
        if (L <= l && tr[u] != 1) {
            --tr[u], --lz[u];
            return;
        }
        int mid = l + r >> 1;
        pushdown(u);
        if (L <= mid) upd(u<<1, l, mid);
        upd(u<<1|1, mid+1, r);
        pushup(u);
    }
    void update(int pos) { L = pos; upd(1, 1, n); }
} f;
int main() {
    FastI in("doll.in"); FastO out("doll.out");
    in(n);
    For(i, 1, n) in(a[i]);
    f.build();
    For(i, 1, n) {
        // roF(j, a[i], 0) ++d[j];
        // For(j, a[i]+1, n-1) ++d[j];
        /* int l = 1, r = i;
        while (l <= r) {
            int mid = l + r >> 1;
            bool flg = true;
            int cnt = (i - 1) / mid;
            For(j, 0, cnt)
                if (d[j] < i - j * mid)
                    { flg = false; break; }
            if (flg) r = mid - 1;
            else l = mid + 1;
        }
        out(l), out.put(' '); */
        f.update(a[i]+1);
        out(ans[0]), out.put(' ');
        /* // int s = 0;
        For(j, 1, n-1) {
            // s = std::max(s, (i - d[j] + j - 1) / j);
            // s = std::max(s, (d[j] + j - 1) / j);
            assert(ans[j] == (d[j] + j - 1) / j);
        }
        // out(s), out.put(' '); */
    }
    out.put('\n');
}
