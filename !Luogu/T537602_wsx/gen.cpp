#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char* argv[]) {
    int test_idx = (argc > 1) ? stoi(argv[1]) : 20;
    mt19937 rng(time(0) + test_idx);

    int n, d, max_c = 100000;
    
    // 梯度分配
    if (test_idx <= 4) n = 1, d = 10;
    else if (test_idx <= 8) n = 10, d = 100;
    else n = 1000, d = 10000;

    cout << n << " " << d << endl;

    vector<int> target_rows; // 存储消息所在的排，用于构造冲突
    for (int i = 0; i < n; ++i) {
        int a, b, c;
        if (test_idx % 5 == 0) { // 强力构造：极长消息
            a = 1;
            b = d;
        } else {
            a = uniform_int_distribution<int>(1, d)(rng);
            b = uniform_int_distribution<int>(1, d - a + 1)(rng);
        }

        // c 的生成逻辑
        if (test_idx >= 13 && test_idx <= 16) c = 1; // 专项测试
        else c = uniform_int_distribution<int>(1, max_c)(rng);
        
        target_rows.push_back(c);
        cout << a << " " << b << " " << c << "\n";
    }

    // 老师巡逻逻辑：增加“撞车”概率
    for (int i = 1; i <= d; ++i) {
        int w;
        if (test_idx > 10 && (rng() % 100 < 40)) { 
            // 40% 的概率从已有的消息排数中挑选，增加干扰强度
            w = target_rows[rng() % target_rows.size()];
        } else {
            w = uniform_int_distribution<int>(1, max_c)(rng);
        }
        cout << w << (i == d ? "" : " ");
    }
    cout << endl;

    return 0;
}