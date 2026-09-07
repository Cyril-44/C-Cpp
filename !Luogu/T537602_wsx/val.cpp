#include <iostream>
#include <cassert>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // 默认检查 100% 的数据范围
    int test_idx = 100; 
    if (argc > 1) test_idx = stoi(argv[1]);

    int n, d;
    if (!(cin >> n >> d)) return 1;

    // 根据测试点编号验证不同的 Constraints
    if (test_idx <= 4) { // 前 20%: n = 1
        assert(n == 1);
    } else if (test_idx <= 8) { // 前 40%: n <= 10
        assert(n <= 10);
    }
    
    // 基础范围验证 (100% 数据)
    assert(n >= 1 && n <= 1000);
    assert(d >= 1 && d <= 10000);

    for (int i = 0; i < n; ++i) {
        int a, b, c;
        assert(cin >> a >> b >> c);
        assert(a >= 1 && a <= d);
        assert(b >= 1 && b <= d);
        assert(a + b - 1 <= d);
        assert(c >= 1 && c <= 100000);
        
        // 特殊性质：另外 20% 的数据 c_i = 1 (假设分配在测试点 13-16)
        if (test_idx >= 13 && test_idx <= 16) {
            assert(c == 1);
        }
    }

    for (int i = 0; i < d; ++i) {
        int w;
        assert(cin >> w);
        assert(w >= 1 && w <= 100000);
    }

    return 0;
}