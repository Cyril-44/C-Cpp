#include <bits/stdc++.h>
using namespace std;

static const int MOD = 998244353;

long long power(long long base, long long exp) {
    long long res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

long long modInverse(long long n) {
    return power(n, MOD - 2);
}

// 节点结构体，用于建树合并
struct Node {
    int depth;
    int size; // 子树大小
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N;
    if (!(cin >> N)) return 0;

    vector<int> C(N);
    for (int i = 1; i < N; i++) {
        cin >> C[i];
    }

    // 统计 sum(2^{-C_i})
    // 用频次数组统计 2^{-k} 的个数
    int max_depth = N + 5;
    vector<int> cnt(max_depth + 2, 0);
    for (int i = 1; i < N; i++) {
        if (C[i] < max_depth) {
            cnt[C[i]]++;
        }
    }

    // 进位处理
    for (int i = max_depth; i >= 1; i--) {
        cnt[i - 1] += cnt[i] / 2;
        cnt[i] %= 2;
    }

    // 计算 S = 1 - sum(2^{-C_i}) 的二进制表示
    // 相当于做二进制减法 1.000... - 0.cnt[1]cnt[2]...
    vector<int> S_bits(max_depth + 2, 0);
    int borrow = 0;
    for (int i = max_depth; i >= 1; i--) {
        int diff = 0 - cnt[i] - borrow;
        if (diff < 0) {
            diff += 2;
            borrow = 1;
        } else {
            borrow = 0;
        }
        S_bits[i] = diff;
    }

    // 收集 S 中为 1 的位
    vector<int> ones;
    for (int i = 1; i <= max_depth; i++) {
        if (S_bits[i] == 1) {
            ones.push_back(i);
        }
    }

    int a = -1, b = -1;
    if (ones.size() == 1) {
        a = ones[0] + 1;
        b = ones[0] + 1;
    } else if (ones.size() == 2) {
        a = ones[0];
        b = ones[1];
    } else {
        cout << 0 << "\n";
        return 0;
    }

    // 计算 N!
    long long factN = 1;
    for (int i = 1; i <= N; i++) {
        factN = (factN * i) % MOD;
    }

    // 尝试一组合法 d0, dN，返回得到的排列数，若非法返回 0
    auto solve = [&](int d0, int dN) -> long long {
        vector<int> D(N + 1);
        D[0] = d0;
        for (int i = 1; i < N; i++) D[i] = C[i];
        D[N] = dN;

        int cur_max_d = 0;
        for (int d : D) cur_max_d = max(cur_max_d, d);

        // 按深度分组节点
        vector<vector<Node>> by_depth(cur_max_d + 1);
        // 初始化叶节点（size = 0）
        vector<Node> current_nodes;
        for (int d : D) {
            current_nodes.push_back({d, 0});
        }

        // 自底向上合并
        for (int d = cur_max_d; d >= 1; d--) {
            vector<Node> next_level;
            int n = current_nodes.size();
            int i = 0;
            while (i < n) {
                if (current_nodes[i].depth != d) {
                    next_level.push_back(current_nodes[i]);
                    i++;
                } else {
                    int j = i;
                    while (j < n && current_nodes[j].depth == d) {
                        j++;
                    }
                    int len = j - i;
                    if (len % 2 != 0) return 0; // 奇数个节点无法配对

                    for (int k = i; k < j; k += 2) {
                        int sz = 1 + current_nodes[k].size + current_nodes[k + 1].size;
                        next_level.push_back({d - 1, sz});
                    }
                    i = j;
                }
            }
            current_nodes = move(next_level);
        }

        if (current_nodes.size() != 1) return 0;

        // 计算 Prod(size(v))
        // 根节点及内部节点的 size 在合并过程中已经确定
        long long prod_size = 1;
        // 递归/遍历收集所有内节点 size
        // 这里可以直接在合并时累计 prod
        // 重新做一遍合并同时乘 prod
        long long prod = 1;
        current_nodes.clear();
        for (int depth_val : D) {
            current_nodes.push_back({depth_val, 0});
        }

        for (int depth_val = cur_max_d; depth_val >= 1; depth_val--) {
            vector<Node> next_level;
            int n = current_nodes.size();
            int i = 0;
            while (i < n) {
                if (current_nodes[i].depth != depth_val) {
                    next_level.push_back(current_nodes[i]);
                    i++;
                } else {
                    int j = i;
                    while (j < n && current_nodes[j].depth == depth_val) j++;
                    for (int k = i; k < j; k += 2) {
                        int sz = 1 + current_nodes[k].size + current_nodes[k + 1].size;
                        prod = (prod * sz) % MOD;
                        next_level.push_back({depth_val - 1, sz});
                    }
                    i = j;
                }
            }
            current_nodes = move(next_level);
        }

        long long ans = (factN * modInverse(prod)) % MOD;
        return ans;
    };

    long long total_ans = 0;
    set<pair<int, int>> candidate_pairs = {{a, b}, {b, a}};
    for (auto p : candidate_pairs) {
        total_ans = (total_ans + solve(p.first, p.second)) % MOD;
    }

    cout << total_ans << "\n";
    return 0;
}