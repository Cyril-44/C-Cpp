#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, x, y;
    cin >> T;
    while (T--) {
        cin >> x >> y;
        if (x < y) puts("2");
        else if (x - 1 <= y || y == 1) puts("-1");
        else puts("3");
    }
    return 0;
}