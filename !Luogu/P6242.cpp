#include <stdio.h>
#include <limits>
#include <algorithm>
constexpr int N = 500005;
#define umax(x, y) ((x) = std::max((x), (y)))
namespace SegTr {
struct Node {
    int mx, hmx; // hmx
// 下面全是懒标记
    int add, hadd; // hadd: 从上次pushdown到这次，最大的 add
    bool covered; // 是否被赋值过（即当前区间是否值全部相等）
    int cov, hcov; // hcov: 同理 最大的赋值
    inline void pulladd(int tadd, int thadd) {
        umax(hmx, mx + thadd), mx += tadd;f 
        mx += tadd;
        if (covered) // 直接区间一起加
            hmax(hcov, cov + thadd), cov += tadd;
        else
            hmax(hadd, add + thadd), add += tadd;
    }
    inline void pullcov(const Node &u) {
        umax(hmx, u.hcov), mx = u.cov;
        umax(hcov, u.hcov), cov = u.cov, covered = true;
    }
} tr[N << 2];
inline void pushdown(int u) {
    if (tr[u].add) { // 默认先加再赋值
        tr[u<<1].pulladd(tr[u]);
        tr[u<<1|1].pulladd(tr[u]);
        tr[u].add = tr[u].hadd = 0;
    }
    if (tr[u].covered) {
        
        tr[u<<1].pullcov(tr[u]);
        tr[u<<1|1].pullcov(tr[u]);
        tr[u].covered = false;
        tr[u].hcov = std::numeric_limits<int>::min();
    }
}
inline void pushup(*int u) {
    
}
}

int main() {
    
    return 0;
}