#include <bits/stdc++.h>
using namespace std;
using Point = pair<int, int>;

long long maximumK(long long n) {
    if (n & 1) return (n * n - 1) / 2;
    if (n % 4 == 2) return (n * n + n - 2) / 2;
    if (n == 4 || n == 8) return (n * n + n - 4) / 2;
    return (n * n + n - 6) / 2;
}

vector<Point> fourVertical(int n) {
    int m = n / 2;
    vector<int> a(m), b(m), d(m);
    for (int j = 0; j < m; ++j) {
        a[j] = min(j + 1, m - 2);
        b[j] = max(n - 2 - j, m + 1);
        d[j] = ((j & 1) || j >= m - 3) ? 1 : -1;
    }
    vector<Point> p;
    p.reserve(n * n);
    auto add = [&](int r, int c) { p.emplace_back(r, c); };
    auto stripe = [&](int j, int lo, int hi, bool down) {
        for (int i = 0; i <= hi - lo; ++i) {
            int r = down ? lo + i : hi - i;
            add(r, 2 * j);
            add(r, 2 * j + 1);
        }
    };

    // Top bands, from left to right.
    for (int j = 0; j < m; ++j)
        stripe(j, 0, a[j] - 1 - (j > 0 && d[j] < 0), j & 1);

    // Upper channel, from right to left.
    for (int j = m - 1; j >= 0; --j) {
        add(a[j], 2 * j + 1);
        if (j > 0) {
            add(a[j] + d[j], 2 * j);
            add(a[j] + d[j], 2 * j + 1);
        }
        add(a[j], 2 * j);
    }

    // Middle bands, from left to right.
    for (int j = 0; j < m; ++j)
        stripe(j, a[j] + 1 + (j > 0 && d[j] > 0),
               b[j] - 1 - (d[j] < 0), !(j & 1));

    // Lower channel, from right to left.
    for (int j = m - 1; j >= 0; --j) {
        add(b[j], 2 * j + 1);
        add(b[j] + d[j], 2 * j);
        add(b[j] + d[j], 2 * j + 1);
        add(b[j], 2 * j);
    }

    // Bottom bands, from left to right.
    for (int j = 0; j < m; ++j)
        stripe(j, b[j] + 1 + (d[j] > 0), n - 1, j & 1);

    return p;
}

string maximumPath(int n) {
    if (n == 4) return "DDDZDXAQAXDZDDD";
    if (n == 8)
        return "DDDZDZDZQQDZXDZDZDZDZDDDQDQDQDDQDDQDQDQDDDZDZDZDZDXZDQQZDZDZDDD";

    vector<Point> p;
    p.reserve(n * n);
    if (n & 1) {
        for (int r = 0; r < n; ++r)
            for (int i = 0; i < n; ++i)
                p.emplace_back(r, (r & 1) ? n - 1 - i : i);
    } else if (n % 4 == 2) {
        for (int j = 0; j < n / 2; ++j) {
            for (int i = 0; i < n; ++i) {
                int r = (j & 1) ? n - 1 - i : i;
                p.emplace_back(r, 2 * j);
                p.emplace_back(r, 2 * j + 1);
            }
        }
    } else {
        p = fourVertical(n);
    }
    const string names[] = {"QWE", "A?D", "ZXC"};
    string answer;
    answer.reserve(n * n - 1);
    for (int i = 1; i < (int)p.size(); ++i) {
        int dr = p[i].first - p[i - 1].first;
        int dc = p[i].second - p[i - 1].second;
        answer += names[dr + 1][dc + 1];
    }
    return answer;
}

// Input: T, followed by T values of N (there is no input K).
// Output for each case: maximum K, then a path attaining it.
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        cout << maximumK(n) << '\n' << maximumPath(n) << '\n';
    }
    return 0;
}
