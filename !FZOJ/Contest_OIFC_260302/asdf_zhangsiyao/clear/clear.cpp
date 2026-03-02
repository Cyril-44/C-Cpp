#include <bits/extc++.h>
using namespace __gnu_pbds;
template<class T> using Set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;
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
int main() {
    freopen("clear.in", "r", stdin);
    freopen("clear.out", "w", stdout);
    std::map<int, Set<int>> all;
    int Tid, n, q;
    in(Tid), in(n), in(q);
    all.insert({0, {}});
    for (int i = 1, ai; i <= n; i++) {
        in(ai); Set<int> st; st.insert(ai);
        all.insert(all.end(), {i, st});
    }
    auto del = [](Set<int> &st, int pos) {
        --pos; // 0 index
        if (pos < (int)st.size() - 1) // Keep one at least
            st.erase(st.find_by_order(pos));
    };
    auto oper = [&](int l, int r) {
        auto it = all.lower_bound(l);
        del(it->second, l - std::prev(it)->first);
        for (++it; it != all.end() && it->first <= r; ++it) {
            del(it->second, 1);
            if (std::prev(it)->first != 0 && *std::prev(it)->second.rbegin() < *it->second.begin()) {
                it->second.join(std::prev(it)->second);
                all.erase(std::prev(it));
            }
        }
        if (it != all.end()) {
            del(it->second, r - std::prev(it)->first);
            if (std::prev(it)->first != 0 && *std::prev(it)->second.rbegin() < *it->second.begin()) {
                it->second.join(std::prev(it)->second);
                all.erase(std::prev(it));
            }
        }
    };
    oper(1, n);
    for (int op, l, r; q--; ) {
        in(op), in(l), in(r);
        if (op == 1) oper(l, r);
        else {
            auto it = all.lower_bound(l);
            int ans = std::min(it->first, r) - l + 1, last = *it->second.rbegin();
            for (++it; it != all.end() && it->first <= r; ++it) {
                ans += it->first - std::prev(it)->first - it->second.order_of_key(last);
                last = *it->second.rbegin();
            }
            if (it != all.end()) {
                int add = r - std::prev(it)->first - it->second.order_of_key(last);
                if (add > 0) ans += add;
            }
            printf("%d\n", ans);
        }
    }
    return 0;
}
