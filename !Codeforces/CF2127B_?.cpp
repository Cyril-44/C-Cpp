#include <bits/stdc++.h>
using namespace std;

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while(T--){
        int n, x;
        string s;
        cin >> n >> x >> s;
        // 1-based indexing of s:
        s = " " + s;

        // find closest '#' to the left of x
        int L = 0;
        for(int i = x-1; i >= 1; i--){
            if(s[i]=='#'){
                L = i;
                break;
            }
        }
        // find closest '#' to the right of x
        int R = n+1;
        for(int i = x+1; i <= n; i++){
            if(s[i]=='#'){
                R = i;
                break;
            }
        }

        const int INF = 1e9;
        int dL = (L>0 ? x - L : INF);
        int dR = (R<=n ? R - x : INF);

        int ans;
        if(dL < INF && dR < INF){
            // walls on both sides
            ans = 1 + min(dL, dR);
        } else {
            // at least one side has no wall initially
            ans = 1;
        }

        cout << ans << "\n";
    }
    return 0;
}
