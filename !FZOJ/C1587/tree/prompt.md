我将会给你一道 OI 题的题面，我的思路，代码以及错误数据。帮我查一查，这份代码中有什么核心逻辑上的问题导致输出的答案不正确。不考虑鲁棒性方面的内容。
```markdown
## Tree 题意

给定一个基环树：

* 环大小为 (t)，环节点编号 (1\sim t)，按顺序组成环。
* 其余节点 (t+1\sim n)，每个节点有父亲 (f_i)，且 (f_i<i)。
* 两点距离定义为环上的最短路径距离。
* 每个点黑/白。

删除一个黑点 (x)。

对于一个白点 (y)：

如果 (x) 是距离 (y) 最远的黑点之一：

[
dist(x,y)=\max_{黑点 z} dist(z,y)
]

那么 (y) 给 (x) 贡献 1。

要求：

[
\max_x \sum_{白点 y}[x\text{ 是 }y\text{的最远黑点}]
]

输出这个最大贡献。

---

## 样例输入
```
#1
1
6 3
3 4 2
1 0 0 1 0 1
```

## 样例输出
```
#1
2
```

## 样例解释

无

---

## 数据范围（Hint）

| 测试点编号 | $n \le$        | $t$      |
|-----------|----------------|----------|
| 1 ~ 2     | $10^3$         | $\le n$  |
| 3 ~ 5     | $2 \times 10^5$ | $=1$     |
| 6 ~ 7     | $2 \times 10^5$ | $\le 50$ |
| 8 ~ 10    | $2 \times 10^5$ | $\le n$  |
```
思路：对于每个白点，将其对应的最远的黑点们 value += 1，取 max 黑点 value 即为答案。
代码：
```cpp
#include <bits/stdc++.h>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
constexpr int N = 200005;
static int n, t, a[N];
static std::vector<int> g[N];
static int from[N]; // fr[i]: i 属于环上哪一个子树
static int trsz[N]; // trsz[i]: 环上第 i 个子树大小
static int depth[N]; // dep[i]: i 在自己的树中的深度 (0-rooted)
static int mxDep[N]; // mxDep[i]: 环上第 i 个子树中最深的黑点的深度
static int mxExDis[N]; // mxExDis[i]: 环上点 i 经过环边到别的子树中黑点的最远距离
static int arrL[N<<1], arrR[N<<1]; // arr: 当前经过等差数列修补的 mxDep
static bool flgL[N<<1], flgR[N<<1]; // flg: 能作为备选贡献的 mxDep
static std::pair<int,int> covR[N], covL[N]; // cov: 在 i 下，能作为备选贡献的 mxDep 为 cov[i].first ~ cov[i].second
static int mxDis[N]; // mxDis[i]: 点 i 能到达最远的黑点的距离
static int disL[N<<1], disR[N<<1];
static int sumL[N<<1], sumR[N<<1];
static int value[N];
static int currentRing;
inline void umax(int &x, int y) { if (x < y) x = y; }
namespace PntDivide {
    bool vis[N];
    int sz[N], dep[N], hson[N];
    void dfssz(int u, int fa = 0) {
        sz[u] = 1, hson[u] = 0;
        for (int v : g[u]) if (v != fa && !vis[v]) {
            dfssz(v, u), sz[u] += sz[v];
            if (sz[v] > sz[hson[u]]) hson[u] = v;
        }
    }
    int mxdep, centroid[N];
    std::vector<int> nodes;
    void dfs3(int u, int fa = 0) {
        dep[u] = dep[fa] + 1;
        nodes.push_back(u);
        if (a[u]) umax(mxdep, dep[u]);
        for (int v : g[u]) if (v != fa && !vis[v]) dfs3(v, u);
    }
    void dfs1(int u) {
        dfssz(u);
        int cursz = sz[u], _u = u;
        while (std::max(cursz - sz[u], sz[hson[u]]) > cursz/2) u = hson[u];
        centroid[_u] = u, vis[u] = true;
        std::vector<int> sons, mxdeps;
        std::vector<std::vector<int>> subtrs;
        sons.reserve(g[u].size()), mxdeps.reserve(g[u].size()), subtrs.reserve(g[u].size());
        int mxid = -1, mx2id = -1;
        for (int v : g[u]) if (!vis[v]) {
            int idx = sons.size();
            mxdep = -1, dfs3(v), subtrs.emplace_back().swap(nodes);
            if (mxid == -1 || mxdep >= mxdeps[mxid]) mx2id = mxid, mxid = idx;
            else if (mx2id == -1 || mxdep >= mxdeps[mx2id]) mx2id = idx;
            sons.push_back(v), mxdeps.push_back(mxdep);
        }
        if (mxid == -1) return;
        if (!a[u]) umax(mxDis[u], mxdeps[mxid]);
        else for (const auto &subtr : subtrs) for (int v : subtr) if (!a[v]) umax(mxDis[v], dep[v]);
        if (mx2id == -1) return;
        for (int i = 0; i < sons.size(); i++) {
            int dis = i == mxid ? mxdeps[mx2id] : mxdeps[mxid];
            for (int vv : subtrs[i]) if (!a[vv])
                umax(mxDis[vv], dep[vv] + dis);
        }
        for (int v : sons) dfs1(v);
    }
    void dfs2(int u) {
        u = centroid[u];
        vis[u] = true;
        std::vector<int> sons, mxids, mx2ids, mxdeps;
        int mxv = -1, mx2v = -1;
        std::vector<std::vector<int>> subtrs;
        sons.reserve(g[u].size()), subtrs.reserve(g[u].size());
        for (int v : g[u]) if (!vis[v]) {
            int idx = sons.size();
            mxdep = -1, dfs3(v), subtrs.emplace_back().swap(nodes);
            if (mxdep > mxv) mx2v = mxv, mx2ids = mxids, mxv = mxdep, mxids = {idx};
            else if (mxdep == mxv) mxids.push_back(idx);
            else if (mxdep > mx2v) mx2v = mxv, mx2ids = {idx};
            else if (mxdep == mx2v) mx2ids.push_back(idx);
            sons.push_back(v), mxdeps.push_back(mxdep);
        }
        if (mxids.empty()) return;
        int add = 0, add2 = 0; // 在 mxids, mx2ids 上加的整体贡献次数
        std::vector<int> subs(sons.size()); // 减去环上某些子树的贡献
        if (a[u]) { // 处理子树白点到根黑点 
            for (const auto &subtr : subtrs) for (int v : subtr) if (dep[v] == mxDis[v]) ++value[u];
        }
        for (int i = 0; i < sons.size(); i++) {
            int dis = mxids.size() == 1 && mxv == mxdeps[i] ? mx2v : mxv;
            for (int vv : subtrs[i]) if (!a[vv])
                if (mxDis[vv] == dep[vv] + dis) {
                    if (mxdeps[i] == mxv) {
                        if (mxids.size() == 1) add2++;
                        else add++, subs[i]++;
                    }
                    else add++;
                }
        }
        for (int i : mxids) for (int v : subtrs[i]) if (a[v] && dep[v] == mxdeps[i]) value[v] += add - subs[i];
        for (int i : mx2ids) for (int v : subtrs[i]) if (a[v] && dep[v] == mxdeps[i]) value[v] += add2;
        for (int v : g[u]) if (!vis[v]) dfs2(v);
    }
    inline void work1() {
        memset(vis, 0, n+1);
        For(i, 1, t) dfs1(i);
    }
    inline void work2() {
        memset(vis, 0, n+1);
        For(i, 1, t) dfs2(i);
    }
}
std::vector<int> mxDepNodes[N]; // mxDepNodes[i]: 在环 i 子树中，depth = mxDep 的黑点
void dfs1(int u, int fa = 0) {
    if (a[u]) umax(mxDep[currentRing], depth[u]);
    for (int v : g[u]) if (v != fa) depth[v] = depth[u] + 1, dfs1(v, u);
}
int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        // 将每个白点对应的最远的黑点 +1，取 max value of 黑点
        scanf("%d%d", &n, &t);
        For(i, 1, n) g[i].clear();
        std::iota(from+1, from+1+t, 1);
        For(i, 1, t) trsz[i] = 1;
        For(i, t+1, n) {
            int fa; scanf("%d", &fa);
            ++trsz[from[i] = from[fa]];
            g[fa].push_back(i);
            g[i].push_back(fa);
        }
        For(i, 1, n) scanf("%d", &a[i]);

        memset(mxExDis, 0, sizeof(int) * (t+1));
        memset(mxDis, 0, sizeof(int) * (n+1));
        memset(value, 0, sizeof(int) * (n+1));
        memset(mxDep, -1, sizeof(int) * (t+1));
        memset(flgL, 0, 2*t+1);
        memset(flgR, 0, 2*t+1);
        memset(sumL, 0, sizeof(int) * (2*t+1));
        memset(sumR, 0, sizeof(int) * (2*t+1));
        // Part I: 计算每个白点能到达最远黑点的距离
        //   Part I.1: 先计算树内的
        PntDivide::work1();
        For(i, 1, t) depth[i] = 0, dfs1(currentRing = i), mxDepNodes[i].clear();
        For(i, 1, n) if (a[i] && depth[i] == mxDep[from[i]]) mxDepNodes[from[i]].push_back(i);
        For(i, 1, t) if (mxDep[i] == -1) mxDep[i] = 0x80000000;
struct DequeBase{
    bool empty() { return hd > tl; }
    void init(int *_arr) { hd=0, tl=-1, arr=_arr; }
    int front() { return q[hd]; }
    void pop() { ++hd; }
    void chkgt(int i) { while (!empty() && front() <= i) pop(); }
    void chklt(int i) { while (!empty() && front() >= i) pop(); }
protected: int q[N], *arr, hd, tl;
};
static struct DequeWithEQ : public DequeBase {
    void push(int i) {
        while (hd <= tl && arr[i] > arr[q[tl]]) --tl;
        q[++tl] = i;
    }
} deq;
static struct DequeNonEQ : public DequeBase {
    void push(int i) {
        while (hd <= tl && arr[i] >= arr[q[tl]]) --tl;
        q[++tl] = i;
    }
} dneq;
        // 先处理往右边走的
        For(i, 1, t) arrR[i] = mxDep[i] + i;
        For(i, t+1, 2*t) arrR[i] = mxDep[i-t] + i;
        int range = t/2; // 最多走这么多条环边
        // printf("Dealing with right: %d\n",  range);
        deq.init(arrR); dneq.init(arrR);
        For(i, 1, range) deq.push(i), dneq.push(i);
        For(i, 1, t) { // 处理跨树（经过环边）
            deq.chkgt(i), dneq.chkgt(i);
            deq.push(i+range), dneq.push(i+range);
            flgR[deq.front()] = true;
            covR[i] = {deq.front(), dneq.front()};
            umax(mxExDis[i], disR[i] = arrR[deq.front()] - i);
        }
        while (!deq.empty() && deq.front() <= dneq.front())
            flgR[deq.front()] = true, deq.pop();
        // 再处理往左边走的
        For(i, 1, t) arrL[i] = mxDep[i] + 2*t - i;
        For(i, t+1, 2*t) arrL[i] = mxDep[i-t] + 2*t - i;
        range = (t-1)/2;
        // printf("Dealing with left: %d\n",  range);
        deq.init(arrL), dneq.init(arrL);
        roF(i, 2*t, 2*t - range + 1) deq.push(i), dneq.push(i);
        roF(i, t, 1) {
            deq.chklt(i+t), dneq.chklt(i+t);
            deq.push(i+t-range), dneq.push(i+t-range);
            flgL[deq.front()] = true;
            covL[i] = {dneq.front(), deq.front()};
            umax(mxExDis[i], disL[i] = arrL[deq.front()] - (t - i));
        }
        while (!deq.empty() && deq.front() >= dneq.front())
            flgL[deq.front()] = true, deq.pop();
        
        For(i, 1, n) if (!a[i])
            umax(mxDis[i], depth[i] + mxExDis[from[i]]);
#ifdef DEBUG
        printf("MxDep: "); For(i, 1, t) printf("%d%c", mxDep[i], " \n"[i==t]);
        printf("MxExDis: "); For(i, 1, t) printf("%d%c", mxExDis[i], " \n"[i==t]);
        printf("MxDis: "); For(i, 1, n) printf("%d%c", mxDis[i], " \n"[i==n]);
        printf("Lflg: "); For(i, 1, 2*t) printf("%d%c", (int)flgL[i], " \n"[i==2*t]);
        printf("Lrg: "); For(i, 1, t) printf("[%d,%d]%c", (covL[i].first-1)%t+1, (covL[i].second-1)%t+1, " \n"[i==t]);
        printf("Rflg: "); For(i, 1, 2*t) printf("%d%c", (int)flgR[i], " \n"[i==2*t]);
        printf("Rrg: "); For(i, 1, t) printf("[%d,%d]%c", (covR[i].first-1)%t+1, (covR[i].second-1)%t+1, " \n"[i==t]);
#endif

        // Part II:
        PntDivide::work2();
        // printf("After Point Divide Value: "); For(i, 1, n) printf("%d%c", value[i], " \n"[i==n]);
        For(i, 1, n) if (!a[i]) {
            if (disL[from[i]] + depth[i] == mxDis[i]) ++sumL[covL[from[i]].first], --sumL[covL[from[i]].second+1];
            if (disR[from[i]] + depth[i] == mxDis[i]) ++sumR[covR[from[i]].first], --sumR[covR[from[i]].second+1];
        }
        For(i, 1, 2*t) {
            sumL[i] += sumL[i-1], sumR[i] += sumR[i-1];
            int u = i > t ? i - t : i;
            if (flgL[i]) for (int v : mxDepNodes[u]) value[v] += sumL[i];
            if (flgR[i]) for (int v : mxDepNodes[u]) value[v] += sumR[i];
        }
#ifdef DEBUG
        printf("disL: "); For(i, 1, t) printf("%d%c", disL[i], " \n"[i==t]);
        printf("disR: "); For(i, 1, t) printf("%d%c", disR[i], " \n"[i==t]);
        printf("sumL: "); For(i, 1, 2*t) printf("%d%c", sumL[i], " \n"[i==2*t]);
        printf("sumR: "); For(i, 1, 2*t) printf("%d%c", sumR[i], " \n"[i==2*t]);
        printf("Final Value: "); For(i, 1, n) printf("%d%c", value[i], " \n"[i==n]);
#endif

        printf("%d\n", *std::max_element(value+1, value+1+n));
    }
    return 0;
}

```

Hack 样例：
```
1
20 8
1 9 9 9 11 13 4 11 10 14 17 15
0 0 1 1 1 0 0 1 0 0 1 0 1 0 0 0 1 1 1 0
```
正确答案：6
我的答案：5
有一个很关键的信息，就是当我的程序处理，仅对于单颗树的情况时，会输出更大的数值。也就是我的点分治大概率错了