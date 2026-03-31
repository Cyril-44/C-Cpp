#include <bits/stdc++.h>
using namespace std;
using int64 = long long;
using i128 = __int128_t;

i128 F(i128 Y, int64 a, int64 b, int64 c){
    if(Y < 0) return 0;
    return Y / a - Y / b + Y / c;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int64 C;
    int t;
    if(!(cin >> C >> t)) return 0;
    while(t--){
        int64 m,a,b,c,n;
        cin >> m >> a >> b >> c >> n;
        i128 base = F((i128)m - 1, a, b, c);
        i128 need = n;
        i128 lo = m;
        i128 hi = m;
        const i128 LIMIT = (i128)9e18;
        // expand hi until enough
        while(F(hi, a, b, c) - base < need){
            hi = hi * 2;
            if(hi > LIMIT) { hi = LIMIT; break; }
        }
        // binary search
        while(lo < hi){
            i128 mid = (lo + hi) >> 1;
            if(F(mid, a, b, c) - base >= need) hi = mid;
            else lo = mid + 1;
        }
        long long ans = (long long)lo;
        cout << ans << '\n';
    }
    return 0;
}
