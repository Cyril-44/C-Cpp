#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
char s[N];
int main() {
    cin.tie(nullptr) -> sync_with_stdio(false);
    int T, n, k;
    cin >> T;
    while (T--) {
        cin >> n >> k >> s;
        int l=1, r=n, ul=1, ur=n;
        for (int i = 0; i < k; i++) {
            switch (s[i]) {
            case '0':
                ++l, ++ul;
                break;
            case '1':
                --r, --ur;
                break;
            case '2':
                ++ul, --ur;
                break;
            }
        }
        if (ur < l && ul > r) l = n+1, r = 0;
        for (int i = 1; i <= n; i++)
            cout.put(i < l || i > r ? '-' : i < ul || i > ur ? '?' : '+');
        cout.put('\n');
    }
    return 0;
}