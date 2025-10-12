#include <bits/stdc++.h>
using namespace std;
int f(int x) {
    int y = 0;
    for (int i = 0; i < 31; i++) {
        if (!(x >> i)) break;
        y = (y << 1) | (x >> i & 1);
    }
    return y;
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    freopen("CF2160C.helper", "w", stderr);
    set<int> st;
    for (int i = 1; i <= 1000; i++)
        st.insert(i ^ f(i));
    for (int i : st) fprintf(stderr, "%x %s\n", i, f(f(i) ^ i) == 0 ? "Yes" : "No");
    fclose(stderr);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int x = 0; x <= 20; x++) {
            if (n == (x ^ f(x))) {
                cout << n << " Find " << x << endl;
                break;
            }
        }
    }
    return 0;
}