 
#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;
using int64 = long long;

struct Item {
    int64 p, q;  // ratio p / q, q > 0
    int64 w;     // weight
};

struct Cost {
    i128 num;    // cost = num / den
    int64 den;
};

string toString(i128 x) {
    if (x == 0) return "0";

    string s;
    while (x > 0) {
        s.push_back(char('0' + x % 10));
        x /= 10;
    }
    reverse(s.begin(), s.end());
    return s;
}

// Minimize sum |Y[i] - t * X[i]| over -1 <= t <= 1.
Cost solve(const vector<int64>& X, const vector<int64>& Y) {
    vector<Item> items;
    int64 totalWeight = 0;

    for (int i = 0; i < (int)X.size(); ++i) {
        if (X[i] == 0) continue;

        int64 w = llabs(X[i]);
        int64 p = (X[i] < 0 ? -Y[i] : Y[i]);
        int64 q = w;

        int64 g = gcd(llabs(p), q);
        p /= g;
        q /= g;

        items.push_back({p, q, w});
        totalWeight += w;
    }

    int64 p = 0, q = 1;

    if (!items.empty()) {
        sort(items.begin(), items.end(), [](const Item& a, const Item& b) {
            return (i128)a.p * b.q < (i128)b.p * a.q;
        });

        int64 prefixWeight = 0;
        for (const auto& item : items) {
            prefixWeight += item.w;
            if (2 * prefixWeight >= totalWeight) {
                p = item.p;
                q = item.q;
                break;
            }
        }

        if (p < -q) {
            p = -1;
            q = 1;
        } else if (p > q) {
            p = 1;
            q = 1;
        }
    }

    i128 numerator = 0;
    for (int i = 0; i < (int)X.size(); ++i) {
        i128 value = (i128)Y[i] * q - (i128)p * X[i];
        numerator += (value < 0 ? -value : value);
    }

    return {numerator, q};
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int64> A(N), B(N);
    for (auto& x : A) cin >> x;
    for (auto& x : B) cin >> x;

    Cost first = solve(A, B);
    Cost second = solve(B, A);

    Cost answer;
    if (first.num * second.den <= second.num * first.den) {
        answer = first;
    } else {
        answer = second;
    }

    int64 remainder = (int64)(answer.num % answer.den);
    int64 g = gcd(answer.den, remainder);

    cout << toString(answer.num / g) << ' ' << answer.den / g << '\n';
    return 0;
}
