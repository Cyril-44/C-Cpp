#include <bits/extc++.h>
using ULL = unsigned long long;
namespace _Tree {
    using namespace __gnu_pbds;
    using Tree = tree<ULL, null_type, std::less<ULL>, rb_tree_tag, tree_order_statistics_node_update>;
} using _Tree::Tree;
int main() {
    int n, m;
    ULL ord = 0;
    scanf("%d%d", &n, &m);
    Tree tr;
    for (int i = 0, ai; i < n; i++) {
        scanf("%d", &ai);
        tr.insert((ULL)ai << 21 | ++ord);
    }
    ULL last_ans = 0, ans = 0;
    for (int opt, x; m--; ) {
        scanf("%d%d", &opt, &x);
        x ^= last_ans;
        switch (opt) {
        case 1: tr.insert((ULL)x << 21 | ++ord); break;
        case 2: tr.erase(tr.lower_bound((ULL)x << 21)); break;
        case 3: ans ^= (last_ans = tr.order_of_key((ULL)x << 21) + 1); break;
        case 4: ans ^= (last_ans = *tr.find_by_order(x - 1) >> 21); break;
        case 5: ans ^= (last_ans = *std::prev(tr.lower_bound((ULL)x << 21)) >> 21); break;
        case 6: ans ^= (last_ans = *tr.upper_bound((x + 1ull) << 21) >> 21); break;
        }
    }
    printf("%llu\n", ans);
    return 0;
}