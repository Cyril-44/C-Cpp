#include <bits/stdc++.h>
using namespace std;
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, x, y, z;
    cin >> T;
    while (T--) {
        cin >> x >> y >> z;
        int xx = x & y, yy = y & z, zz = z & x;
        if (xx != yy || yy != zz || zz != xx) puts("NO");
        else puts("YES");
    }
    return 0;
}