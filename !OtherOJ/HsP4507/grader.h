#ifndef LOCAL_GRADER_H
#define LOCAL_GRADER_H

#include <bits/stdc++.h>
using namespace std;

// =====================
// 加密常量（可随意修改）
// =====================
static const uint64_t ENC_KEY1 = 0x3636c58053a16f91ULL;
static const uint64_t ENC_KEY2 = 0xf315e1c17f266248ULL;

// =====================
// 内部状态（混淆）
// =====================
namespace {
    int N_global;
    vector<int> perm;
    long long query_count = 0;

    uint64_t scramble(uint64_t x) {
        x ^= ENC_KEY1;
        x *= ENC_KEY2;
        x ^= (x >> 33);
        return x;
    }

    // 生成排列（与 interactor 保持一致）
    vector<int> generate_perm(int N, int mode, uint64_t seed) {
        vector<int> p(N);
        iota(p.begin(), p.end(), 1);

        uint64_t x = seed ^ (ENC_KEY1 + mode * 1315423911ULL);
        for (int i = N - 1; i > 0; i--) {
            x = scramble(x);
            int j = x % (i + 1);
            swap(p[i], p[j]);
        }
        return p;
    }
}

// =====================
// 选手可调用的 query
// =====================
int query(const vector<int>& q) {
    if ((int)q.size() != N_global) {
        cerr << "Query size mismatch" << endl;
        exit(1);
    }
    query_count++;

    int match = 0;
    for (int i = 0; i < N_global; i++)
        if (q[i] == perm[i]) match++;

    if (match == N_global) {
        // 输出加密后的 Q
        uint64_t enc = scramble(query_count);
        cout << enc << endl;
        exit(0);
    }

    return match;
}

// =====================
// 选手实现的 solve
// =====================
void solve(int N);

// =====================
// 隐藏 main
// =====================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int mode;
    uint64_t seed;
    cin >> N_global >> mode >> seed;

    perm = generate_perm(N_global, mode, seed);

    solve(N_global);

    // 若 solve 未正确结束
    uint64_t enc = scramble(query_count);
    cout << enc << endl;
    return 0;
}

#endif
