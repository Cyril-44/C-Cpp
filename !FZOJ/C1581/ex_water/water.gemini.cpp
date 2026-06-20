#include <bits/stdc++.h>

using namespace std;

using i64 = long long;

constexpr int MAXN = 200000 + 5;
constexpr int LOG = 19;

int n, q;
int heightv[MAXN];

int left_child[MAXN], right_child[MAXN];
int up[LOG][MAXN];

i64 subtree_size[MAXN];
i64 subtree_sum[MAXN];

i64 fill_self[MAXN];
i64 fill_parent[MAXN];

int root_node;

int dfn_left[MAXN], rev_left[MAXN], timer_left;
int dfn_right[MAXN], rev_right[MAXN], timer_right;

i64 prefix_cost[MAXN];
i64 suffix_cost[MAXN];

void build_info(int u) {
    subtree_size[u] = 1;
    subtree_sum[u] = heightv[u];

    for (int k = 1; k < LOG; ++k) {
        up[k][u] = up[k - 1][up[k - 1][u]];
    }

    if (left_child[u]) {
        build_info(left_child[u]);
        subtree_size[u] += subtree_size[left_child[u]];
        subtree_sum[u] += subtree_sum[left_child[u]];
    }

    if (right_child[u]) {
        build_info(right_child[u]);
        subtree_size[u] += subtree_size[right_child[u]];
        subtree_sum[u] += subtree_sum[right_child[u]];
    }

    fill_self[u] = subtree_size[u] * 1LL * heightv[u] - subtree_sum[u];
    fill_parent[u] =
        subtree_size[u] * 1LL * heightv[up[0][u]] - subtree_sum[u];
}

void build_prefix(int u, i64 acc = 0) {
    if (left_child[u]) {
        build_prefix(left_child[u], acc);
        acc += fill_parent[left_child[u]];
    }

    if (right_child[u]) {
        build_prefix(right_child[u], acc);
        acc += fill_parent[right_child[u]];
    }

    dfn_left[u] = ++timer_left;
    rev_left[timer_left] = u;
    prefix_cost[timer_left] = acc;
}

void build_suffix(int u, i64 acc = 0) {
    if (right_child[u]) {
        build_suffix(right_child[u], acc);
        acc += subtree_size[right_child[u]] * 1LL * heightv[u] -
               subtree_sum[right_child[u]];
    }

    if (left_child[u]) {
        build_suffix(left_child[u], acc);
        acc += subtree_size[left_child[u]] * 1LL * heightv[u] -
               subtree_sum[left_child[u]];
    }

    dfn_right[u] = ++timer_right;
    rev_right[timer_right] = u;
    suffix_cost[timer_right] = acc;
}

i64 solve_left(int u, i64 water) {
    water += suffix_cost[dfn_right[u]] - fill_parent[left_child[u]];

    int l = 1;
    int r = n;

    while (l <= r) {
        int mid = (l + r) >> 1;

        if (suffix_cost[mid] < water) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }

    return r - (dfn_right[u] - subtree_size[left_child[u]] - 1);
}

i64 solve_right(int u, i64 water) {
    water += prefix_cost[dfn_left[u]] - fill_parent[right_child[u]];

    int l = 1;
    int r = n;

    while (l <= r) {
        int mid = (l + r) >> 1;

        if (prefix_cost[mid] < water) {
            l = mid + 1;
        } else {
            r = mid - 1;
        }
    }

    return r - (dfn_left[u] - subtree_size[right_child[u]] - 1);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;

    heightv[0] = INT_MAX;

    vector<int> stk;

    for (int i = 1; i <= n; ++i) {
        cin >> heightv[i];

        while (!stk.empty() && heightv[stk.back()] < heightv[i]) {
            int cur = stk.back();
            stk.pop_back();

            if (stk.empty() || heightv[stk.back()] > heightv[i]) {
                up[0][cur] = i;
                left_child[i] = cur;
                break;
            }

            up[0][cur] = stk.back();
            right_child[stk.back()] = cur;
        }

        stk.push_back(i);
    }

    while (!stk.empty()) {
        int cur = stk.back();
        stk.pop_back();

        if (stk.empty()) {
            root_node = cur;
            break;
        }

        up[0][cur] = stk.back();
        right_child[stk.back()] = cur;
    }

    build_info(root_node);
    build_prefix(root_node);
    build_suffix(root_node);

    cin >> q;

    while (q--) {
        int pos;
        i64 water;

        cin >> pos >> water;

        if (fill_self[pos] >= water) {
            if (fill_parent[right_child[pos]] < water / 2) {
                cout << subtree_size[right_child[pos]] +
                            solve_left(
                                pos, water - fill_parent[right_child[pos]])
                     << '\n';
            } else if (fill_parent[left_child[pos]] < water / 2) {
                cout << subtree_size[left_child[pos]] +
                            solve_right(
                                pos, water - fill_parent[left_child[pos]])
                     << '\n';
            } else {
                cout << solve_left(pos, water / 2) +
                            solve_right(pos, water / 2)
                     << '\n';
            }
        } else {
            int u = pos;

            for (int k = LOG - 1; k >= 0; --k) {
                if (up[k][u] && fill_self[up[k][u]] < water) {
                    u = up[k][u];
                }
            }

            if (fill_parent[u] >= water) {
                cout << subtree_size[u] << '\n';
                continue;
            }

            i64 answer = subtree_size[u];

            if (right_child[up[0][u]] == u) {
                answer += solve_left(up[0][u], water - fill_parent[u]);
            } else {
                answer += solve_right(up[0][u], water - fill_parent[u]);
            }

            cout << answer << '\n';
        }
    }

    return 0;
}