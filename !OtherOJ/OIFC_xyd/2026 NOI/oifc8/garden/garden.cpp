#include <fstream>
int solve(int l, int x, int y, int m) { // 求 min_(i=0)^(l-1) { (X + Y * i) % m }
    // fprintf(stderr, "%d %d %d %d\n", l, x, y, m);
    x = (x%m+m)%m, y=(y%m+m)%m;
    if (y == 0) return x;
    if (y * 2 > m) { // 因为算法依赖 m --> y --> y - m%y 这样的链条，而注意到这个 y - m%y 不是很友好。我们要 m%y。
        return solve(l, (x + y*(l-1ll)) % m, (m-y%m), m); // 于是平衡一下。把序列反过来，就好了。
    }
    if (x >= y) { // 此时应当跳到末项然后重开 x, x+y, x+2y, ..., x+py, (x+py+y)%m
        int p = (m - x - 1) / y;
        if (p+1 >= l) return x; // 不够跳到末项的
        return std::min(x, solve(l-p-1, (x+(p+1)*y)%m, y, m));
    } else
        return std::min(x, solve((1ll*y*l+m-1)/m, x%y, (y-m%y)%y, y));
}
inline int solvemin(int l, int x, int y, int m) {
    return solve(l, x%m, y%m, m);
}
inline int solvemax(int l, int x, int y, int m) { // max_(i=0)^(l-1){(X+Yi)%m} = m-1 - min_(i=0)^(l-1){(m-1-X-Yi)%m}
    return m - 1 - solve(l, m-1-x%m, (m-y%m)%m, m);
}
int main() {
    std::ifstream fin("garden.in"); std::ofstream fout("garden.out");
    int T, l, x, y, m;
    fin >> T;
    while (T--) {
        fin >> l >> x >> y >> m;
        fout << solvemax(l, x, y, m) << ' ' << solvemin(l, x, y, m) << '\n';
    }
    return 0;
}