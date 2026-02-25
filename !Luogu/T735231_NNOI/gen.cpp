#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

long long rnd(long long l, long long r) {
    uniform_int_distribution<long long> dist(l, r);
    return dist(rng);
}

struct P {
    long long w, v;
};

void print_case(const vector<P>& a) {
    int n = (int)a.size();
    cout << n << "\n";
    for (auto &p : a) cout << p.w << " " << p.v << "\n";
}

/*** 模式函数 ***/

vector<P> random_uniform(int n, long long W, long long V) {
    vector<P> a(n);
    for (int i = 0; i < n; ++i) {
        a[i].w = rnd(1, W);
        a[i].v = rnd(1, V);
    }
    return a;
}

vector<P> monotone_chain(int n, long long W, long long V) {
    vector<long long> ws(n), vs(n);
    long long curW = rnd(1, 10), curV = rnd(1, 10);
    for (int i = 0; i < n; ++i) {
        long long dw = rnd(1, max(1LL, W / n));
        long long dv = rnd(1, max(1LL, V / n));
        curW = min(W, curW + dw);
        curV = min(V, curV + dv);
        ws[i] = curW;
        vs[i] = curV;
    }
    // 保证严格递增
    for (int i = 1; i < n; ++i) {
        if (ws[i] <= ws[i-1]) ws[i] = ws[i-1] + 1;
        if (vs[i] <= vs[i-1]) vs[i] = vs[i-1] + 1;
    }
    for (int i = 0; i < n; ++i) {
        ws[i] = min(ws[i], W);
        vs[i] = min(vs[i], V);
    }
    vector<P> a(n);
    for (int i = 0; i < n; ++i) a[i] = {ws[i], vs[i]};
    return a;
}

vector<P> cross_pattern(int n, long long W, long long V) {
    vector<P> a(n);
    long long wStep = max(1LL, W / (long long)n);
    long long vStep = max(1LL, V / (long long)n);
    for (int i = 0; i < n; ++i) {
        long long w = 1 + i * wStep;
        long long v = 1 + (n - 1 - i) * vStep;
        w = min(w, W);
        v = min(v, V);
        // 加一点随机扰动
        w = max(1LL, min(W, w + rnd(-2, 2)));
        v = max(1LL, min(V, v + rnd(-2, 2)));
        a[i] = {w, v};
    }
    return a;
}

vector<P> same_w_random_v(int n, long long W, long long V) {
    vector<P> a(n);
    long long W0 = rnd(1, W);
    for (int i = 0; i < n; ++i) {
        a[i].w = W0;
        a[i].v = rnd(1, V);
    }
    return a;
}

vector<P> same_v_random_w(int n, long long W, long long V) {
    vector<P> a(n);
    long long V0 = rnd(1, V);
    for (int i = 0; i < n; ++i) {
        a[i].v = V0;
        a[i].w = rnd(1, W);
    }
    return a;
}

vector<P> clustered(int n, long long W, long long V, int clusters) {
    vector<P> a;
    a.reserve(n);
    vector<pair<long long,long long>> center;
    for (int c = 0; c < clusters; ++c) {
        long long cw = rnd(1, W);
        long long cv = rnd(1, V);
        center.push_back({cw, cv});
    }
    for (int i = 0; i < n; ++i) {
        auto [cw, cv] = center[rnd(0, clusters-1)];
        long long w = cw + rnd(- (long long)max(1, (int)(W / 1000)), (long long)max(1, (int)(W / 1000)));
        long long v = cv + rnd(- (long long)max(1, (int)(V / 1000)), (long long)max(1, (int)(V / 1000)));
        w = max(1LL, min(W, w));
        v = max(1LL, min(V, v));
        a.push_back({w, v});
    }
    return a;
}

vector<P> duplicate_heavy(int n, long long W, long long V) {
    vector<P> a;
    a.reserve(n);
    int types = min(n, 20); // 20 种点反复出现
    vector<P> base(types);
    for (int i = 0; i < types; ++i) {
        base[i].w = rnd(1, W);
        base[i].v = rnd(1, V);
    }
    for (int i = 0; i < n; ++i) {
        a.push_back(base[rnd(0, types-1)]);
    }
    return a;
}

