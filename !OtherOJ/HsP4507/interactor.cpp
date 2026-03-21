#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

static const uint64_t ENC_KEY1 = 0x3636c58053a16f91ULL;
static const uint64_t ENC_KEY2 = 0xf315e1c17f266248ULL;

uint64_t scramble(uint64_t x) {
    x ^= ENC_KEY1;
    x *= ENC_KEY2;
    x ^= (x >> 33);
    return x;
}

// 计算某个子任务下，给定 Q 的得分（返回 double，方便直接用于 quitp）
double score_subtask(int sid, long long Q) {
    // 按题面定义 Q'
    auto Qprime = [&](long long base) -> long long {
        return ( (Q / base) + 1 ) * base;
    };

    if (sid == 1) {
        // Subtask 1: N <= 7
        // Q 直接评分
        if (Q <= 50) return 10.0;
        if (Q <= 200) return 7.0;
        if (Q <= 5040) return 4.0;
        return 0.0;
    }

    if (sid == 2) {
        // Subtask 2: N <= 50
        long long Qp = Qprime(100);
        if (Qp <= 400) return 20.0;
        if (Qp <= 700) {
            // 20 -> 15 线性
            // (400,700]: (20-15)*(700-Q')/300 + 15
            return (20.0 - 15.0) * (700.0 - Qp) / 300.0 + 15.0;
        }
        if (Qp <= 1300) {
            // 15 -> 10
            return (15.0 - 10.0) * (1300.0 - Qp) / 600.0 + 10.0;
        }
        if (Qp <= 10000) {
            // 10 -> 3
            return (10.0 - 3.0) * (10000.0 - Qp) / 8700.0 + 3.0;
        }
        return 0.0;
    }

    if (sid == 3) {
        // Subtask 3: N <= 256
        long long Qp = Qprime(200);
        if (Qp <= 2400) return 25.0;
        if (Qp <= 5000) {
            // 25 -> 15
            return (25.0 - 15.0) * (5000.0 - Qp) / 2600.0 + 15.0;
        }
        if (Qp <= 15000) {
            // 15 -> 5
            return (15.0 - 5.0) * (15000.0 - Qp) / 10000.0 + 5.0;
        }
        return 0.0;
    }

    if (sid == 4) {
        // Subtask 4: N <= 2000
        long long Qp = Qprime(500);
        if (Qp <= 20000) return 20.0;
        if (Qp <= 72000) {
            // 20 -> 10
            return (20.0 - 10.0) * (72000.0 - Qp) / 52000.0 + 10.0;
        }
        if (Qp <= 200000) {
            // 10 -> 3
            return (10.0 - 3.0) * (200000.0 - Qp) / 128000.0 + 3.0;
        }
        return 0.0;
    }

    if (sid == 5) {
        // Subtask 5: N <= 5000
        long long Qp = Qprime(1000);
        if (Qp <= 50000) return 25.0;
        if (Qp <= 90000) {
            // 25 -> 15
            return (25.0 - 15.0) * (90000.0 - Qp) / 40000.0 + 15.0;
        }
        if (Qp <= 200000) {
            // 15 -> 5
            return (15.0 - 5.0) * (200000.0 - Qp) / 110000.0 + 5.0;
        }
        return 0.0;
    }

    // 未知子任务 ID
    return 0.0;
}

double max_score_of_subtask(int sid) {
    if (sid == 1) return 10.0;
    if (sid == 2) return 20.0;
    if (sid == 3) return 25.0;
    if (sid == 4) return 20.0;
    if (sid == 5) return 25.0;
    return 0.0;
}

int main(int argc, char* argv[]) {
    registerInteraction(argc, argv);

    // 输入格式：
    // N
    // mode
    // seed
    // subtask_id
    int N = inf.readInt();
    int mode = inf.readInt();
    unsigned long long seed = inf.readUnsignedLong();
    int sid = inf.readInt(1, 5, "subtask_id");

    // 输出给选手
    cout << N << " " << mode << " " << seed << endl;
    cout.flush();

    // 读取选手输出的加密 Q
    uint64_t encQ = ouf.readUnsignedLong();

    // 解密：这里不设显式 Q_limit，而是用一个合理上界扫一遍
    // 上界可以取 1e7 或更大，按你题目最大 Q' 范围来定
    const long long brute_limit = 5000000; // 可按需调大
    long long Q = -1;
    for (long long guess = 0; guess <= brute_limit; guess++) {
        if (scramble(guess) == encQ) {
            Q = guess;
            break;
        }
    }

    if (Q == -1) {
        quitf(_wa, "Failed Hijacked: invalid encrypted Q");
    }

    double sc = score_subtask(sid, Q);
    double full = max_score_of_subtask(sid);

    if (sc <= 0.0) {
        // 这里认为是查询次数过多导致 0 分
        quitf(_wa, "Wrong Answer — Too many queries: Q = %lld", Q);
    }

    if (fabs(sc - full) < 1e-9) {
        // 满分
        quitf(_ok, "Correct: Q = %lld, full score = %.2f", Q, sc);
    } else {
        // 部分分
        double ratio = sc * 100. / full;
        quitp(ratio, "Partial: Q = %lld, score = %.2f / %.2f", Q, sc, full);
    }
}
