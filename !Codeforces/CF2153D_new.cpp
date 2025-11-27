#include <bits/stdc++.h>
using namespace std;
long long solve(vector<int> &&a) {
    vector<long long> f(a.size());
    f[0] = numeric_limits<long long>::max() / 2; f[1] = abs(a[1] - a[0]);
    f[2] = max({a[0], a[1], a[2]}) - min({a[0], a[1], a[2]});
    for (int i = 3; i < (int)a.size(); i++) {
        auto tri = minmax({a[i-1], a[i-2], a[i]});
        f[i] = min(f[i-2] + abs(a[i] - a[i-1]), f[i-3] + tri.second - tri.first);
    }
    return f.back();
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        vector<int> a(n);
        for (int &i : a) cin >> i;
        a.insert(a.end(), {a[0], a[1]});
        cout << min({solve(vector<int>(a.begin(), a.end()-2)), solve(vector<int>(a.begin()+1, a.end()-1)), solve(vector<int>(a.begin()+2, a.end()))}) << '\n';
    }
    return 0;
}