/*** 按 Subtask / case 组合 ***/

void gen_subtask1(int case_id) {
    int n = 100;
    long long W = 1000, V = 1000;
    vector<P> a;
    if (case_id == 1) a = random_uniform(n, W, V);
    else if (case_id == 2) a = cross_pattern(n, W, V);
    else if (case_id == 3) a = same_w_random_v(n, W, V);
    else if (case_id == 4) a = same_v_random_w(n, W, V);
    else a = monotone_chain(n, W, V);
    print_case(a);
}

void gen_subtask2(int case_id) {
    int n = 1000;
    long long W = 1e6, V = 1e6;
    vector<P> a;
    if (case_id == 1) a = random_uniform(n, W, V);
    else if (case_id == 2) a = cross_pattern(n, W, V);
    else if (case_id == 3) a = clustered(n, W, V, 5);
    else if (case_id == 4) a = duplicate_heavy(n, 100, 100);
    else a = same_w_random_v(n, W, V);
    print_case(a);
}

void gen_subtask3(int case_id) {
    int n = 5000;
    long long W = 1e7, V = 1e7;
    vector<P> a;
    if (case_id == 1) a = random_uniform(n, W, V);
    else if (case_id == 2) a = cross_pattern(n, W, V);
    else if (case_id == 3) a = clustered(n, W, V, 8);
    else if (case_id == 4) a = duplicate_heavy(n, 1000, 1000);
    else a = same_v_random_w(n, W, V);
    print_case(a);
}

void gen_subtask4(int case_id) {
    int n = 200000;
    long long W = 1e9, V = 1e9;
    vector<P> a = monotone_chain(n, W, V);
    print_case(a);
}

void gen_subtask5(int case_id) {
    int n = 200000;
    long long W = 100, V = 100;
    vector<P> a;
    if (case_id == 1) a = random_uniform(n, W, V);
    else if (case_id == 2) a = duplicate_heavy(n, W, V);
    else if (case_id == 3) a = clustered(n, W, V, 5);
    else if (case_id == 4) a = same_w_random_v(n, W, V);
    else a = same_v_random_w(n, W, V);
    print_case(a);
}

void gen_subtask6(int case_id) {
    int n = 200000;
    long long W = 100000, V = 100000;
    vector<P> a;
    if (case_id == 1) a = random_uniform(n, W, V);
    else if (case_id == 2) a = cross_pattern(n, W, V);
    else if (case_id == 3) a = clustered(n, W, V, 10);
    else if (case_id == 4) a = duplicate_heavy(n, 1000, 1000);
    else a = same_w_random_v(n, W, V);
    print_case(a);
}

void gen_subtask7(int case_id) {
    int n = 200000;
    long long W = 1000000000LL, V = 1000000000LL;
    vector<P> a;
    if (case_id == 1) a = random_uniform(n, W, V);
    else if (case_id == 2) a = cross_pattern(n, W, V);
    else if (case_id == 3) a = clustered(n, W, V, 12);
    else if (case_id == 4) a = duplicate_heavy(n, 100000, 100000);
    else if (case_id == 5) a = same_w_random_v(n, W, V);
    else a = same_v_random_w(n, W, V);
    print_case(a);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        cerr << "Usage: ./gen <subtask_id> <case_id>\n";
        return 0;
    }
    int sub = atoi(argv[1]);
    int cid = atoi(argv[2]);
    // 可选：用 (sub, cid) 固定 seed，保证可复现
    rng.seed(((uint64_t)sub << 32) ^ (uint64_t)cid ^ chrono::steady_clock::now().time_since_epoch().count());

    switch (sub) {
        case 1: gen_subtask1(cid); break;
        case 2: gen_subtask2(cid); break;
        case 3: gen_subtask3(cid); break;
        case 4: gen_subtask4(cid); break;
        case 5: gen_subtask5(cid); break;
        case 6: gen_subtask6(cid); break;
        case 7: gen_subtask7(cid); break;
        default: gen_subtask7(cid); break;
    }
    return 0;
}
