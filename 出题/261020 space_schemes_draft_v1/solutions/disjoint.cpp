#include <iostream>
using namespace std;

// Partial solution: correct when all simultaneously active requests are disjoint.
// It also solves n=1 and singleton-only subtasks. O(q log n), O(log n) stack.
int blocks(int l, int r, int ql, int qr) {
    if (ql <= l && r <= qr) return 1;
    const int mid = (l + r) / 2;
    int result = 0;
    if (ql <= mid) result += blocks(l, mid, ql, qr);
    if (qr > mid) result += blocks(mid + 1, r, ql, qr);
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, q;
    if (!(cin >> n >> q)) return 0;
    int answer = 0;
    while (q--) {
        char op;
        int l, r;
        cin >> op >> l >> r;
        answer += (op == '+' ? 1 : -1) * blocks(1, n, l, r);
        cout << answer << " 1\n";
    }
}
