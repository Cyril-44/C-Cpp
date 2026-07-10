#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n = 20;          // 小数据规模，可自行修改
    int extra = 20;      // 额外边数量（不交叉）
    int q = 30;          // 询问数量

    cout << n << " " << (n-1 + extra) << " " << q << "\n";

    // 链边
    for (int i = 1; i < n; i++) {
        long long w = rng() % 20 + 1;
        cout << i << " " << i+1 << " " << w << "\n";
    }

    // 生成不交叉边：按 r 递增，l 递减
    vector<pair<int,int>> segs;
    for (int i = 0; i < extra; i++) {
        int r = rng() % (n-1) + 2;      // r ∈ [2,n]
        int l = rng() % (r-1) + 1;      // l ∈ [1,r-1]
        segs.push_back({l,r});
    }

    sort(segs.begin(), segs.end(), [&](auto &a, auto &b){
        if (a.second != b.second) return a.second < b.second;
        return a.first > b.first;
    });

    for (auto &e : segs) {
        long long w = rng() % 20 + 1;
        cout << e.first << " " << e.second << " " << w << "\n";
    }

    // 询问
    for (int i = 0; i < q; i++) {
        int x = rng() % n + 1;
        int y = rng() % n + 1;
        cout << x << " " << y << "\n";
    }

    return 0;
}
