#include <bits/stdc++.h>
using namespace std;
// Independent reverse-transition DP oracle. Input/output are values only;
// this is a local review tool, not a submitted solution.
int main() {
    int n;
    while (cin >> n) {
        assert(2 <= n && n <= 4);
        int k=n*n, full=(1<<k)-1;
        vector<int> f((1<<k)*k,-1000000);
        f[k]=0;
        for (int s=1;s<=full;s+=2)
            for (int v=1;v<k;++v) if (s>>v&1) {
                int prior=s^(1<<v);
                for (int u=0;u<k;++u)
                    if ((prior>>u&1) && max(abs(u/n-v/n),abs(u%n-v%n))==1)
                        f[s*k+v]=max(f[s*k+v],f[prior*k+u]+int(v==u+1 && v/n==u/n));
            }
        cout << f[full*k+k-1] << '\n';
    }
}
