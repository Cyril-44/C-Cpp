#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

int main() {
    int id, n, m;
    cin >> id >> n >> m;
    vector<ull> fib(n + 2, 0);
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 1;
    for (int i = 3; i <= n + 1; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    struct Node {
        ull sum = 0;
        ull sqsum = 0;
        ull fa = 0;
        ull fb = 0;
        ull lazy_a = 0;
        ull lazy_b = 0;
    };

    vector<Node> tree(4 * (n + 1));

    auto get_s = [&fib](int m) -> ull {
        if (m <= 0) return 0;
        ull f = fib[m + 1];
        ull sq = f * f;
        if (m % 2 == 0) sq -= 1;
        return sq;
    };

    auto push_up = [&tree](int p) {
        tree[p].sum = tree[p * 2].sum + tree[p * 2 + 1].sum;
        tree[p].sqsum = tree[p * 2].sqsum + tree[p * 2 + 1].sqsum;
    };

    auto build = [&] (auto &self, int p, int cl, int cr) -> void {
        tree[p].fa = fib[cl - 1];
        tree[p].fb = fib[cl];
        if (cl == cr) return;
        int mid = (cl + cr) / 2;
        self(self, p * 2, cl, mid);
        self(self, p * 2 + 1, mid + 1, cr);
    };

    build(build, 1, 1, n);

    auto push_down = [&] (int p, int cl, int cr) -> void {
        ull la = tree[p].lazy_a;
        ull lb = tree[p].lazy_b;
        if (la == 0 && lb == 0) return;
        int mid = (cl + cr) / 2;
        int left_len = mid - cl + 1;
        int right_len = cr - mid;
        int l = p * 2, r = p * 2 + 1;

        // left
        ull add_sum_l = fib[left_len + 1] * (la + lb) + fib[left_len] * lb - (la + lb);
        ull old_sum_l = tree[l].sum;
        tree[l].sum = old_sum_l + add_sum_l;
        ull add_sq_l = la * la * fib[left_len - 1] * fib[left_len] + lb * lb * fib[left_len] * fib[left_len + 1] + 2 * la * lb * get_s(left_len - 1);
        tree[l].sqsum = tree[l].sqsum + add_sq_l + 2 * old_sum_l * add_sum_l;
        tree[l].lazy_a += la;
        tree[l].lazy_b += lb;
        tree[l].fa += lb;
        tree[l].fb += la + lb;

        if (right_len > 0) {
            // right
            ull ra = la * fib[left_len - 1] + lb * fib[left_len];
            ull rb = la * fib[left_len] + lb * fib[left_len + 1];
            ull add_sum_r = fib[right_len + 1] * (ra + rb) + fib[right_len] * rb - (ra + rb);
            ull old_sum_r = tree[r].sum;
            tree[r].sum = old_sum_r + add_sum_r;
            ull add_sq_r = ra * ra * fib[right_len - 1] * fib[right_len] + rb * rb * fib[right_len] * fib[right_len + 1] + 2 * ra * rb * get_s(right_len - 1);
            tree[r].sqsum = tree[r].sqsum + add_sq_r + 2 * old_sum_r * add_sum_r;
            tree[r].lazy_a += ra;
            tree[r].lazy_b += rb;
            tree[r].fa += rb;
            tree[r].fb += ra + rb;
        }

        tree[p].lazy_a = 0;
        tree[p].lazy_b = 0;
    };

    auto update = [&] (auto &self, int p, int cl, int cr, int ql, int qr) -> void {
        if (ql <= cl && cr <= qr) {
            ull la = fib[cl - ql];
            ull lb = fib[cl - ql + 1];
            ull len_ = cr - cl + 1;
            ull old_sum = tree[p].sum;
            ull add_sum = fib[len_ + 1] * (la + lb) + fib[len_] * lb - (la + lb);
            tree[p].sum = old_sum + add_sum;
            ull add_sq = la * la * fib[len_ - 1] * fib[len_] + lb * lb * fib[len_] * fib[len_ + 1] + 2 * la * lb * get_s(len_ - 1);
            tree[p].sqsum = tree[p].sqsum + add_sq + 2 * old_sum * add_sum;
            tree[p].lazy_a += la;
            tree[p].lazy_b += lb;
            tree[p].fa += lb;
            tree[p].fb += la + lb;
            return;
        }
        push_down(p, cl, cr);
        int mid = (cl + cr) / 2;
        if (ql <= mid) self(self, p * 2, cl, mid, ql, qr);
        if (qr > mid) self(self, p * 2 + 1, mid + 1, cr, ql, qr);
        push_up(p);
    };

    auto query = [&] (auto &self, int p, int cl, int cr, int ql, int qr) -> ull {
        if (ql <= cl && cr <= qr) return tree[p].sqsum;
        push_down(p, cl, cr);
        int mid = (cl + cr) / 2;
        ull ans = 0;
        if (ql <= mid) ans += self(self, p * 2, cl, mid, ql, qr);
        if (qr > mid) ans += self(self, p * 2 + 1, mid + 1, cr, ql, qr);
        return ans;
    };

    vector<ull> ans_list;
    for (int i = 0; i < m; ++i) {
        int opt, l, r;
        cin >> opt >> l >> r;
        if (opt == 1) {
            update(update, 1, 1, n, l, r);
        } else {
            ull res = query(query, 1, 1, n, l, r);
            ans_list.push_back(res);
        }
    }

    for (auto &res : ans_list) {
        cout << res << endl;
    }
    return 0;
}