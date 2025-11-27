#include <bits/stdc++.h>
using namespace std;
constexpr int N = 100005;
int a[N];
long long b[N];
vector<int> mem[N];
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++) cin >> b[i], mem[i].clear();
        int tot = 0;
        for (int i = 1; i <= n; i++) {
            int same = i - (b[i] - b[i-1]);
            if (!same) a[i] = ++tot;
            else a[i] = a[same];
        }
        for (int i = 1; i <= n; i++) cout << a[i] << ' ';
        cout << '\n';
    }
    return 0;
}
