#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

const int MAXN = 200005;
struct Person {
    int w, v;
};

bool compareW(const Person& a, const Person& b) {
    return a.w > b.w;
}

// 主席树部分
int root[MAXN], L[MAXN * 40], R[MAXN * 40], cnt[MAXN * 40], node_count;
int val_v[MAXN], sorted_v[MAXN], m;

int update(int prev, int l, int r, int val) {
    int curr = ++node_count;
    L[curr] = L[prev]; R[curr] = R[prev]; cnt[curr] = cnt[prev] + 1;
    if (l == r) return curr;
    int mid = (l + r) >> 1;
    if (val <= mid) L[curr] = update(L[prev], l, mid, val);
    else R[curr] = update(R[prev], mid + 1, r, val);
    return curr;
}

int query(int node, int l, int r, int k) {
    if (l == r) return sorted_v[l - 1];
    int mid = (l + r) >> 1;
    int right_cnt = cnt[R[node]];
    if (k <= right_cnt) return query(R[node], mid + 1, r, k);
    else return query(L[node], l, mid, k - right_cnt);
}

int n;
Person p[MAXN];
ll ans[MAXN];

void solve(int kL, int kR, int iL, int iR) {
    if (kL > kR) return;
    int kMid = (kL + kR) >> 1;
    ll best_val = -1;
    int best_i = iL;

    // 决策点 i 必须至少为 kMid，因为要有 k 个人
    for (int i = max(iL, kMid); i <= iR; ++i) {
        ll current_v = p[i-1].w + (ll)query(root[i], 1, m, kMid);
        if (current_v >= best_val) {
            best_val = current_v;
            best_i = i;
        }
    }

    ans[kMid] = best_val;
    solve(kL, kMid - 1, iL, best_i);
    solve(kMid + 1, kR, best_i, iR);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    if (!(cin >> n)) return 0;
    for (int i = 0; i < n; ++i) {
        cin >> p[i].w >> p[i].v;
        sorted_v[i] = p[i].v;
    }

    // 预处理：排序与离散化
    sort(p, p + n, compareW);
    sort(sorted_v, sorted_v + n);
    m = unique(sorted_v, sorted_v + n) - sorted_v;

    for (int i = 1; i <= n; ++i) {
        int v_pos = lower_bound(sorted_v, sorted_v + m, p[i-1].v) - sorted_v + 1;
        root[i] = update(root[i-1], 1, m, v_pos);
    }

    solve(1, n, 1, n);

    for (int i = 1; i <= n; ++i) {
        cout << ans[i] << (i == n ? "" : " ");
    }
    cout << endl;

    return 0;
}