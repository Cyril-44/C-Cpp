#include <bits/stdc++.h>
constexpr int N = 505;
int a[N], sz[N], b[N];
int q[N], n;
inline int inquire(const int x) {
    std::cout << "? " << x;
    for (int i = 1; i <= n; i++)
        std::cout << ' ' << q[i];
    std::cout << std::endl;
    int res; std::cin >> res;
    return res;
}
int main() {
    int T;
    std::cin >> T;
    while (T--) {
        std::cin >> n;
        for (int i = 2; i <= n; i++) {
            q[1] = n-1;
            for (int j = 2, top = 1; j <= n; j++)
                q[j] = j == i ? n : top++;
            int res = inquire(1);
            sz[i] = res - n * (n-1);
        }
        sz[1] = n;
        std::vector<std::pair<int,int>> ans;
        ans.reserve(n-1);
        std::iota(b+1, b+1+n, 1);
        std::sort(b+1, b+1+n, [](int x, int y){ return sz[x] < sz[y]; });
        for (int ip = 1; ip < n; ip++) {
            for (int jp = 1, cur = n; jp <= n; jp++) {
                if (ip == jp) q[b[jp]] = 1;
                else q[b[jp]] = cur--;
            }
            int res = inquire(b[ip]) - 1, left = n-1;
            for (int jp = 1; jp <= n; jp++) if (ip != jp) {
                if (q[b[jp]] * left == res) {
                    ans.emplace_back(b[ip], b[jp]);
                    break;
                }
                res -= q[b[jp]], left--;
            }
            assert(ans.size() == ip);
        }
        std::cout << "!\n";
        for (const auto &[x, y] : ans) std::cout << x << ' ' << y << '\n';
        std::cout << std::flush;
    }
}