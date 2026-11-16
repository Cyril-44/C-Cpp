#include <bits/stdc++.h>
using namespace std;
using int64 = long long;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;
    vector<int64> edgeWeight(n + 1), value(n + 1);
    for (int i = 2; i <= n; ++i) {
        int parent;
        cin >> parent >> edgeWeight[i];
    }

    while (m--) {
        int op, x;
        cin >> op >> x;
        if (op == 1) {
            int y;
            cin >> y;
            int64 d;
            if (x == y) d = 0;
            else if (x == 1) d = edgeWeight[y];
            else if (y == 1) d = edgeWeight[x];
            else d = edgeWeight[x] + edgeWeight[y];
            value[x] += d;
        } else {
            // In a star, every SSubTr(x) contains only x.
            cout << value[x] << '\n';
        }
    }
}
