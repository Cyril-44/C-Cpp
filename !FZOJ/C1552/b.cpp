#include <climits>
#include <numeric>
#include <stdio.h>
#include <vector>
#include <tuple>
using Sol = std::tuple<
    std::vector<std::vector<int>>, // 矩阵
    std::vector<int>, // A
    std::vector<int>, // B
    std::vector<int>  // C
>;
inline void printSingleLine(const std::vector<int> &vec) {
    for (int i : vec) printf("%d ", i);
    putchar('\n');
}
inline void printout(const Sol& sol) {
    puts("YES");
    for (const auto &i : std::get<0>(sol)) printSingleLine(i);
    printSingleLine(std::get<1>(sol));
    printSingleLine(std::get<2>(sol));
    printSingleLine(std::get<3>(sol));
}
inline Sol solveOdd(int n) {
    std::vector<std::vector<int>> mat(n, std::vector<int>(n));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            mat[i][j] = (i + j) % n;
    std::vector<int> A(n), B(n), C(n);
    std::iota(A.begin(), A.end(), 0);
    std::iota(B.begin(), B.end(), 0);
    for (int i = 0; i < n; i++) C[i] = (i << 1) % n;
    return {mat, A, B, C};
}

const Sol solDefault(
{{0}},{0},{0},{0}
), sol4(
{{0,1,2,3},
 {1,0,3,2},
 {2,3,0,1},
 {3,2,1,0}},
{0,1,2,3},
{0,2,3,1},
{0,3,1,2}
), sol8{
{{0,1,2,3,4,5,6,7},
 {1,0,7,6,5,4,3,2},
 {2,3,4,5,6,7,0,1},
 {3,2,1,0,7,6,5,4},
 {4,5,6,7,0,1,2,3},
 {5,4,3,2,1,0,7,6},
 {6,7,0,1,2,3,4,5},
 {7,6,5,4,3,2,1,0}},
{0,6,4,2,5,7,1,3},
{0,1,2,3,4,5,6,7},
{0,7,6,5,1,2,3,4}
};

inline std::vector<int> concat1D(const std::vector<int>& fin, const std::vector<int>& abs) {
    int nfin = fin.size(), nabs = abs.size(), n = nfin * nabs;
    std::vector<int> res(n);
    for (int i = 0, addp = 0; i < nabs; ++i, addp += nfin) {
        int addv = abs[i] * nfin;
        for (int j = 0; j < nfin; ++j)
            res[j + addp] = addv + fin[j];
    }
    return res;
}
inline Sol concat(const Sol &sfin, const Sol &sabs) { // replace final Sol into abstract Sol
    const auto matfin = std::get<0>(sfin), matabs = std::get<0>(sabs);
    int nfin = matfin.size(), nabs = matabs.size(), n = nfin * nabs;
    std::vector<std::vector<int>> mat(n, std::vector<int>(n));
    for (int i = 0; i < nabs; i++)
        for (int j = 0; j < nabs; j++) {
            int add = matabs[i][j] * nfin, addi = i * nfin, addj = j * nfin;
            for (int ii = 0; ii < nfin; ++ii)
                for (int jj = 0; jj < nfin; ++jj)
                    mat[addi + ii][addj + jj] = add + matfin[ii][jj];
        }
    return {mat, 
        concat1D(std::get<1>(sfin), std::get<1>(sabs)), 
        concat1D(std::get<2>(sfin), std::get<2>(sabs)), 
        concat1D(std::get<3>(sfin), std::get<3>(sabs))};
}
inline Sol qpow(Sol b, int n) {
    Sol res(solDefault);
    while (n) {
        fprintf(stderr, "%d\n", n);
        if (n & 1) res = concat(b, res);
        b = concat(b, b); n >>= 1;
    }
    return res;
}
int main() {
    int T, n;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        if (n & 1) {
            printout(solveOdd(n));
        } else if (!(n & 3)) {
            int cnt = __builtin_ctz(n); // n = 2^cnt * (odd)
            Sol ans(solDefault), base(solveOdd(n >> cnt));
            for (; cnt > 3; cnt -= 2) ans = concat(ans, sol4);
            if (cnt == 3) ans = concat(ans, sol8);
            else ans = concat(ans, sol4);
            printout(concat(ans, base));
        } else puts("NO");
    }
    return 0;
}