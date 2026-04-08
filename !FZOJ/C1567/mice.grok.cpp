#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Mouse {
    ll L, R;
};

struct Node {
    vector<ll> sortedR;
    vector<ll> pref;
};

int n, K;
vector<Mouse> mice;
vector<ll> Ls;
vector<ll> prefix;
vector<ll> all_end;
vector<Node> tree;
vector<ll> dp_prev, dp_curr;
ll base = 0;

void build(int node, int start, int end) {
    if (start == end) {
        tree[node].sortedR = {mice[start].R};
        tree[node].pref = {0, mice[start].R};
        return;
    }
    int mid = (start + end) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    auto& left = tree[2 * node].sortedR;
    auto& right = tree[2 * node + 1].sortedR;
    tree[node].sortedR.resize(left.size() + right.size());
    merge(left.begin(), left.end(), right.begin(), right.end(), tree[node].sortedR.begin());
    auto& s = tree[node].sortedR;
    tree[node].pref.resize(s.size() + 1, 0);
    for (size_t j = 1; j <= s.size(); ++j) {
        tree[node].pref[j] = tree[node].pref[j - 1] + s[j - 1];
    }
}

int query_count(int node, int start, int end, int ql, int qr, ll vv) {
    if (qr < start || end < ql) return 0;
    if (ql <= start && end <= qr) {
        auto& s = tree[node].sortedR;
        auto it = upper_bound(s.begin(), s.end(), vv);
        return it - s.begin();
    }
    int mid = (start + end) / 2;
    return query_count(2 * node, start, mid, ql, qr, vv) + query_count(2 * node + 1, mid + 1, end, ql, qr, vv);
}

ll query_sum(int node, int start, int end, int ql, int qr, ll vv) {
    if (qr < start || end < ql) return 0;
    if (ql <= start && end <= qr) {
        auto& s = tree[node].sortedR;
        auto it = upper_bound(s.begin(), s.end(), vv);
        int pos = it - s.begin();
        return tree[node].pref[pos];
    }
    int mid = (start + end) / 2;
    return query_sum(2 * node, start, mid, ql, qr, vv) + query_sum(2 * node + 1, mid + 1, end, ql, qr, vv);
}

ll compute_f(int left, int right) {
    int N = right - left + 1;
    if (N == 0) return 0;
    ll sum_L_group = prefix[right + 1] - prefix[left];
    int low = 0, high = (int)all_end.size() - 1;
    int chosen = (int)all_end.size();
    while (low <= high) {
        int mid = low + (high - low) / 2;
        ll vv = all_end[mid];
        auto it = upper_bound(Ls.begin() + left, Ls.begin() + right + 1, vv);
        int numL = it - (Ls.begin() + left);
        int numR = query_count(1, 0, n - 1, left, right, vv);
        if (numL + numR >= N) {
            chosen = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    ll v = all_end[chosen];
    ll sum_small = 0;
    if (chosen == 0) {
        sum_small = N * v;
    } else {
        ll prevv = all_end[chosen - 1];
        auto it = upper_bound(Ls.begin() + left, Ls.begin() + right + 1, prevv);
        int numL_less = it - (Ls.begin() + left);
        int numR_less = query_count(1, 0, n - 1, left, right, prevv);
        int num_less = numL_less + numR_less;
        ll sumL_less = (numL_less > 0 ? prefix[left + numL_less] - prefix[left] : 0);
        ll sumR_less = query_sum(1, 0, n - 1, left, right, prevv);
        ll sum_less = sumL_less + sumR_less;
        int need = N - num_less;
        sum_small = sum_less + need * v;
    }
    return sum_L_group - sum_small;
}

void compute_dp(int kk, int L, int R, int optleft, int optright) {
    if (L > R) return;
    int mid = (L + R) / 2;
    ll best_val = LLONG_MAX / 2;
    int best_j = -1;
    for (int j = max(optleft, 0); j <= min(mid - 1, optright); ++j) {
        ll cost = compute_f(j, mid - 1);
        ll val = dp_prev[j] + cost;
        if (val < best_val) {
            best_val = val;
            best_j = j;
        }
    }
    dp_curr[mid] = best_val;
    compute_dp(kk, L, mid - 1, optleft, best_j);
    compute_dp(kk, mid + 1, R, best_j, optright);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> n >> K;
    vector<ll> x(n), y(n);
    for (int i = 0; i < n; ++i) cin >> x[i];
    for (int i = 0; i < n; ++i) cin >> y[i];
    mice.resize(n);
    for (int i = 0; i < n; ++i) {
        mice[i].L = min(x[i], y[i]);
        mice[i].R = max(x[i], y[i]);
        base += mice[i].R - mice[i].L;
    }
    sort(mice.begin(), mice.end(), [](const Mouse& a, const Mouse& b) { return a.L < b.L; });
    Ls.resize(n);
    for (int i = 0; i < n; ++i) Ls[i] = mice[i].L;
    prefix.resize(n + 1, 0);
    for (int i = 1; i <= n; ++i) prefix[i] = prefix[i - 1] + Ls[i - 1];
    all_end.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        all_end.push_back(mice[i].L);
        all_end.push_back(mice[i].R);
    }
    sort(all_end.begin(), all_end.end());
    all_end.erase(unique(all_end.begin(), all_end.end()), all_end.end());
    tree.resize(4 * n + 10);
    if (n > 0) build(1, 0, n - 1);
    dp_prev.assign(n + 1, LLONG_MAX / 2);
    dp_prev[0] = 0;
    vector<ll> answers(K);
    for (int kk = 1; kk <= K; ++kk) {
        dp_curr.assign(n + 1, LLONG_MAX / 2);
        dp_curr[0] = 0;
        if (n > 0) compute_dp(kk, 1, n, 0, n);
        answers[kk - 1] = base + 2LL * dp_curr[n];
        dp_prev = std::move(dp_curr);
    }
    for (int i = 0; i < K; ++i) {
        if (i > 0) cout << " ";
        cout << answers[i];
    }
    cout << "\n";
    return 0;
}