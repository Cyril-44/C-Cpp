#include "testlib.h"
#include <vector>
#include <unordered_set>

using namespace std;

int main(int argc, char* argv[]) {
    // 初始化 checker，必须在最前面调用一次
    setName("check array: distinct, in [1, 1e8], pairwise XOR distinct");
    registerTestlibCmd(argc, argv);

    // ---------- 从输入文件读取 n ----------
    int n = inf.readInt();

    // ---------- 从选手输出读取 n 个整数 ----------
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        a[i] = ouf.readInt();
    }

    // 检查是否有多余输出（忽略空白后仍有内容则报格式错误）
    if (!ouf.seekEof()) {
        quitf(_pe, "extra output found after %d integers", n);
    }

    // ---------- 条件 1: 正整数，值域 [1, 1e8] ----------
    const int MAX_VAL = 100000000;
    for (int i = 0; i < n; ++i) {
        if (a[i] < 0 || a[i] >= MAX_VAL) {
            quitf(_wa, "a[%d] = %d is out of range [1, %d]", i + 1, a[i], MAX_VAL);
        }
    }

    // ---------- 条件 2: 所有数互不相同 ----------
    unordered_set<int> distinct_set;
    distinct_set.reserve(n * 2);
    for (int x : a) {
        if (!distinct_set.insert(x).second) {
            quitf(_wa, "duplicate value found: %d", x);
        }
    }

    // ---------- 条件 3: 两两异或值各不相同 ----------
    // 值域 [1, 1e8] 下，异或结果 < 2^27 = 134217728
    const int XOR_MAX = 1 << 27;
    vector<bool> seen(XOR_MAX, false);

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int x = a[i] ^ a[j];
            if (seen[x]) {
                quitf(_wa, "duplicate XOR value %d at a[%d]^a[%d]", x, i + 1, j + 1);
            }
            seen[x] = true;
        }
    }

    quitf(_ok, "correct: %d integers, all distinct, all pairwise XORs distinct", n);
    return 0;
}