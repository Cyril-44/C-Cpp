#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <vector>
#include <functional>
#include <algorithm>
#ifdef DEBUG
# define debug(...) fprintf(stderr, __VA_ARGS__)
#else
# define debug(...) ((void)0)
#endif
constexpr int N = 3005;
#define For(i, s, t) for (int i = (s); i <= (t); ++i)
struct Point {
    int x, y;
    inline Point (int _x, int _y) : x(_x), y(_y) {}
    inline bool operator< (const Point &rhs) const {
        return x < rhs.x || x == rhs.x && y < rhs.y;
    }
    inline bool operator> (const Point &rhs) const {
        return x < rhs.x || x == rhs.x && y > rhs.y;
    }
    inline bool operator== (const Point &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};
using LL = long long;
struct Frac {
    int nume, deno;
    explicit inline Frac (LL x, LL y = 1) {
        if (y == 0) {
            deno = 1, nume = x>0?(int)2e9:-(int)2e9;
            return;
        }
        int com = std::__gcd(x, y);
        x /= com, y /= com;
        if (y < 0) x = -x, y = -y;
        nume = x, deno = y;
    }
    inline Frac operator/ (const Frac &rhs) const {
        return Frac(nume * rhs.deno, deno * rhs.nume);
    }
    inline bool operator< (const Frac &rhs) const {
        return (LL)nume * rhs.deno < (LL)deno * rhs.nume;
    }
    inline bool operator> (const Frac &rhs) const { return rhs < *this; }
};
inline double len(const Point &a, const Point &b) {
    return sqrt(LL(a.x - b.x) * (a.x - b.x) + LL(a.y - b.y) * (a.y - b.y));
}
inline double calc(const Point &a, const Point &b, const Point &c) {
    double l1 = len(a, b), l2 = len(b, c), l3 = len(c, a);
    double avg = (l1 + l2 + l3) / 2.;
    return sqrt(avg * (avg - l1) * (avg - l2) * (avg - l3));
}
inline Frac getk(const Point &a, const Point &b) {
    return Frac(a.y - b.y, a.x - b.x);
}
struct Area {
    LL base; int8_t flt;
    inline Area() : base(0), flt(0) {}
    inline Area(Point a, Point b, Point c) : flt(0) {
        if (b < a) std::swap(a, b);
        if (c < a) std::swap(a, c);
        if (c < b) std::swap(b, c); 
        Point mn(std::min({a.x, b.x, c.x}), std::min({a.y, b.y, c.y}));
        Point mx(std::max({a.x, b.x, c.x}), std::max({a.y, b.y, c.y}));
        base = LL(mx.y - mn.y) * (mx.x - mn.x);
        if (a.y == mn.y && c.y == mx.y || a.y == mx.y && c.y == mn.y) {
            debug ("Detected Case 1: ");
            flt += base & 1; base >>= 1; // base /= 2;
            Frac k1 = getk(mn, mx), k2 = getk(mn, b);
            LL tmp = (k2 < k1) ? (b.x - mn.x) : (mx.x - b.x);
            tmp *= mx.y - mn.y;
            flt += tmp & 1; base -= tmp + 1 >> 1; // base -= tmp / 2;
            tmp = (k2 < k1) ? (b.y - mn.y) : (mx.y - b.y);
            tmp *= mx.x - mn.x;
            flt += tmp & 1; base -= tmp + 1 >> 1;
        } else {
            debug ("Detected Case 2: ");
            LL tmp = llabs(a.x - b.x) * llabs(a.y - b.y);
            flt += tmp & 1; base -= tmp + 1 >> 1;
            tmp = llabs(a.x - c.x) * llabs(a.y - c.y);
            flt += tmp & 1; base -= tmp + 1 >> 1;
            tmp = llabs(b.x - c.x) * llabs(b.y - c.y);
            flt += tmp & 1; base -= tmp + 1 >> 1;
        }
        base += flt >> 1;
        flt &= 1;
        debug("(%d,%d)(%d,%d)(%d,%d) Area=%lld.%d, real_calc=%.1lf\n", a.x, a.y, b.x, b.y, c.x, c.y, base, flt?5:0, calc(a, b, c));
    }
    inline bool operator< (const Area &rhs) const {
        return base < rhs.base || base == rhs.base && flt < rhs.flt;
    }
    inline void print() const {
        printf("%lld.", base);
        if (flt) putchar('5');
        else putchar('0');
    }
};
inline void solveSingleTestCase() {
    int n;
    scanf("%d", &n);
    std::vector<Point> a[3];
    For (i, 0, 2) a[i].reserve(n);
    For (i, 1, n) {
        int x, y, c;
        scanf("%d%d%d", &x, &y, &c);
        a[c].emplace_back(x, y);
    }
    if (a[0].size() > a[2].size()) std::swap(a[0], a[2]);
    if (a[1].size() > a[2].size()) std::swap(a[1], a[2]);
    std::vector<std::pair<Frac, Point>> lch, uch;
    // 下凸 k 递增
    std::sort(a[2].begin(), a[2].end());
    lch.reserve(n), lch.emplace_back(-2e9, a[2][0]);
    for (auto ptr = std::next(a[2].begin()); ptr != a[2].end(); ++ptr) {
        if (ptr->x == lch.crbegin()->second.x) continue;
        while (lch.crbegin()->first > getk(lch.crbegin()->second, *ptr)) lch.pop_back();
        lch.emplace_back(getk(lch.crbegin()->second, *ptr), *ptr);
    }
    // 上凸 k 递减
    std::sort(a[2].begin(), a[2].end(), std::greater<Point>());
    uch.reserve(n), uch.emplace_back(2e9, a[2][0]);
    for (auto ptr = std::next(a[2].begin()); ptr != a[2].end(); ++ptr) {
        if (ptr->x == uch.crbegin()->second.x) continue;
        while (uch.crbegin()->first < getk(uch.crbegin()->second, *ptr)) uch.pop_back();
        uch.emplace_back(getk(uch.crbegin()->second, *ptr), *ptr);
    }

    // debug("===Lower Convex Hull===\n");
    // for (const auto& [v1, v2] : lch) debug("(%d, %d) %.1lf\n", v2.x, v2.y, 1. * v1.nume / v1.deno);
    // debug("===Upper Convex Hull===\n");
    // for (const auto& [v1, v2] : uch) debug("(%d, %d) %.1lf\n", v2.x, v2.y, 1. * v1.nume / v1.deno);
    Area ans;
    for (const Point &i : a[0]) {
        for (const Point &j : a[1]) {
            if (i.x == j.x) {
                ans = std::max({ans, Area(i, j, a[2].front()), Area(i, j, a[2].back())});
                continue;
            }
            Frac k = getk(i, j);
            auto lit = std::prev(std::upper_bound(lch.begin(), lch.end(), k, [](const Frac &x, const std::pair<Frac, Point> &y) { return x < y.first; }));
            auto uit = std::prev(std::upper_bound(uch.begin(), uch.end(), k, [](const Frac &x, const std::pair<Frac, Point> &y) { return x > y.first; }));
            debug("(%d,%d)(%d,%d) {(%d,%d),(%d,%d)}\n", i.x, i.y, j.x, j.y, lit->second.x, lit->second.y, uit->second.x, uit->second.y);
            ans = std::max({ans, Area(i, j, lit->second), Area(i, j, uit->second)});
        }
    }
    ans.print(); putchar('\n');
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) solveSingleTestCase();
    return 0;
}