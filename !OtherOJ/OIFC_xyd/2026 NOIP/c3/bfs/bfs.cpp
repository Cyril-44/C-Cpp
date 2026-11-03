#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cerrno>
#include <type_traits>
#ifdef _WIN32
#include <io.h>
#include <fcntl.h>
#else
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#endif

namespace detail {
    constexpr std::size_t CAP = 31000000;
    struct InputHelper { alignas(64) std::uint8_t table[65536]; constexpr InputHelper() : table{} { for (unsigned a = 0; a < 10; ++a) for (unsigned b = 0; b < 10; ++b) table[(a + 48) | ((b + 48) << 8)] = a * 10 + b + 1; } };
    static constexpr InputHelper I{};
    alignas(64) static char buffer[CAP + 64];
}

class Scanner {
    const char *i = detail::buffer, *end = detail::buffer, *boundary = nullptr;
#ifndef _WIN32
    const char *mapped = nullptr; std::size_t mappedSize = 0;
#endif
    inline void skip() {
        while (i < end && (unsigned char)*i <= 32) ++i;
#ifndef _WIN32
        if (__builtin_expect(boundary && i >= boundary, 0)) { std::size_t n = end - i; std::memcpy(detail::buffer, i, n); std::memset(detail::buffer + n, 0, 32); i = detail::buffer; end = i + n; boundary = nullptr; }
#endif
    }
    template <class T> inline void s2(T& v) { std::uint16_t k; std::memcpy(&k, i, 2); unsigned x = detail::I.table[k]; if (x) v = T(v * 100 + x - 1), i += 2; }
    template <class T> inline void s4(T& v) { s2(v); s2(v); }
    static inline bool digit(char c) { return (unsigned)(c - '0') < 10; }
public:
    explicit Scanner(int fd = 0) {
#ifndef _WIN32
        struct stat st{};
        if (!fstat(fd, &st) && S_ISREG(st.st_mode) && st.st_size > 0) {
            void* p = mmap(nullptr, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (p != MAP_FAILED) { mapped = i = (const char*)p; mappedSize = st.st_size; end = i + mappedSize; boundary = i + (mappedSize > 32 ? mappedSize - 32 : 0); skip(); return; }
        }
#else
        _setmode(fd, _O_BINARY);
#endif
        std::size_t n = 0;
        while (n < detail::CAP) {
#ifdef _WIN32
            int r = _read(fd, detail::buffer + n, (unsigned)(detail::CAP - n));
#else
            ssize_t r = read(fd, detail::buffer + n, detail::CAP - n);
#endif
            if (r > 0) n += r; else if (!r) break; else if (errno != EINTR) break;
        }
        end = i + n; std::memset(detail::buffer + n, 0, 32); skip();
    }
#ifndef _WIN32
    ~Scanner() { if (mapped) munmap((void*)mapped, mappedSize); }
#endif
    Scanner(const Scanner&) = delete;
    Scanner& operator=(const Scanner&) = delete;
template <class T> inline void scan(T& v) {
    static_assert(std::is_integral<T>::value && (sizeof(T) == 4 || sizeof(T) == 8), "Unsupported integer type");
    using U = typename std::make_unsigned<T>::type;
    bool neg = std::is_signed<T>::value && *i == '-'; i += neg;
    U x = 0;
    if (sizeof(T) == 4) { s4(x); s4(x); s2(x); }
    else { s4(x); s4(x); s4(x); s4(x); s4(x); }
    while (i < end && digit(*i)) x = x * 10 + (*i++ - '0');
    skip();
    if (neg) x = U(0) - x;
    std::memcpy(&v, &x, sizeof(v));
}
    template <class T> inline Scanner& operator>>(T& v) { return scan(v), *this; }
};

#include <bits/stdc++.h>
constexpr int N = 1000005;
#ifdef CLANGD
#define N 15
#endif
std::vector<int> g[N];
char outputbuf[N * 2];
int p[N], ssz[N], fa[N], lv[N], lsum[N], que[N];
constexpr uint64_t B = (int)1e9 + 7;
uint64_t base[N];
std::mt19937 rng(std::random_device{}());
struct DelPQ {
    std::priority_queue<int, std::vector<int>, std::greater<int>> lq, ldq;
    std::priority_queue<int> gq, gdq;
    template<class T> int top(T& q, T& dq) {
        if (q.empty()) return -1;
        int res = q.top();
        while (!dq.empty() && res == dq.top()) {
            q.pop(), dq.pop();
            if (q.empty()) return -1;
            res = q.top();
        }
        return res;
    }
    int front() { return top(lq, ldq); }
    int back() { return top(gq, gdq); }
    void push(int x) { lq.push(x), gq.push(x); }
    void erase(int x) { ldq.push(x), gdq.push(x); }
} pq[N];
struct Treap {
    struct Node {
        uint64_t hash;
        int bh, bv, vl, vr, sz, ls, rs;
        unsigned w;
    } tr[N*2];
    int bin[N*2], rt[2][N], top;
    int alloc(int hsh, int rnk) {
        int u = bin[0] ? bin[bin[0]--] : ++top;
        tr[u] = {(uint64_t)hsh, hsh, rnk, rnk, rnk, 1, 0, 0, (unsigned)rng()};
        return u;
    }
    void pushup(int u) {
        tr[u].sz = tr[tr[u].ls].sz + 1 + tr[tr[u].rs].sz;
        tr[u].vl = tr[u].ls ? tr[tr[u].ls].vl : tr[u].bv;
        tr[u].vr = tr[u].rs ? tr[tr[u].rs].vr : tr[u].bv;
        tr[u].hash = tr[tr[u].ls].hash * base[tr[tr[u].rs].sz + 1] + tr[u].bh * base[tr[tr[u].rs].sz] + tr[tr[u].rs].hash;
    }
    int merge(int u, int v) {
        if (!u || !v) return u | v;
        if (tr[u].w < tr[v].w) return tr[u].rs = merge(tr[u].rs, v), pushup(u), u;
        return tr[v].ls = merge(u, tr[v].ls), pushup(v), v;
    }
    std::pair<int,int> split(int u, int sz) {
        if (sz == 0) return {0, u};
        if (!u) assert(0);
        if (sz == tr[u].sz) return {u, 0};
        if (sz <= tr[tr[u].ls].sz) {
            auto [l1, l2] = split(tr[u].ls, sz);
            tr[u].ls = l2, pushup(u);
            return {l1, u};
        }
        auto [r1, r2] = split(tr[u].rs, sz - tr[tr[u].ls].sz - 1);
        tr[u].rs = r1, pushup(u);
        return {u, r2};
    }
    std::pair<int,int> lower_bound(int u, int val) { // [1, val) [val, n]
        if (!u) return {0, 0};
        if (val <= tr[u].vl) return {0, u};
        if (val > tr[u].vr) return {u, 0};
        if (val <= tr[u].bv) {
            auto [l1, l2] = lower_bound(tr[u].ls, val);
            tr[u].ls = l2, pushup(u);
            return {l1, u};
        }
        auto [r1, r2] = lower_bound(tr[u].rs, val);
        tr[u].rs = r1, pushup(u);
        return {u, r2};
    }
    void recycle(int u) { bin[++bin[0]] = u; }
    bool equal(int i) { return tr[rt[0][i]].hash == tr[rt[1][i]].hash; }
    int insert(int f, int q, int x) {
        if (!ssz[q]) return 0;
        const int pre = !equal(lv[q]);
        int &u = rt[f][lv[q]];
        auto [l, r] = lower_bound(u, x);
        // fprintf(stderr, "Insert (%d,%d) into %d[%d][%d]: ", q, x, u, (int)f, lv[q]);
        // audit(l); fprintf(stderr, "| "); audit(r); fprintf(stderr, "\n");
        u = merge(merge(l, alloc(q, x)), r);
        const int now = !equal(lv[q]);
        return now - pre;
    }
    int erase(int f, int q, int x) {
        if (!ssz[q]) return 0;
        const int pre = !equal(lv[q]);
        int &u = rt[f][lv[q]];
        auto [l, tmp] = lower_bound(u, x);
        auto [v, r] = lower_bound(tmp, x+1);
        // fprintf(stderr, "Erase (%d,%d) from %d[%d][%d]: ", q, x, u, (int)f, lv[q]);
        // audit(l); fprintf(stderr, "| "); audit(v); fprintf(stderr, "| "); audit(r); fprintf(stderr, "\n");
        recycle(v);
        u = merge(l, r);
        const int now = !equal(lv[q]);
        return now - pre;
    }
    void audit(int u) {
        if (!u) return;
        audit(tr[u].ls);
        fprintf(stderr, "(%d,%d [%d,%d]) ", tr[u].bh, tr[u].bv, tr[u].vl, tr[u].vr);
        audit(tr[u].rs);
    }
} ftr;
int main() {
    freopen("bfs.in", "r", stdin);
    freopen("bfs.out", "w", stdout);
    static Scanner cin;
    int n, q;
    cin >> n >> q;
    base[0] = 1;
    for (int i = 1; i <= n; i++) base[i] = base[i-1] * B;
    for (int i = 1; i <= n; i++) cin >> p[i];
    for (int i = 1, u, v; i < n; i++) {
        cin >> u >> v;
        g[u].push_back(v), g[v].push_back(u);
    }
    for (int i = 1; i <= n; i++) ssz[i] = (int)g[i].size() - (i != 1);
    for (int hd=0, tl=que[0]=lv[1]=1; hd < tl; hd++) {
        int u = que[hd]; ++lsum[lv[u]];
        for (int v : g[u]) if (!lv[v])
            fa[v] = u, lv[v] = lv[u] + 1, que[tl++] = v;
    }
    for (int i = 1; i <= n; i++) lsum[i] += lsum[i-1];

    int rangeErr = 0, hashErr = 0, contErr = 0;
    auto inRange = [] (int i) { return lsum[lv[p[i]] - 1] < i && i <= lsum[lv[p[i]]]; };
    for (int i = 1; i <= n; i++) {
        pq[fa[p[i]]].push(i);
        rangeErr += !inRange(i);
    }
    auto notContinuous = [&](int v) { return ssz[v] && pq[v].back() - pq[v].front() + 1 != ssz[v]; };
    for (int i = 1; i <= n; i++) {
        contErr += notContinuous(p[i]);
        hashErr += ftr.insert(0, p[i], i);
        hashErr += ftr.insert(1, p[i], pq[p[i]].front());
    }
    /* for (int i = 1; i <= n; i++) if (ftr.rt[0][i]) {
        fprintf(stderr, "Level%3d: ", i); ftr.audit(ftr.rt[0][i]); fprintf(stderr, "\n");
        fprintf(stderr, "          ");    ftr.audit(ftr.rt[1][i]); fprintf(stderr, "\n");
    } */
    for (int x, y, i = 0; i < q; i++) {
        // fprintf(stderr, "=========================\n");
        cin >> x >> y;
        if (x != y) {
            const int fax = fa[p[x]], fay = fa[p[y]], 
                      pretopx = pq[fax].front(), pretopy = pq[fay].front();
            if (fax) contErr -= notContinuous(fax), pq[fax].erase(x), pq[fax].push(y), contErr += notContinuous(fax);
            if (fay) contErr -= notContinuous(fay), pq[fay].erase(y), pq[fay].push(x), contErr += notContinuous(fay);
            hashErr += ftr.erase(0, p[x], x);
            hashErr += ftr.erase(0, p[y], y);
            rangeErr -= !inRange(x);
            rangeErr -= !inRange(y);
            std::swap(p[x], p[y]);
            rangeErr += !inRange(x);
            rangeErr += !inRange(y);
            hashErr += ftr.insert(0, p[x], x);
            hashErr += ftr.insert(0, p[y], y);
            const int nowtopx = pq[fax].front(), nowtopy = pq[fay].front();
            if (fax && nowtopx != pretopx) hashErr += ftr.erase(1, fax, pretopx);
            if (fay && fax != fay && nowtopy != pretopy) hashErr += ftr.erase(1, fay, pretopy);
            if (fax && nowtopx != pretopx) hashErr += ftr.insert(1, fax, nowtopx);
            if (fay && fax != fay && nowtopy != pretopy) hashErr += ftr.insert(1, fay, nowtopy);
        }
        outputbuf[i<<1] = (!rangeErr && !hashErr && !contErr) | '0';
        outputbuf[i<<1|1] = '\n';
        // fprintf(stderr, "%d %d %d\n", rangeErr, hashErr, contErr);
        // for (int i = 1; i <= n; i++) if (ftr.rt[0][i]) {
        //     fprintf(stderr, "Level%3d: ", i); ftr.audit(ftr.rt[0][i]); fprintf(stderr, "\n");
        //     fprintf(stderr, "          ");    ftr.audit(ftr.rt[1][i]); fprintf(stderr, "\n");
        // }
    }
    fwrite(outputbuf, q, 2, stdout);
}