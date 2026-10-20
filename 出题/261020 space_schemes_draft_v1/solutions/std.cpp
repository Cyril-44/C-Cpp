#include <algorithm>
#include <cstdio>
#include <vector>

// C++17. O(n + q log n) time, O(n) memory.
namespace {
constexpr int MOD = 998244353;

class FastInput {
    char buf[1 << 16];
    int at = 0, size = 0;
public:
    int get() {
        if (at == size) {
            size = static_cast<int>(std::fread(buf, 1, sizeof(buf), stdin));
            at = 0;
            if (!size) return EOF;
        }
        return buf[at++];
    }
    int integer() {
        int c = get();
        while (c != EOF && (c < '0' || c > '9')) c = get();
        int value = 0;
        while (c >= '0' && c <= '9') {
            value = value * 10 + c - '0';
            c = get();
        }
        return value;
    }
    char operation() {
        int c = get();
        while (c != EOF && c != '+' && c != '-') c = get();
        return static_cast<char>(c);
    }
};

class FastOutput {
    char buf[1 << 16];
    int size = 0;
public:
    ~FastOutput() { flush(); }
    void flush() {
        std::fwrite(buf, 1, size, stdout);
        size = 0;
    }
    void put(char c) {
        if (size == static_cast<int>(sizeof(buf))) flush();
        buf[size++] = c;
    }
    void integer(int value, char end) {
        char digits[12];
        int count = 0;
        do { digits[count++] = static_cast<char>('0' + value % 10); value /= 10; }
        while (value);
        while (count) put(digits[--count]);
        put(end);
    }
};

struct Node {
    int cnt = 0;
    int a_cost = 0, a_ways = 1;
    int b_cost = 1, b_ways = 1;
};

class Solver {
    std::vector<Node> tree;
    void pull(int p, int l, int r) {
        Node &v = tree[p];
        if (l == r) {
            v.a_cost = v.cnt > 0;
            v.a_ways = 1;
            v.b_cost = v.b_ways = 1;
            return;
        }
        const Node &left = tree[p * 2], &right = tree[p * 2 + 1];
        const int base_cost = left.a_cost + right.a_cost;
        const int base_ways = 1LL * left.a_ways * right.a_ways % MOD;
        const int keep_cost = base_cost + 1;
        const int split_cost = left.b_cost + right.b_cost;
        const int split_ways = 1LL * left.b_ways * right.b_ways % MOD;
        v.b_cost = std::min(keep_cost, split_cost);
        if (keep_cost < split_cost) v.b_ways = base_ways;
        else if (split_cost < keep_cost) v.b_ways = split_ways;
        else {
            v.b_ways = base_ways + split_ways;
            if (v.b_ways >= MOD) v.b_ways -= MOD;
        }
        if (v.cnt > 0) {
            v.a_cost = v.b_cost;
            v.a_ways = v.b_ways;
        } else {
            v.a_cost = base_cost;
            v.a_ways = base_ways;
        }
    }
public:
    explicit Solver(int n) : tree(4 * n + 8) {}
    void update(int p, int l, int r, int ql, int qr, int delta) {
        if (ql <= l && r <= qr) tree[p].cnt += delta;
        else {
            int mid = (l + r) / 2;
            if (ql <= mid) update(p * 2, l, mid, ql, qr, delta);
            if (qr > mid) update(p * 2 + 1, mid + 1, r, ql, qr, delta);
        }
        pull(p, l, r);
    }
    const Node &root() const { return tree[1]; }
};
}

int main() {
    FastInput input;
    FastOutput output;
    int n = input.integer(), q = input.integer();
    Solver solver(n);
    for (int i = 0; i < q; ++i) {
        char op = input.operation();
        int l = input.integer(), r = input.integer();
        solver.update(1, 1, n, l, r, op == '+' ? 1 : -1);
        output.integer(solver.root().a_cost, ' ');
        output.integer(solver.root().a_ways, '\n');
    }
}
