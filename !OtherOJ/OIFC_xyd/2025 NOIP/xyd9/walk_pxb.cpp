#include <bits/stdc++.h>
using namespace std;
int test_cases, n, m, k, depth[100005], parent[100005], on_path[100005], path_length, visited_count, special_count, found_end;
bool visited[100005];
vector<int> graph[100005];
void in(int&x);
void out(int x);
void dfs_tree(int x, int fa) {
    depth[x] = depth[fa] + 1;
    parent[x] = fa;
    for (int y : graph[x]) {
        if (y != fa) {
            dfs_tree(y, x);
        }
    }
}
void output_path(int x, int fa) {
    out(x);
    putchar(' ');
    visited[x] = 1;
    visited_count++;
    if (on_path[x])
        special_count++;
    if (visited_count == k) {
        found_end = x;
        return;
    }
    int next_vertex = 0;
    for (int y : graph[x]) {
        if (y != fa) {
            if (!on_path[y]) {
                if (visited_count - special_count < k - path_length)
                    output_path(y, x);
            } else
                next_vertex = y;
        }
    }
    if (next_vertex) {
        output_path(next_vertex, x);
    }
    if (!found_end) {
        out(parent[x]);
        putchar(' ');
    }
}
int main() {
    freopen("walk.in", "r", stdin);
    freopen("walk.out", "w", stdout);
    in(test_cases);
    while (test_cases--) {
        in(n), in(m), in(k);
        for (int i = 1; i <= n; i++) {
            graph[i].clear();
        }
        for (int i = 1; i < n; i++) {
            int x, y;
            in(x), in(y);
            graph[x].push_back(y);
            graph[y].push_back(x);
        }
        dfs_tree(1, 0);
        int start_vertex = 1;
        for (int i = 1; i <= n; i++) {
            if (depth[start_vertex] < depth[i])
                start_vertex = i;
        }
        dfs_tree(start_vertex, 0);
        int end_vertex = start_vertex;
        for (int i = 1; i <= n; i++) {
            if (depth[end_vertex] < depth[i])
                end_vertex = i;
            on_path[i] = visited[i] = 0;
        }
        path_length = 1;
        for (int i = end_vertex; i != start_vertex; i = parent[i]) {
            path_length++;
            on_path[i] = 1;
        }
        on_path[start_vertex] = 1;
        int remaining_steps = 0;
        if (k < path_length) {
            if (k > m) {
                puts("No");
                continue;
            } else if (k < m)
                remaining_steps = m - k;
        } else {
            if (path_length + (k - path_length) * 2 > m) {
                puts("No");
                continue;
            } else if (path_length + (k - path_length) * 2 < m) {
                remaining_steps = m - path_length - (k - path_length) * 2;
            }
        }
        puts("Yes");
        visited_count = special_count = 0;
        found_end = 0;
        output_path(start_vertex, 0);
        end_vertex = found_end;
        while (remaining_steps--) {
            int next_node;
            for (int neighbor : graph[end_vertex]) {
                if (visited[neighbor]) {
                    next_node = neighbor;
                    break;
                }
            }
            end_vertex = next_node;
            out(end_vertex);
            putchar(' ');
        }
        putchar('\n');
    }
    return 0;
}
void in(int& x) {
    x = 0;
    char ch = getchar();
    while (ch < '0' || ch > '9')
        ch = getchar();
    while (ch >= '0' && ch <= '9') {
        x = (x << 3) + (x << 1) + ch - 48;
        ch = getchar();
    }
}
void out(int x) {
    if (!x) return;
    out(x / 10);
    putchar(x % 10 + 48);
}