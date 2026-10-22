#include "partial_common.h"
int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    int t; cin >> t;
    while (t--) {
        int n; cin >> n;
        if (n > 3) { unsupported(); continue; }
        int best = -1, k = n*n;
        vector<int> path{0}, bestPath;
        auto dfs = [&](auto&& self, int u, int mask, int score) -> void {
            if (u == k-1) {
                if (mask == (1<<k)-1 && score > best) { best=score; bestPath=path; }
                return;
            }
            for (int v=0; v<k; ++v)
                if (!(mask>>v&1) && max(abs(u/n-v/n),abs(u%n-v%n))==1) {
                    path.push_back(v);
                    self(self,v,mask|(1<<v),score+(u/n==v/n && v%n==u%n+1));
                    path.pop_back();
                }
        };
        dfs(dfs,0,1,0);
        printPath(encode(bestPath,n));
    }
}
