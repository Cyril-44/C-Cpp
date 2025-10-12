#include <bits/stdc++.h>
using namespace std;
int f(int x, int b) {
    int y = 0;
    for (int i = 0; i < b; i++) {
        y = (y << 1) | (x >> i & 1);
    }
    return y;
}
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        if (n == 0) {puts("YES"); continue;}
        int bits = 32 - __builtin_clz(n);
        int now = f(n, bits) ^ n;
        puts(now == f(now, bits) ? "Yes" : "No");
    }
    return 0;
}