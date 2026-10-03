#include <bits/stdc++.h>
using namespace std;

class FastInput {
    static const int S = 1 << 20;
    char buf[S];
    int p = 0, len = 0;

    char get() {
        if (p == len) {
            len = (int)fread(buf, 1, S, stdin);
            p = 0;
            if (!len) return 0;
        }
        return buf[p++];
    }

public:
    int read() {
        char c;
        do {
            c = get();
        } while (c <= ' ' && c);

        int x = 0;
        while (c >= '0' && c <= '9') {
            x = x * 10 + c - '0';
            c = get();
        }
        return x;
    }
};

struct Node {
    long long upper = 0, tag = 0;

    int base = 0;
    int scale = 0;
    int cap = 1;

    int head = -1;
    int degree = 0;

    int parent = 0;
    int size = 1;

    unsigned mask = 0;
    bool active = false;
};

// id 与 id ^ 1 是同一无向边的两个方向。
// prev、next 用于通知桶中的循环双向链表。
// prev == -1 表示当前不在任何通知桶中。
struct Edge {
    long long seen;
    int to, adjNext, prev, next;
};

class Solver {
    // 最大通知间隔为 2^18，共 19 个桶。
    static constexpr int K = 19;

    int n, answer = 0;
    vector<Node> a;
    vector<Edge> edges;
    vector<int> bucket;

    int owner(int id) const {
        return edges[id ^ 1].to;
    }

    void setScale(int x) {
        int d = 2 * a[x].cap;
        unsigned t = (a[x].base + d - 1) / d;

        // 通知间隔取不超过 t 的最大二次幂。
        a[x].scale = 31 - __builtin_clz(t);
    }

    void appendBucket(int id) {
        Edge &e = edges[id];

        int x = owner(id);
        int s = a[e.to].scale;

        // 所有入队记录均从当前累计减权开始计数。
        e.seen = a[x].tag;

        int &h = bucket[x * K + s];

        if (h == -1) {
            h = id;
            e.prev = e.next = id;
            a[x].mask |= 1u << s;
        } else {
            int tail = edges[h].prev;

            e.prev = tail;
            e.next = h;

            edges[tail].next = id;
            edges[h].prev = id;
        }
    }

    void eraseBucket(int id, int s) {
        Edge &e = edges[id];
        if (e.prev == -1) return;

        int x = owner(id);
        int &h = bucket[x * K + s];

        if (e.next == id) {
            h = -1;
            a[x].mask &= ~(1u << s);
        } else {
            edges[e.prev].next = e.next;
            edges[e.next].prev = e.prev;

            if (h == id) h = e.next;
        }

        e.prev = e.next = -1;
    }

    int find(int x) {
        while (a[x].parent != x) {
            a[x].parent = a[a[x].parent].parent;
            x = a[x].parent;
        }
        return x;
    }

    void unite(int x, int y) {
        x = find(x);
        y = find(y);

        if (x == y) return;
        if (a[x].size < a[y].size) swap(x, y);

        a[y].parent = x;
        a[x].size += a[y].size;

        answer = max(answer, a[x].size);
    }

    void activate(int x) {
        a[x].active = true;
        answer = max(answer, 1);

        for (int id = a[x].head; id != -1;
             id = edges[id].adjNext) {
            int y = edges[id].to;

            if (a[y].active) unite(x, y);

            // 删除所有以 x 为目标的通知。
            // x 自己的通知桶仍保留，因为它还能作为操作中心。
            eraseBucket(id ^ 1, a[x].scale);
        }
    }

    void rebuild(int x) {
        int oldScale = a[x].scale;

        // 先结算全部未通知减权，并从旧桶中删除。
        for (int id = a[x].head; id != -1;
             id = edges[id].adjNext) {
            int r = id ^ 1;
            long long t = a[edges[id].to].tag;

            a[x].upper -= t - edges[r].seen;
            edges[r].seen = t;

            eraseBucket(r, oldScale);
        }

        if (a[x].upper <= 0) {
            activate(x);
            return;
        }

        a[x].base = (int)a[x].upper;
        setScale(x);

        // 使用新间隔重新入队。
        for (int id = a[x].head; id != -1;
             id = edges[id].adjNext) {
            appendBucket(id ^ 1);
        }
    }

