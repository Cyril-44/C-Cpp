#include <bits/extc++.h>
using Tree = __gnu_pbds::tree<int, int, std::less<int>, __gnu_pbds::rb_tree_tag, __gnu_pbds::tree_order_statistics_node_update>;
constexpr int N = 100005;
Tree row[N], last_col;
int main() {
    int n, m, q;
    scanf("%d%d%d", &n, &m, &q);
    for (int x, y; q--; ) {
        scanf("%d%d", &x, &y);
        if (y == m) {
            auto it = last_col.find_by_order(y);
            
            last_col.erase(it);
        }
    }
    return 0;
}
