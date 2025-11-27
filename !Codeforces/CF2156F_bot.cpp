#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        int n; cin >> n;
        vector<int> p(n+1), pos(n+1);
        for (int i = 1; i <= n; ++i) {
            cin >> p[i];
            pos[p[i]] = i;
        }
        for (int i = 1; i <= n; ++i) {
            while (p[i] >= 3) {
                int v = p[i];
                if (pos[v-1] > i && pos[v-2] > i) {
                    int a = pos[v-1];
                    int b = pos[v-2];
                    p[i] = v - 2;
                    p[a] = v;
                    p[b] = v - 1;
                    pos[v] = a;
                    pos[v-1] = b;
                    pos[v-2] = i;
                } else break;
            }
        }
        for (int i = 1; i <= n; ++i) {
            if (i > 1) cout << ' ';
            cout << p[i];
        }
        cout << '\n';
    }
    return 0;
}
