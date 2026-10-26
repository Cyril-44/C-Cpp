#include <bits/stdc++.h>
using namespace std;
using Point = pair<int, int>;

long long maximumK(long long n) {
    if (n & 1) return (n * n - 1) / 2;
    if (n % 4 == 2) return (n * n + n - 2) / 2;
    if (n == 4 || n == 8) return (n * n + n - 4) / 2;
    return (n * n + n - 6) / 2;
}

struct Solver {
    int n;
    vector<Point> p;

    // 4 x 2, top-left -> bottom-right; index = number of D.
    const vector<string> block42 = {
        "XEXZXEX", "DXAXXEX", "DZDZXEX", "DZDXAXD", "DZDZDZD"
    };
    // 4 x 4, top-left -> bottom-right; index = number of D.
    const vector<string> block44 = {
        "XEXEXEXZZQXECEX", "DZXEEXEXZZAECEX", "DDZAXXEEEXXZAEC",
        "DDDZAAXXECAEEXX", "DDDZAAXXEXEEXZD", "DDDZAAXCAEDEXZD",
        "DDDZAAXDZDEEXZD", "DDDZDZQAXDZDDEX", "DDDZDXAQAXDZDDD"
    };

    explicit Solver(int n) : n(n) { p.reserve(n * n); }
    void add(int r, int c) { p.emplace_back(r, c); }

    // Append every cell of a block, including its first cell.
    // vertical=-1 reflects the block vertically and preserves the D count.
    void pattern(int r, int c, const string& s, int vertical = 1) {
        static const string name = "QWEDCXZA";
        static const int dr[] = {-1, -1, -1, 0, 1, 1, 1, 0};
        static const int dc[] = {-1, 0, 1, 1, 1, 0, -1, -1};
        add(r, c);
        for (char ch : s) {
            int k = (int)name.find(ch);
            r += vertical * dr[k];
            c += dc[k];
            add(r, c);
        }
    }

    // 2 x n, top-left -> bottom-left, with any t in [0,n-1].
    void twoRows(int r, int t) {
        vector<int> row(n, 0);
        for (int c = 1; c < n; ++c)
            row[c] = row[c - 1] ^ (c > t);
        for (int c = 0; c < n; ++c) add(r + row[c], c);
        for (int c = n - 1; c >= 0; --c) add(r + 1 - row[c], c);
    }

    // h x 2, h even and h>=4; any t in [0,h].
    // Downward: top-left -> bottom-right. Upward: vertical reflection.
    void evenColumns(int top, int col, int h, int t, bool upward) {
        int first = min(t, 4);
        if ((first & 1) != (t & 1)) --first;
        int vertical = upward ? -1 : 1;
        auto at = [&](int r) { return top + (upward ? h - 1 - r : r); };
        pattern(at(0), col, block42[first], vertical);
        t -= first;
        for (int r = 4; r < h; r += 2) {
            int take = min(t, 2);
            pattern(at(r), col, take == 2 ? "DZD" : "XEX", vertical);
            t -= take;
        }
    }

    // Odd n: 0..n-1. Even n: 0..n-2.
    void smallK(int k) {
        int columns = (n & 1) ? n : n - 2;
        for (int c = 0; c < columns; ++c)
            for (int i = 0; i < n; ++i)
                add((c & 1) ? n - 1 - i : i, c);
        if (!(n & 1))
            for (int r = 0; r < n; r += 2)
                pattern(r, n - 2, "XEX");

        int remove = ((n & 1) ? n - 1 : n - 2) - k;
        for (int i = 0; i + 1 < (int)p.size() && remove; ++i) {
            if (p[i].first == p[i + 1].first &&
                p[i + 1].second == p[i].second + 1) {
                swap(p[i], p[i + 1]);
                --remove;
                ++i;
            }
        }
    }

    // n divisible by 4, n-2 < k <= n*n/2.
    void ordinaryMultiple4(int k) {
        int strips = (n - 4) / 2;
        int remaining = k - strips; // Fixed D connections in the bottom bar.
        for (int r = 0; r < n - 4; r += 2) {
            int take = min(remaining, n - 1);
            twoRows(r, take);
            remaining -= take;
        }
        for (int j = 0; j < strips; ++j) {
            int take = min(remaining, 4);
            evenColumns(n - 4, 2 * j, 4, take, j & 1);
            remaining -= take;
        }
        pattern(n - 4, n - 4, block44[remaining]);
    }

