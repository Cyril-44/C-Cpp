#include <cstdio>
#include <map>
#include <set>
#include <vector>
const int N = 305;
int a[N][N];
long long s[N][N];
int main() {
    int t, n;
    scanf("%d", &t);
    while (t--) {
        scanf("%d", &n);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= n; j++)
                scanf("%d", &a[i][j]);
        for (int i = 1; i <= n; i++) {
            s[i][n + 1] = 0;
            for (int j = n; j >= 1; j--)
                s[i][j] = s[i][j + 1] + a[i][j];
        } 
        std::map<int, std::vector<int>> mp;
        for (int i = 1; i <= n; i++)
            for (int j = n; j >= 1; j--)
                mp[a[i][j]].push_back(i);
        int ans = 1;
        bool flg = true;
        std::map<int, int> mp2;
        while (ans < n) {
            flg = true;
            for (const int &i : mp[ans]) {
                ++mp2[i];
            }
            for (const auto &[i, j] : mp2) {
                if (j > mp2.size()) {
                    flg = false;
                    break;
                }
            }
            if (flg && mp2.size() >= ans) ++ans;
            else break;
        }
        printf("%d\n", ans);
    }
    return 0;
}