#include <cstdio>
#include <cstring>
#include <cmath>
#ifdef CLANGD
constexpr int N = 15;
#else
constexpr int N = 100005;
#endif
/* 将原题转化为子问题（只有一个白点其余都是黑点）的集合，
   对于每一个子问题其 SG 函数值为其所有后继状态的 mex，
   一个问题先手是否必败转化为求子问题 SG 函数值异或和是否为 0
 * 于是本题的 sg[i] = mex(0, sg[2*i], sg[2*i] ^ sg[3*i], sg[2*i] ^ sg[3*i] ^ sg[4*i], ...)
   最后对于每一组询问求解异或和即可。
 * 发现 sg[i] 的值仅仅与 n / i 有关。
   考虑整除分块，块内值都是相同的。
 * 计算块内值，再嵌套一层整除分块，因为 n / (i*j) = (n/i) / j，所以对 n/i 做整除分块就好了。
   最终时间复杂度为 O(n^(3/4)) 可以当结论记下来。
 */
int st[N]; // Trick 1: 求 mex 的时候使用标识符判断是否属于当前集合，而无需清空整个 set.
struct Block {
    int l, r, val;
} blk[N]; // 整除分块
int top;
int n, k, sqrtn;
int sgdat[2][N]; // Trick 2: 分块后，对于 x <= sqrt(n) 的部分直接存在 sg[0][x] 中，对于 x > sqrt(n) 的部分存在 sg[n / x] 中
inline int sg(int x, int nx) { return x <= sqrtn ? sgdat[0][x] : sgdat[1][nx]; }
inline int sg(int x) { return sg(x, n / x); }
inline void init() {
    for (int l = 1, r; l <= n; l = r + 1) {
        int v = n / l;
        r = n / v;
        blk[++top] = {l, r, v};
    }
    for (int i = top; i >= 1; i--) {
        int xorpre = 0;
        for (int l = 1, r; l <= blk[i].val; l = r + 1) {
            int v = blk[i].val / l;
            r = blk[i].val / v;
            int now = sg(l * blk[i].l, v);
            st[xorpre ^ now] = i;
            if (r - l & 1 ^ 1) xorpre ^= now;
        }
        int sgv = 1;
        while (st[sgv] == i) ++sgv;
        if (blk[i].val <= sqrtn) sgdat[1][blk[i].val] = sgv;
        for (int j = blk[i].l; j <= blk[i].r && j <= sqrtn; j++)
            sgdat[0][j] = sgv;
    }
}
int main() {
    scanf("%d%d", &n, &k);
    sqrtn = sqrt(n);
    init();
    for (int w; k--; ) {
        scanf("%d", &w);
        int xorsum = 0;
        for (int p; w--; ) scanf("%d", &p), xorsum ^= sg(p);
        puts(xorsum ? "Yes" : "No");
    }
    return 0;
}