    // The five-stage maximum construction, minus delta right moves.
    // n>=12; only the first middle strip is changed.
    void fiveStages(int delta) {
        int m = n / 2;
        vector<int> a(m), b(m), d(m);
        for (int j = 0; j < m; ++j) {
            a[j] = min(j + 1, m - 2);
            b[j] = max(n - 2 - j, m + 1);
            d[j] = ((j & 1) || j >= m - 3) ? 1 : -1;
        }
        auto stripe = [&](int j, int lo, int hi, bool down) {
            for (int i = 0; i <= hi - lo; ++i) {
                int r = down ? lo + i : hi - i;
                add(r, 2 * j);
                add(r, 2 * j + 1);
            }
        };
        for (int j = 0; j < m; ++j)
            stripe(j, 0, a[j] - 1 - (j > 0 && d[j] < 0), j & 1);
        for (int j = m - 1; j >= 0; --j) {
            add(a[j], 2 * j + 1);
            if (j > 0) {
                add(a[j] + d[j], 2 * j);
                add(a[j] + d[j], 2 * j + 1);
            }
            add(a[j], 2 * j);
        }
        for (int j = 0; j < m; ++j) {
            int lo = a[j] + 1 + (j > 0 && d[j] > 0);
            int hi = b[j] - 1 - (d[j] < 0);
            if (j == 0) {
                int h = hi - lo; // Keep the final row unchanged.
                evenColumns(lo, 0, h, h - delta, false);
                add(hi, 0);
                add(hi, 1);
            } else {
                stripe(j, lo, hi, !(j & 1));
            }
        }
        for (int j = m - 1; j >= 0; --j) {
            add(b[j], 2 * j + 1);
            add(b[j] + d[j], 2 * j);
            add(b[j] + d[j], 2 * j + 1);
            add(b[j], 2 * j);
        }
        for (int j = 0; j < m; ++j)
            stripe(j, b[j] + 1 + (d[j] > 0), n - 1, j & 1);
    }

    void build(int k) {
        if (n == 2) {
            pattern(0, 0, k == 0 ? "XEX" : "DZD");
        } else if (k <= n - 2 + (n & 1)) {
            smallK(k);
        } else if (n & 1) {
            int remaining = k - (n - 1);
            for (int r = 0; r < n - 1; r += 2) {
                int take = min(remaining, n - 1);
                twoRows(r, take);
                remaining -= take;
            }
            for (int c = 0; c < n; ++c) add(n - 1, c);
        } else if (n % 4 == 2) {
            int remaining = k - (n / 2 - 1);
            for (int j = 0; j < n / 2; ++j) {
                int take = min(remaining, n);
                evenColumns(0, 2 * j, n, take, j & 1);
                remaining -= take;
            }
        } else if (k <= n * n / 2) {
            ordinaryMultiple4(k);
        } else if (n == 8) {
            pattern(0, 0, k == 33
                ? "DDDZDDDQDDDZDXZDQAZDQAZDQQAXDZDZDDDDDDDXZDQAZDQAZDQZQDZXDDDDDDD"
                : "DDDZDZDZQQDZXDZDZDZDZDDDQDQDQDDQDDQDQDQDDDZDZDZDZDXZDQQZDZDZDDD");
        } else {
            fiveStages((int)maximumK(n) - k);
        }
    }

    string answer() const {
        static const string name[] = {"QWE", "A?D", "ZXC"};
        string result;
        result.reserve(n * n - 1);
        for (int i = 1; i < (int)p.size(); ++i) {
            int dr = p[i].first - p[i - 1].first;
            int dc = p[i].second - p[i - 1].second;
            result += name[dr + 1][dc + 1];
        }
        return result;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n;
        long long k;
        cin >> n >> k;
        if (k < 0 || k > maximumK(n) || (n == 2 && k == 1)) {
            cout << "No\n";
            continue;
        }
        Solver solver(n);
        solver.build((int)k);
        cout << "Yes\n" << solver.answer() << '\n';
    }
    return 0;
}
