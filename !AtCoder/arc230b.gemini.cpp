#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct Operation {
    string type;
    int l, r;
};

void solve() {
    int N;
    if (!(cin >> N)) return;

    vector<int> X(N + 1), Y(N + 1), A(N + 1), B(N + 1);
    for (int i = 1; i <= N; ++i) cin >> X[i];
    for (int i = 1; i <= N; ++i) cin >> Y[i];
    for (int i = 1; i <= N; ++i) cin >> A[i];
    for (int i = 1; i <= N; ++i) cin >> B[i];

    // 1. 判断合法的 chmin 区间
    vector<vector<bool>> valid_min(N + 1, vector<bool>(N + 1, false));
    for (int l = 1; l <= N; ++l) {
        int max_B = 0;
        for (int r = l; r <= N; ++r) {
            max_B = max(max_B, B[r]);
            int len = r - l + 1;
            if (Y[len] >= max_B) {
                valid_min[l][r] = true;
            }
        }
    }

    // 2. 标记哪些位置是 chmin-covered 的
    vector<bool> chmin_covered(N + 1, false);
    for (int l = 1; l <= N; ++l) {
        for (int r = l; r <= N; ++r) {
            if (valid_min[l][r]) {
                int len = r - l + 1;
                for (int i = l; i <= r; ++i) {
                    if (Y[len] == B[i]) {
                        chmin_covered[i] = true;
                    }
                }
            }
        }
    }

    // 3. 计算每个位置 chmax 的上限 Cap
    const int INF = 1e9;
    vector<int> Cap(N + 1);
    for (int i = 1; i <= N; ++i) {
        if (chmin_covered[i]) {
            Cap[i] = INF;
        } else {
            Cap[i] = max(A[i], B[i]);
        }
    }

    // 4. 判断合法的 chmax 区间
    vector<vector<bool>> valid_max(N + 1, vector<bool>(N + 1, false));
    for (int l = 1; l <= N; ++l) {
        int min_cap = INF;
        for (int r = l; r <= N; ++r) {
            min_cap = min(min_cap, Cap[r]);
            int len = r - l + 1;
            if (X[len] <= min_cap) {
                valid_max[l][r] = true;
            }
        }
    }

    // 5. 可行性检查
    for (int i = 1; i <= N; ++i) {
        if (B[i] < A[i]) {
            if (!chmin_covered[i]) {
                cout << -1 << "\n";
                return;
            }
        } else if (B[i] > A[i]) {
            bool ok = false;
            for (int l = 1; l <= i; ++l) {
                for (int r = i; r <= N; ++r) {
                    if (valid_max[l][r]) {
                        int len = r - l + 1;
                        if (X[len] == B[i] || (chmin_covered[i] && X[len] >= B[i])) {
                            ok = true;
                            break;
                        }
                    }
                }
                if (ok) break;
            }
            if (!ok) {
                cout << -1 << "\n";
                return;
            }
        }
    }

    // 6. 构造操作序列
    vector<Operation> ops;

    // Phase 1: chmax
    vector<bool> satisfied_max(N + 1, false);
    for (int i = 1; i <= N; ++i) {
        if (A[i] >= B[i]) satisfied_max[i] = true;
    }

    for (int i = 1; i <= N; ++i) {
        if (!satisfied_max[i]) {
            // 找一个能覆盖 i 的最佳 chmax 区间
            int best_l = -1, best_r = -1;
            for (int l = 1; l <= i; ++l) {
                for (int r = i; r <= N; ++r) {
                    if (valid_max[l][r]) {
                        int len = r - l + 1;
                        if (X[len] == B[i] || (chmin_covered[i] && X[len] >= B[i])) {
                            if (r > best_r) {
                                best_l = l;
                                best_r = r;
                            }
                        }
                    }
                }
            }
            if (best_l != -1) {
                ops.push_back({"chmax", best_l, best_r});
                int len = best_r - best_l + 1;
                for (int j = best_l; j <= best_r; ++j) {
                    if (X[len] == B[j] || (chmin_covered[j] && X[len] >= B[j])) {
                        satisfied_max[j] = true;
                    }
                }
            }
        }
    }

    // 计算 chmax 操作后的中间序列 A_mid
    vector<int> A_mid = A;
    for (const auto& op : ops) {
        if (op.type == "chmax") {
            int len = op.r - op.l + 1;
            for (int i = op.l; i <= op.r; ++i) {
                A_mid[i] = max(A_mid[i], X[len]);
            }
        }
    }

    // Phase 2: chmin
    vector<bool> satisfied_min(N + 1, false);
    for (int i = 1; i <= N; ++i) {
        if (A_mid[i] == B[i]) satisfied_min[i] = true;
    }

    for (int i = 1; i <= N; ++i) {
        if (!satisfied_min[i]) {
            int best_l = -1, best_r = -1;
            for (int l = 1; l <= i; ++l) {
                for (int r = i; r <= N; ++r) {
                    if (valid_min[l][r]) {
                        int len = r - l + 1;
                        if (Y[len] == B[i]) {
                            if (r > best_r) {
                                best_l = l;
                                best_r = r;
                            }
                        }
                    }
                }
            }
            if (best_l != -1) {
                ops.push_back({"chmin", best_l, best_r});
                int len = best_r - best_l + 1;
                for (int j = best_l; j <= best_r; ++j) {
                    if (Y[len] == B[j]) {
                        satisfied_min[j] = true;
                    }
                }
            }
        }
    }

    // 输出答案
    cout << ops.size() << "\n";
    for (const auto& op : ops) {
        cout << op.type << " " << op.l << " " << op.r << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    if (cin >> T) {
        while (T--) {
            solve();
        }
    }
    return 0;
}