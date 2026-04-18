#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int MAXN = 1000005;
int n, q, a[MAXN], pos[MAXN];
int min_tree[MAXN << 2], max_tree[MAXN << 2];

void build(int node, int start, int end) {
    if (start == end) {
        min_tree[node] = max_tree[node] = pos[start];
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    min_tree[node] = min(min_tree[2 * node], min_tree[2 * node + 1]);
    max_tree[node] = max(max_tree[2 * node], max_tree[2 * node + 1]);
}

// 在线段树上查询从 k 开始，满足位置在 [L, R] 之间的最长连续段
int query(int node, int start, int end, int k, int L, int R) {
    if (end < k || (min_tree[node] >= L && max_tree[node] <= R)) {
        if (end < k) return 0;
        return end - max(start, k) + 1;
    }
    if (start == end) return 0;
    int mid = (start + end) / 2;
    int res = query(2 * node, start, mid, k, L, R);
    // 如果左子树（包含k的部分）能全覆盖，才看右子树
    if (res == max(0, mid - max(start, k) + 1)) {
        res += query(2 * node + 1, mid + 1, end, k, L, R);
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    if (!(cin >> n >> q)) return 0;
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
        pos[a[i]] = i;
    }
    build(1, 1, n);
    while (q--) {
        int l, r, k;
        cin >> l >> r >> k;
        if (k > n || pos[k] < l || pos[k] > r) {
            cout << 0 << "\n";
            continue;
        }
        cout << query(1, 1, n, k, l, r) << "\n";
    }
    return 0;
}