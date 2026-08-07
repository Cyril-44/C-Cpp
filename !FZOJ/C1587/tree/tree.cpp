#include <bits/stdc++.h>
#define For(i, s, t) for (int i = (s); i <= (t); i++)
#define roF(i, s, t) for (int i = (s); i >= (t); i--)
constexpr int N = 5;
int n, t, a[N];
std::vector<int> g[N];
int from[N]; // fr[i]: i 属于环上哪一个子树
int trsz[N]; // trsz[i]: 环上第 i 个子树大小
int depth[N]; // dep[i]: i 在自己的树中的深度 (0-rooted)
int mxDep[N]; // mxDep[i]: 环上第 i 个子树中最深的黑点的深度
int mxExDis[N]; // mxExDis[i]: 环上点 i 经过环边到别的子树中黑点的最远距离
int arrL[N<<1], arrR[N<<1]; // arr: 当前经过等差数列修补的 mxDep
bool flgL[N<<1], flgR[N<<1]; // flg: 能作为备选贡献的 mxDep
std::pair<int,int> covR[N], covL[N]; // cov: 在 i 下，能作为备选贡献的 mxDep 为 cov[i].first ~ cov[i].second
int mxDis[N]; // mxDis[i]: 点 i 能到达最远的黑点的距离
int disL[N<<1], disR[N<<1];
int sumL[N<<1], sumR[N<<1];
int value[N];
int currentRing;
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
        std::vector<bool> subs(sons.size()); // 减去环上某些子树的贡献
        for (int i = 0; i < sons.size(); i++) {
            int v = sons[i];
            int dis = mxids.size() == 1 && mxv == mxdeps[i] ? mx2v : mxv;
            for (int vv : subtrs[i]) if (!a[vv])
                if (mxDis[vv] == dep[vv] + dis) {
                    if (mxdeps[i] == mxv) {
                        if (mxids.size() == 1) add2++;
                        else add++, subs[i] = true;
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
        
        // Part I: 计算每个白点能到达最远黑点的距离
        //   Part I.1: 先计算树内的
        memset(value, 0, sizeof(int) * (n+1));
        PntDivide::work1();
        memset(mxDep, -1, sizeof(int) * (t+1));
        For(i, 1, t) depth[i] = 0, dfs1(currentRing = i), mxDepNodes[i].clear();
        For(i, 1, n) if (a[i] && depth[i] == mxDep[from[i]]) mxDepNodes[from[i]].push_back(i); 
struct DequeBase{
    bool empty() { return hd > tl; }
    void init(int *_arr) { hd=0, tl=-1, arr=_arr; }
    void pop() { ++hd; }
    int front() { return q[hd]; }
    void chkge(int i) { while (!empty() && front() < i) pop(); }
    void chkle(int i) { while (!empty() && front() > i) pop(); }
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
        while (hd <= tl && arr[i] > arr[q[tl]]) --tl;
        q[++tl] = i;
    }
} dneq;
        For(i, 1, t) arrR[i] = mxDep[i] + i;
        For(i, t+1, 2*t) arrR[i] = mxDep[i-t] + i;
        memset(flgL, 0, 2*t+1);
        memset(flgR, 0, 2*t+1);
        // 先处理往右边走的
        int range = (t-1)/2; // 最多走这么多条环边
        deq.init(arrR); dneq.init(arrR);
        For(i, 1, range) deq.push(i), dneq.push(i);
        For(i, 1, t) { // 处理跨树（经过环边）
            deq.chkge(i), dneq.chkge(i);
            deq.push(i+range), dneq.push(i+range);
            flgR[deq.front()] = true;
            covR[i] = {deq.front(), dneq.front()};
            umax(mxExDis[i], disR[i] = arrR[deq.front()] - i);
        }
        while (!deq.empty() && deq.front() < covR[t])
            flgR[deq.front()] = true, deq.pop();
        // 再处理往左边走的
        For(i, 1, t) arrL[i] = mxDep[i] + 2*t - i;
        For(i, t+1, 2*t) arrL[i] = mxDep[i-t] + 2*t - i;
        range = t/2 - 1;
        deq.init(arrL), dneq.init(arrL);
        roF(i, 2*t, 2*t - range + 1) deq.push(i), dneq.push(i);
        roF(i, t, 1) {
            deq.chkle(i+t), dneq.chkle(i+t);
            deq.push(i+t-range), dneq.push(i+t-range);
            flgL[deq.front()] = true;
            covL[i] = {dneq.front(), deq.front()};
            umax(mxExDis[i], disL[i] = arrL[deq.front()] - (t - i));
        }
        while (!deq.empty() && deq.front() > covL[1])
            flgL[deq.front()] = true, deq.pop();
        
        For(i, 1, n) if (!a[i])
            umax(mxDis[i], depth[i] + mxExDis[from[i]]);

        // Part II:
        PntDivide::work2();
        For(i, 1, t) {
            if (disL[i] == mxDis[i]) ++sumL[covL[i].first], --sumL[covL[i].second+1];
            if (disR[i] == mxDis[i]) ++sumR[covR[i].first], --sumR[covR[i].second+1];
        }
        For(i, 1, 2*t) {
            sumL[i] += sumL[i-1], sumR[i] += sumR[i-1];
            int u = i > t ? i - t : i;
            if (flgL[i]) for (int v : mxDepNodes[u]) value[v] += sumL[i];
            if (flgR[i]) for (int v : mxDepNodes[u]) value[v] += sumR[i];
        }
        
        printf("%d\n", *std::max_element(value+1, value+1+n));
    }
    return 0;
}