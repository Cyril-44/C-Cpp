#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    vector<int> a(n + 1), pos(n + 1);
    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pos[a[i]] = i;
    }

    int m = (n + 63) >> 6;
    vector<ull> low(m, 0), revHigh(m, ~0ULL);

    // mask unused bits in the last  ord
    if (n % 64) {
        ull mask = (1ULL << (n % 64)) - 1ULL;
        revHigh[m - 1] &= mask;
    }

    auto clear_bit = [&](vector<ull>& bs, int idx0) {
        bs[idx0 >> 6] &= ~(1ULL << (idx0 & 63));
    };
    auto set_bit = [&](vector<ull>& bs, int idx0) {
        bs[idx0 >> 6] |= (1ULL << (idx0 & 63));
    };

    // Return word w of (revHigh shifted by 'shift' bits):
    // shift > 0 means right shift, shift < 0 means left shift
    auto get_shifted_word = [&](int w, int shift) -> ull {
        if (shift == 0) return revHigh[w];

        if (shift > 0) {
            int ws = shift >> 6;
            int bs = shift & 63;
            int src = w + ws;
            if (src >= m) return 0ULL;
            ull res = revHigh[src] >> bs;
            if (bs && src + 1 < m) res |= (revHigh[src + 1] << (64 - bs));
            return res;
        } else {
            int s = -shift;
            int ws = s >> 6;
            int bs = s & 63;
            int src = w - ws;
            if (src < 0) return 0ULL;
            ull res = revHigh[src] << bs;
            if (bs && src - 1 >= 0) res |= (revHigh[src - 1] >> (64 - bs));
            return res;
        }
    };

    long long ans = 0;

    for (int v = 1; v <= n; v++) {
        int p = pos[v] - 1;           // 0-based center position

        // remove current value from High: need strictly > v
        int rp = (n - 1) - p;         // reversed index
        clear_bit(revHigh, rp);

        int shift = (n - 1) - 2 * p;  // delta

        // only l < p
        if (p > 0) {
            int lastWord = (p - 1) >> 6;
            for (int w = 0; w <= lastWord; w++) {
                ull x = low[w] & get_shifted_word(w, shift);

                if (w == lastWord) {
                    // number of valid bits in this last word = p - w*64  (range 1..64)
                    int bits = p - (w << 6);
                    ull mask = (bits == 64) ? ~0ULL : ((1ULL << bits) - 1ULL);
                    x &= mask;
                }

                ans += __builtin_popcountll(x);
            }
        }

        // add current position to Low for future centers
        set_bit(low, p);
    }

    cout << ans << "\n";
    return 0;
}