    // 返回是否已完成死亡处理或阶段重建。
    bool check(int x) {
        if (a[x].upper <= 0) {
            activate(x);
            return true;
        }

        // 间隔为 1 时，通知结算后的 upper 就是真实点权，
        // 不需要继续进行减半重建。
        if (a[x].scale > 0 &&
            a[x].upper <= a[x].base / 2) {
            rebuild(x);
            return true;
        }

        return false;
    }

    void insertEdgeRecord(int id) {
        int y = edges[id].to;
        if (a[y].active) return;

        appendBucket(id);

        if (a[y].degree > a[y].cap) {
            a[y].cap *= 2;

            if (a[y].scale > 0) {
                rebuild(y);
            }
        }
    }

public:
    Solver(int count, int maxEdges)
        : n(count),
          a(count + 1),
          bucket((count + 1) * K, -1) {
        edges.reserve(2 * maxEdges);

        for (int i = 1; i <= n; ++i) {
            a[i].parent = i;
        }
    }

    void setWeight(int x, int w) {
        a[x].upper = a[x].base = w;
    }

    void appendEdge(int x, int y) {
        int id = (int)edges.size();

        // 新边不追溯此前的减权。
        edges.push_back({
            a[x].tag, y, a[x].head, -1, -1
        });
        a[x].head = id;

        edges.push_back({
            a[y].tag, x, a[y].head, -1, -1
        });
        a[y].head = id ^ 1;

        ++a[x].degree;
        ++a[y].degree;
    }

    void initialize() {
        for (int i = 1; i <= n; ++i) {
            while (a[i].cap < a[i].degree) {
                a[i].cap *= 2;
            }
            setScale(i);
        }

        for (int id = 0; id < (int)edges.size(); ++id) {
            appendBucket(id);
        }
    }

    void addEdge(int x, int y) {
        int id = (int)edges.size();
        appendEdge(x, y);

        if (a[x].active && a[y].active) {
            unite(x, y);
        }

        insertEdgeRecord(id);
        insertEdgeRecord(id ^ 1);
    }

    int subtract(int x, int v) {
        // 中心自身单独减权。
        if (!a[x].active) {
            a[x].upper -= v;
            check(x);
        }

        // tag 只表示对邻居的累计减权。
        a[x].tag += v;

        // 处理开始时非空的桶。
        // 本次新入队的通知均严格晚于当前 tag，不必额外扫描。
        unsigned todo = a[x].mask;

        while (todo) {
            int s = __builtin_ctz(todo);
            todo &= todo - 1;

            int index = x * K + s;
            long long step = 1LL << s;

            while (bucket[index] != -1) {
                int id = bucket[index];

                if (edges[id].seen + step > a[x].tag) {
                    break;
                }

                int y = edges[id].to;

                eraseBucket(id, s);

                a[y].upper -= a[x].tag - edges[id].seen;
                edges[id].seen = a[x].tag;

                if (!check(y)) {
                    appendBucket(id);
                }
            }
        }

        return answer;
    }
};

int main() {
    static FastInput in;

    int n = in.read();
    int m = in.read();
    int q = in.read();

    Solver solver(n, m + q);

    for (int i = 1; i <= n; ++i) {
        solver.setWeight(i, in.read());
    }

    for (int i = 0; i < m; ++i) {
        int x = in.read();
        int y = in.read();
        solver.appendEdge(x, y);
    }

    solver.initialize();

    int last = 0;
    string output;
    output.reserve(q * 7);

    for (int i = 0; i < q; ++i) {
        int type = in.read();
        int x = in.read() ^ last;
        int y = in.read() ^ last;

        if (type == 1) {
            solver.addEdge(x, y);
        } else {
            last = solver.subtract(x, y);
            output += to_string(last);
            output += '\n';
        }
    }

    fwrite(output.data(), 1, output.size(), stdout);
    return 0;
}