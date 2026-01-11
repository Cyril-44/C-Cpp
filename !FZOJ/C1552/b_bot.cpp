#include <bits/stdc++.h>
using namespace std;

// primitive/irreducible polynomials for GF(2^k) (standard choices)
// stored as bitmasks including the x^k term.
// k up to 9 is enough; here n <= 500 so k <= 8 actually.
static int primPoly[10] = {
    0, 0,
    0b111,        // k=2:  x^2 + x + 1
    0b1011,       // k=3:  x^3 + x + 1
    0b10011,      // k=4:  x^4 + x + 1
    0b100101,     // k=5:  x^5 + x^2 + 1
    0b1000011,    // k=6:  x^6 + x + 1
    0b10001001,   // k=7:  x^7 + x^3 + 1
    0x11D,        // k=8:  x^8 + x^4 + x^3 + x^2 + 1
    0x211         // k=9:  x^9 + x^4 + 1
};

// multiply by x in GF(2^k) with modulus primPoly[k]
static inline int mulX(int a, int k) {
    int mask = (1 << k) - 1;
    int msb = 1 << (k - 1);
    int poly = primPoly[k] & mask; // drop x^k term
    int res = (a << 1) & mask;
    if (a & msb) res ^= poly;
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;

        if (n % 2 == 1) {
            // YES: cyclic group Z_n
            cout << "YES\n";
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (j) cout << ' ';
                    cout << (i + j) % n;
                }
                cout << "\n";
            }
            // A = id
            for (int i = 0; i < n; i++) {
                if (i) cout << ' ';
                cout << i;
            }
            cout << "\n";
            // B = id
            for (int i = 0; i < n; i++) {
                if (i) cout << ' ';
                cout << i;
            }
            cout << "\n";
            // C(i)=2i mod n
            for (int i = 0; i < n; i++) {
                if (i) cout << ' ';
                cout << (2LL * i) % n;
            }
            cout << "\n";
        } else if (n % 4 == 2) {
            // NO
            cout << "NO\n";
        } else {
            // YES: n divisible by 4
            // n = 2^k * m, with m odd, k>=2
            int k = 0;
            int tmp = n;
            while ((tmp & 1) == 0) { k++; tmp >>= 1; }
            int m = tmp;                 // odd part
            int pw = 1 << k;             // 2^k
            // build table for group (Z2)^k x Z_m
            cout << "YES\n";
            for (int i = 0; i < n; i++) {
                int ai = (m == 0 ? 0 : i / m);
                int bi = (m == 0 ? 0 : i % m);
                for (int j = 0; j < n; j++) {
                    int aj = (m == 0 ? 0 : j / m);
                    int bj = (m == 0 ? 0 : j % m);
                    int a = ai ^ aj;
                    int b = (m == 1 ? 0 : (bi + bj) % m);
                    int val = a * m + b;
                    if (j) cout << ' ';
                    cout << val;
                }
                cout << "\n";
            }

            // A = id
            for (int i = 0; i < n; i++) {
                if (i) cout << ' ';
                cout << i;
            }
            cout << "\n";

            // B(x) = (mulX(a), b)
            for (int x = 0; x < n; x++) {
                int a = (m == 0 ? 0 : x / m);
                int b = (m == 0 ? 0 : x % m);
                int a2 = mulX(a, k);
                int bx = a2 * m + b;
                if (x) cout << ' ';
                cout << bx;
            }
            cout << "\n";

            // C(x) = x * B(x) = (a xor mulX(a), 2b mod m)
            for (int x = 0; x < n; x++) {
                int a = (m == 0 ? 0 : x / m);
                int b = (m == 0 ? 0 : x % m);
                int a2 = mulX(a, k);
                int a3 = a ^ a2;
                int b3 = (m == 1 ? 0 : (2LL * b) % m);
                int cx = a3 * m + b3;
                if (x) cout << ' ';
                cout << cx;
            }
            cout << "\n";
        }
    }
    return 0;
}
