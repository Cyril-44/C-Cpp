#include <cstdio>
#include <bitset>
#include <random>
#include <cstdint>
#include <cstring>
constexpr int N = 1000001;
/*
Trick 1: 在环上计算相交弦(u,v)对当前弦(x,y)的异或和贡献，即所有 u<x<v<y 或者 x<u<y<v 的贡献：
    只需要在 BIT 上存 u, v 两个点，区间查询 [x,y] 异或和如果不相交要不然算0次要不然算2次，相交就只算一次。
Trick 2: 判断桶中每个元素是否都为偶数个时（桶必须拆分成若干个桶的情况下）：
    可以对于每一个桶维护个数为奇数的集合，最终将所有分桶做异或并，即数量为偶数舍弃，数量为奇数留存，如果得到空集就是全部为偶数个。
    这里还可以将集合表示为 XOR Hash 形式，记录集合元素权值的 XOR Sum，最终直接做 XOR 合并，如果得到的值为 0（全部抵消）就是偶数个。
*/
bool f[N];
uint64_t g[N];
int n;
void upd(auto tr, int p, auto x) {
    for (; p <= n*2; p += p & -p) tr[p] = tr[p] ^ x;
}
auto sum(auto tr, int p) {
    typename std::decay_t<decltype(tr[0])> res = 0;
    for (; p > 0; p -= p & -p) res ^= tr[p];
    return res;
}
int main() {
    int T;
    scanf("%d", &T);
    std::mt19937 rng(std::random_device{}());
    while (T--) {
        scanf("%d", &n);
        uint64_t hash = 0;
        memset(f, 0, 2*n+1);
        memset(g, 0, sizeof(uint64_t)*(2*n+1));
        for (int i = 1, u, v; i <= n; i++) {
            scanf("%d%d", &u, &v);
            bool fi = sum(f, u) ^ sum(f, v) ^ 1; // 以 i 结尾的链的数量是否为奇数
            uint64_t gi = sum(g, u) ^ sum(g, v) ^ (fi * rng()); // 以 i 结尾的所有链中，弦出现奇数次的集合
            if (fi) upd(f, u, true), upd(f, v, true);
            upd(g, u, gi), upd(g, v, gi);
            hash ^= gi;
            if (hash == 0) putchar('1');
            else putchar('0');
        }
        putchar('\n');
    }
    return 0;
}