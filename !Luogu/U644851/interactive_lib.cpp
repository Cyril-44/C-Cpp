#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

// =============================
// 你要实现的函数
// =============================
extern int myatof(char* s, double* vec);

// =============================
// 生成一个随机浮点数字符串
// =============================
static inline string gen_float(random_t& rnd) {
    int L = rnd.next(0, 5);
    int R = rnd.next(0, 5);

    string s;

    if (L > 0) {
        s += char('1' + rnd.next(0, 8));
        for (int i = 1; i < L; i++) s += char('1' + rnd.next(0, 8));
    }

    if (R > 0) {
        s += '.';
        for (int i = 0; i < R; i++) s += char('1' + rnd.next(0, 8));
    }

    if (L == 0 && R == 0) {
        s = ".";
        s += char('1' + rnd.next(0, 8));
    }

    return s;
}

// =============================
// 流式生成大输入
// =============================
static inline void generate_stream_input(
    uint64_t total_len,
    FILE* fp
) {
    uint64_t cur = 0;

    while (cur + 20 < total_len) {
        string f = gen_float(rnd);
        if (cur + f.size() + 1 > total_len) break;
        fwrite(f.data(), 1, f.size(), fp);
        fputc(',', fp);
        cur += f.size() + 1;
    }

    string f = gen_float(rnd);
    fwrite(f.data(), 1, f.size(), fp);
    fputc(';', fp);
}

// =============================
// 用 strtod 流式解析 ground truth
// =============================
static inline vector<double> parse_truth(const string& s) {
    vector<double> v;
    const char* p = s.c_str();
    char* endp;

    while (*p) {
        double x = strtod(p, &endp);
        v.push_back(x);
        if (*endp == ';') break;
        p = endp + 1;
    }
    return v;
}

// =============================
// 主交互逻辑（testlib）
// =============================
int main(int argc, char* argv[]) {
    registerInteraction(argc, argv);

    uint64_t total_len = inf.readLong();
    uint64_t seed      = inf.readLong();
    rnd.setSeed(seed);

    // 临时文件保存生成的输入
    FILE* fp = tmpfile();
    if (!fp) quitf(_fail, "tmpfile failed");

    generate_stream_input(total_len, fp);

    // 读取整个输入到内存（你可以改成 mmap）
    fseek(fp, 0, SEEK_END);
    size_t n = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    string s;
    s.resize(n);
    fread(&s[0], 1, n, fp);
    fclose(fp);

    // 解析 ground truth
    vector<double> truth = parse_truth(s);

    // 调用你的 myatof
    vector<double> buf(truth.size() + 10);

    auto t1 = chrono::high_resolution_clock::now();
    int cnt = myatof(s.data(), buf.data());
    auto t2 = chrono::high_resolution_clock::now();

    double ms = chrono::duration<double, milli>(t2 - t1).count();

    if (cnt != (int)truth.size()) {
        quitf(_wa, "count mismatch: my=%d truth=%d", cnt, (int)truth.size());
    }

    for (int i = 0; i < cnt; i++) {
        if (fabs(buf[i] - truth[i]) > 1e-6) {
            quitf(_wa,
                  "value mismatch at %d: my=%.10f truth=%.10f",
                  i, buf[i], truth[i]);
        }
    }

    quitf(_ok, "OK, parsed %d floats in %.2f ms", cnt, ms);
}
