#include <cstdio>
#include <cmath>
#include <cassert>
#include <numeric>
#include <vector>
constexpr int N = 405;
int n;

std::vector<int> operator+(std::vector<int> a, const std::vector<int> &b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}
std::vector<int> rangev(int l, int r) {
    std::vector<int> vec(r-l+1);
    std::iota(vec.begin(), vec.end(), l);
    return vec;
}
inline void pr(const std::vector<int>& vec) {
    for (int i = 0, v = 1; v <= n; v++) {
        if (i < vec.size() && vec[i] == v)
            ++i, putchar('1');
        else putchar('0');
        putchar(' ');
    }
    putchar('\n');
}
int main() {
    scanf("%d", &n);
    printf("%d\n", n);
    int p = sqrt(n);
    for (int i = 2; i <= n; i += p)
        pr(rangev(1,1) + rangev(i, i+p-1));
    for (int v = 2; v <= p+1; v++) {
        std::vector<int> vec(p+1);
        vec[0] = v;
        for (int i = 1; i <= p; i++)
            vec[i] = p*i+2 + (v-2) * (i-1) % p;
        for (int _=p; _--; ) {
            for (int i = 1; i <= p; i++)
                if (++vec[i] > p*(i+1)+1) vec[i] = p*i+2;
            pr(vec);
        }
    }
    return 0;
}
