#include "testlib.h"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;
using Interval = pair<int, int>;

static uint64_t key(Interval p) {
    return (uint64_t(p.first) << 32) | uint32_t(p.second);
}

struct Operation {
    char type;
    Interval interval;
};

class Sequence {
    int limit;
    vector<Interval> active;
    unordered_map<uint64_t, size_t> index;

public:
    vector<Operation> ops;

    explicit Sequence(int q) : limit(q) {
        ops.reserve(q);
        active.reserve(q);
        index.reserve(q);
    }

    bool full() const { return int(ops.size()) == limit; }
    bool empty() const { return active.empty(); }
    bool contains(Interval p) const { return index.count(key(p)) != 0; }

    bool add(Interval p) {
        if (full() || !index.emplace(key(p), active.size()).second) return false;
        active.push_back(p);
        ops.push_back({'+', p});
        return true;
    }

    bool remove(Interval p) {
        if (full()) return false;
        auto it = index.find(key(p));
        if (it == index.end()) return false;
        const size_t pos = it->second;
        index.erase(it);
        if (pos + 1 != active.size()) {
            active[pos] = active.back();
            index.at(key(active[pos])) = pos;
        }
        active.pop_back();
        ops.push_back({'-', p});
        return true;
    }

    void toggle(Interval p) {
        if (contains(p)) remove(p);
        else add(p);
    }

    void remove_random() {
        ensuref(!empty(), "No active interval to remove");
        remove(active[rnd.next(int(active.size()))]);
    }
};

static void build(int l, int r, vector<Interval>& nodes) {
    nodes.emplace_back(l, r);
    if (l == r) return;
    int mid = (l + r) / 2;
    build(l, mid, nodes);
    build(mid + 1, r, nodes);
}

static Interval choose(const vector<Interval>& pool) {
    return pool[rnd.next(int(pool.size()))];
}

