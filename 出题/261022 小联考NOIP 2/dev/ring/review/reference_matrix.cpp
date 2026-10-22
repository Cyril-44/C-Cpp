#include <bits/stdc++.h>
using namespace std;
constexpr long long P = 1000000007;
struct Mat { long long a, b, c, d; };
Mat add(Mat x, Mat y) {
    return {(x.a+y.a)%P, (x.b+y.b)%P, (x.c+y.c)%P, (x.d+y.d)%P};
}
// x * [[1,1],[1,0]] * y: no adjacent boundary leaves may both be selected.
Mat join(Mat x, Mat y) {
    Mat z{(x.a+x.b)%P, x.a, (x.c+x.d)%P, x.c};
    return {(z.a*y.a+z.b*y.c)%P, (z.a*y.b+z.b*y.d)%P,
            (z.c*y.a+z.d*y.c)%P, (z.c*y.b+z.d*y.d)%P};
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    if (!(cin >> n)) return 0;
    vector<int> parent(n+1);
    for (int u=2; u<=n; ++u) cin >> parent[u];
    // Eight uint32 cells per node; multiplication uses long long.
    vector<array<int,8>> dp(n+1);
    vector<unsigned char> has_child(n+1);
    int leaves = 0;
    auto read = [&](int u, int s) {
        int k=4*s;
        return Mat{dp[u][k],dp[u][k+1],dp[u][k+2],dp[u][k+3]};
    };
    auto write = [&](int u, int s, Mat m) {
        int k=4*s;
        dp[u][k]=m.a; dp[u][k+1]=m.b; dp[u][k+2]=m.c; dp[u][k+3]=m.d;
    };
    // Completing children in decreasing label order reverses the entire DFS
    // leaf sequence. An undirected leaf cycle is invariant under reversal.
    for (int u=n; u>=1; --u) {
        if (!has_child[u]) {
            ++leaves;
            write(u,0,{1,0,0,0});
            write(u,1,{0,0,0,1});
        }
        if (u == 1) break;
        int p=parent[u];
        Mat no=add(read(u,0),read(u,1)), yes=read(u,0);
        if (has_child[p]) {
            no=join(read(p,0),no);
            yes=join(read(p,1),yes);
        }
        write(p,0,no); write(p,1,yes);
        has_child[p]=true;
    }
    Mat root=add(read(1,0),read(1,1));
    cout << (root.a+root.b+root.c+(leaves==1 ? root.d : 0))%P << '\n';
}
