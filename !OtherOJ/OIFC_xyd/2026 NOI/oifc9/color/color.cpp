#include <cstdio>
#include <fstream>
#include <cassert>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
constexpr int N = 605;
bool pin[N][N];
uint8_t mp[N][N], ban[N][N];
int n;
bool dfs(int x, int y) {
    if (y > n) return dfs(x+1, 1);
    if (x > n) return true;
    if (pin[x][y]) return dfs(x, y+1);
    int bann = ban[x][y] | 1<<mp[x-1][y] | 1<<mp[x][y-1];
    For(v, 1, 3)
        if (!(bann >> v & 1) && (mp[x][y]=v, dfs(x, y+1))) return true;
    return false;
}
int main() {
    std::ifstream fin("color.in");
    std::ofstream fout("color.out");
    int T, m;
    fin >> T;
    while (T--) {
        fin >> n >> m;
        For(i, 1, n) For(j, 1, n) pin[i][j] = false;
        For(i, 1, n+1) For(j, 1, n+1) mp[i][j] = 0;
        For(i, 1, m) {
            int x, y, z;
            fin >> x >> y >> z;
            pin[x][y] = true;
            mp[x][y] = z;
        }
        For(i, 1, n) For(j, 1, n) {
            ban[i][j] = (1<<mp[i][j+1]) | (1<<mp[i+1][j]);
        }
        assert(dfs(1, 1));
        For(i, 1, n) {
            For(j, 1, n) fout << (char)(mp[i][j] | '0');
            fout << '\n';
        }
    }
    return 0;
}