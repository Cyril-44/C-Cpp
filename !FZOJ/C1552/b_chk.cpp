#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

static void checkPermutation(const vector<int>& p, int n, const char* name, int tc) {
    vector<int> cnt(n, 0);
    for (int i = 0; i < n; i++) {
        if (p[i] < 0 || p[i] >= n)
            quitf(_wa, "Test %d: %s[%d]=%d out of range [0,%d)", tc, name, i, p[i], n);
        cnt[p[i]]++;
    }
    for (int v = 0; v < n; v++) {
        if (cnt[v] != 1)
            quitf(_wa, "Test %d: %s is not a permutation: value %d appears %d times", tc, name, v, cnt[v]);
    }
}

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);

    int T = inf.readInt(1, 1000, "T");

    for (int tc = 1; tc <= T; tc++) {
        int n = inf.readInt(2, 500, "n");

        string tag = ouf.readToken();
        if (tag != "YES" && tag != "NO") {
            quitf(_wa, "Test %d: first token must be YES or NO, got '%s'", tc, tag.c_str());
        }

        // 判无解：只有 n % 4 == 2 才允许 NO
        if (tag == "NO") {
            if (n % 4 == 2) continue;
            quitf(_wa, "Test %d: output NO but a construction should exist for n=%d", tc, n);
        }

        // tag == "YES": read table and permutations
        vector<int> op(n * n);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                op[i * n + j] = ouf.readInt(0, n - 1, "op[i][j]");
            }
        }

        vector<int> A(n), B(n), C(n);
        for (int i = 0; i < n; i++) A[i] = ouf.readInt(0, n - 1, "A[i]");
        for (int i = 0; i < n; i++) B[i] = ouf.readInt(0, n - 1, "B[i]");
        for (int i = 0; i < n; i++) C[i] = ouf.readInt(0, n - 1, "C[i]");

        checkPermutation(A, n, "A", tc);
        checkPermutation(B, n, "B", tc);
        checkPermutation(C, n, "C", tc);

        auto mul = [&](int x, int y) -> int {
            return op[x * n + y];
        };

        // check A(x) * B(x) = C(x)
        for (int x = 0; x < n; x++) {
            int lhs = mul(A[x], B[x]);
            if (lhs != C[x]) {
                quitf(_wa, "Test %d: equation fails at x=%d: A[x]*B[x]=%d, C[x]=%d",
                      tc, x, lhs, C[x]);
            }
        }

        // find identity element e
        int e = -1;
        for (int cand = 0; cand < n; cand++) {
            bool ok = true;
            for (int x = 0; x < n; x++) {
                if (mul(cand, x) != x || mul(x, cand) != x) {
                    ok = false;
                    break;
                }
            }
            if (ok) { e = cand; break; }
        }
        if (e == -1) {
            quitf(_wa, "Test %d: no identity element found", tc);
        }

        // check inverses
        for (int a = 0; a < n; a++) {
            int inv = -1;
            for (int b = 0; b < n; b++) {
                if (mul(a, b) == e && mul(b, a) == e) {
                    inv = b;
                    break;
                }
            }
            if (inv == -1) {
                quitf(_wa, "Test %d: element %d has no two-sided inverse", tc, a);
            }
        }

        // check associativity: (a*b)*c == a*(b*c)
        // O(n^3), optimized with pointers
        for (int a = 0; a < n; a++) {
            const int* rowA = &op[a * n];
            for (int b = 0; b < n; b++) {
                int ab = rowA[b];
                const int* rowAB = &op[ab * n];
                const int* rowB  = &op[b * n];
                for (int c = 0; c < n; c++) {
                    int left = rowAB[c];
                    int bc = rowB[c];
                    int right = rowA[bc];
                    if (left != right) {
                        quitf(_wa,
                              "Test %d: associativity fails at (a,b,c)=(%d,%d,%d): "
                              "(a*b)*c=%d, a*(b*c)=%d",
                              tc, a, b, c, left, right);
                    }
                }
            }
        }
    }

    ouf.seekEof();
    quitf(_ok, "All tests passed.");
}