int main(int argc, char** argv) {
    registerGen(argc, argv, 1);
    const int tid = opt<int>("tid");
    const int n = opt<int>("n");
    const int q = opt<int>("q");
    const string mode = opt<string>("mode");
    const auto seed = opt<long long>("seed");
    rnd.setSeed(seed);
    ensuref(1 <= tid && tid <= 10, "tid outside limits");
    ensuref(1 <= n && n <= 100000 && 1 <= q && q <= 300000, "n or q outside limits");
    Sequence out(q);
    vector<Interval> nodes;
    nodes.reserve(2 * n - 1);
    build(1, n, nodes);

    auto singleton = [&]() -> Interval {
        int p = rnd.next(1, n);
        return {p, p};
    };
    auto random_interval = [&]() -> Interval {
        switch (rnd.next(12)) {
            case 0: return {1, n};
            case 1: return singleton();
            case 2: return {1, rnd.next(1, n)};
            case 3: return {rnd.next(1, n), n};
            case 4: return choose(nodes);
            case 5: {
                int l = rnd.next(1, n);
                return {l, min(n, l + rnd.next(32))};
            }
            default: {
                int l = rnd.next(1, n), r = rnd.next(1, n);
                return {min(l, r), max(l, r)};
            }
        }
    };
    auto fill = [&](const function<Interval()>& candidate, int add_percent = 60, bool insert_only = false) {
        int next_l = 1, next_r = 1;
        while (!out.full()) {
            if (!insert_only && !out.empty() && rnd.next(100) >= add_percent) {
                out.remove_random();
                continue;
            }
            bool added = false;
            for (int attempt = 0; attempt < 40 && !added; ++attempt) added = out.add(candidate());
            if (added) continue;
            if (!insert_only) {
                out.remove_random();
                continue;
            }
            // With insertions only, previously scanned intervals stay active.
            while (next_l <= n && out.contains({next_l, next_r})) {
                if (++next_r > n) next_r = ++next_l;
            }
            ensuref(next_l <= n, "Not enough distinct intervals");
            out.add({next_l, next_r});
        }
    };
    auto ties = [&](int cap) {
        for (auto [l, r] : nodes) {
            if (int(out.ops.size()) >= cap || out.full()) break;
            if (r == l + 1) {
                out.add({l, r});
                out.add({l, l});
            }
        }
    };
    auto waves = [&](vector<Interval> pool) {
        while (!out.full()) {
            shuffle(pool.begin(), pool.end());
            for (auto p : pool) { if (out.full()) break; out.add(p); }
            shuffle(pool.begin(), pool.end());
            for (auto p : pool) { if (out.full()) break; out.remove(p); }
        }
    };

    if (mode == "gray") {
        ensuref(n <= 5, "gray requires n <= 5");
        vector<Interval> pool;
        for (int l = 1; l <= n; ++l)
            for (int r = l; r <= n; ++r) pool.emplace_back(l, r);
        int previous = 0;
        for (int i = 1; i <= q; ++i) {
            const int step = i % (1 << pool.size());
            const int mask = step ^ (step >> 1);
            out.toggle(pool[__builtin_ctz(unsigned(mask ^ previous))]);
            previous = mask;
        }
    } else if (mode == "singletons") {
        fill(singleton);
    } else if (mode == "whole-singleton") {
        out.add({1, n});
        vector<int> points(n);
        iota(points.begin(), points.end(), 1);
        shuffle(points.begin(), points.end());
        for (int p : points) out.add({p, p});
        for (int p = 1; p <= n; ++p) out.remove({p, p});
        while (!out.full()) out.toggle(rnd.next(25) ? singleton() : Interval{1, n});
    } else if (mode == "short") {
        ties(q);
        fill([&]() -> Interval {
            int l = rnd.next(1, n);
            return {l, min(n, l + rnd.next(2))};
        });
    } else if (mode == "node-only" || mode == "hierarchy") {
        if (mode == "hierarchy") for (auto p : nodes) { if (out.full()) break; out.add(p); }
        fill([&]() { return choose(nodes); });
    } else if (mode == "insert-only") {
        ensuref(q <= 1LL * n * (n + 1) / 2, "Too many distinct insertions");
        fill(random_interval, 100, true);
    } else if (mode == "disjoint") {
        vector<Interval> zones;
        for (int l = 1; l <= n; ) {
            int r = min(n, l + rnd.next(0, min(n, 193)));
            zones.emplace_back(l, r);
            l = r + 1;
        }
        vector<Interval> current(zones.size());
        while (!out.full()) {
            size_t j = rnd.next(int(zones.size()));
            if (current[j].first) {
                out.remove(current[j]);
                current[j] = {};
            } else {
                auto [l, r] = zones[j];
                int a = rnd.next(l, r), b = rnd.next(l, r);
                current[j] = {min(a, b), max(a, b)};
                out.add(current[j]);
            }
        }
    } else if (mode == "ties") {
        ties(q);
        vector<Interval> cherries;
        for (auto p : nodes) if (p.second == p.first + 1) cherries.push_back(p);
        if (cherries.empty()) fill(singleton);
        while (!out.full()) {
            auto [l, r] = choose(cherries);
            int x = rnd.next(l, r);
            out.toggle({x, x});
        }
    } else if (mode == "prefix-wave") {
        vector<Interval> pool;
        for (int r = 1; r <= n; ++r) pool.emplace_back(1, r);
        waves(pool);
    } else if (mode == "nested-path") {
        vector<Interval> path;
        int l = 1, r = n;
        while (true) {
            path.emplace_back(l, r);
            if (l == r) break;
            int mid = (l + r) / 2;
            if (rnd.next(2)) l = mid + 1;
            else r = mid;
        }
        waves(path);
    } else if (mode == "boundary" || mode == "shared") {
        vector<Interval> pool;
        if (mode == "boundary") {
            pool = {{1, n}, {1, 1}, {n, n}};
            for (int p : {2, 3, n / 2, (n + 1) / 2, n - 1, n - 2}) {
                if (p < 1 || p > n) continue;
                pool.insert(pool.end(), {{1, p}, {p, n}, {p, p}});
                if (p < n) pool.emplace_back(p, p + 1);
            }
        } else {
            const int mid = (n + 1) / 2;
            for (int r = mid; r <= n; ++r) pool.emplace_back(1, r);
            for (int l = 1; l <= mid; ++l) pool.emplace_back(l, n);
        }
        sort(pool.begin(), pool.end());
        pool.erase(unique(pool.begin(), pool.end()), pool.end());
        if (mode == "shared") shuffle(pool.begin(), pool.end());
        for (auto p : pool) { if (out.full()) break; out.add(p); }
        fill([&]() { return choose(pool); }, mode == "shared" ? 45 : 60);
    } else if (mode == "mixed") {
        out.add({1, n});
        ties(q / 4);
        vector<int> endpoints(n - (n + 1) / 2 + 1);
        iota(endpoints.begin(), endpoints.end(), (n + 1) / 2);
        shuffle(endpoints.begin(), endpoints.end());
        for (int r : endpoints) {
            if (int(out.ops.size()) >= q / 2 || out.full()) break;
            out.add({1, r});
        }
        fill(random_interval, 48);
    } else {
        ensuref(mode == "random", "Unknown mode: %s", mode.c_str());
        fill(random_interval);
    }

    ensuref(out.full(), "Wrong number of operations");
    ios::sync_with_stdio(false);
    cout << tid << ' ' << n << ' ' << q << '\n';
    for (auto [op, p] : out.ops) cout << op << ' ' << p.first << ' ' << p.second << '\n';
}
