#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct Skill {
    int id;
    int type;
    vector<int> outs;
    vector<long long> vals; // 对于 type2: 每条边同一个值，但存多份方便统一处理
};

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);

    // 读输入
    int n = inf.readInt();
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        a[i] = inf.readLong();
    }

    long long totalResin = 515LL * n;

    // 读选手输出
    if (ouf.seekEof()) {
        quitf(_wa, "No output");
    }

    int m = ouf.readInt();
    if (m < 1 || m > 5 * n) {
        quitf(_wa, "m out of range: %d, expected 1..%d", m, 5 * n);
    }

    int minId = n + 1;
    int maxId = n + m;

    // 读技能
    map<int, int> idToIndex;
    vector<Skill> skills(m);
    vector<bool> usedId(m + 1, false); // 只是辅助，不强制连续顺序

    for (int i = 0; i < m; ++i) {
        int id = ouf.readInt();
        if (id < minId || id > maxId) {
            quitf(_wa, "skill id %d out of range [%d, %d]", id, minId, maxId);
        }
        if (idToIndex.count(id)) {
            quitf(_wa, "duplicate skill id %d", id);
        }
        idToIndex[id] = i;
        skills[i].id = id;

        int type = ouf.readInt();
        if (type < 1 || type > 3) {
            quitf(_wa, "invalid skill type %d", type);
        }
        skills[i].type = type;

        if (type == 1) {
            // id 1 out val
            int out = ouf.readInt();
            long long val = ouf.readLong();
            if (val <= 0) {
                quitf(_wa, "skill %d type1 has non-positive val", id);
            }
            skills[i].outs.push_back(out);
            skills[i].vals.push_back(val);
        } else if (type == 2) {
            // id 2 num out1..outk val
            int num = ouf.readInt();
            if (num <= 0) {
                quitf(_wa, "skill %d type2 has non-positive num", id);
            }
            vector<int> outs(num);
            for (int j = 0; j < num; ++j) {
                outs[j] = ouf.readInt();
            }
            long long val = ouf.readLong();
            if (val <= 0) {
                quitf(_wa, "skill %d type2 has non-positive val", id);
            }
            skills[i].outs = outs;
            skills[i].vals.assign(num, val);
        } else { // type == 3
            // id 3 out1 out2 val1 val2
            int out1 = ouf.readInt();
            int out2 = ouf.readInt();
            long long v1 = ouf.readLong();
            long long v2 = ouf.readLong();
            if (v1 <= 0 || v2 <= 0) {
                quitf(_wa, "skill %d type3 has non-positive val", id);
            }
            skills[i].outs = {out1, out2};
            skills[i].vals = {v1, v2};
        }

        // 检查同一技能的输出目标互不相同
        {
            auto &outs = skills[i].outs;
            sort(outs.begin(), outs.end());
            if (unique(outs.begin(), outs.end()) != outs.end()) {
                quitf(_wa, "skill %d has duplicate outputs", id);
            }
        }
    }

    // 建图并做各种检查
    // 节点编号：
    //   敌人：1..n
    //   技能：n+1..n+m
    //   空气：-1（单独处理）
    // 我们需要：
    //   inflow / outflow
    map<int, long long> inflow, outflow;

    // 技能图（只在技能之间建边，用于 DAG 检查和最长路）
    // 将技能 id 压缩到 [0..m-1]
    vector<vector<int>> g(m);

    auto isEnemy = [&](int x) { return 1 <= x && x <= n; };
    auto isSkill = [&](int x) { return minId <= x && x <= maxId; };
    auto isAir   = [&](int x) { return x == -1; };

    for (int i = 0; i < m; ++i) {
        int id = skills[i].id;
        auto &outs = skills[i].outs;
        auto &vals = skills[i].vals;

        for (int k = 0; k < (int)outs.size(); ++k) {
            int to = outs[k];
            long long v = vals[k];

            if (!(isEnemy(to) || isSkill(to) || isAir(to))) {
                quitf(_wa, "skill %d has invalid output target %d", id, to);
            }

            outflow[id] += v;
            if (!isAir(to)) {
                inflow[to] += v;
            }

            if (isSkill(to)) {
                int j = idToIndex[to];
                g[idToIndex[id]].push_back(j);
            }
        }
    }

    // 加上旅行者 -> (n+1) 的虚拟边，流量 515n
    inflow[minId] += totalResin;

    // 流量平衡检查
    // 敌人：inflow == a[i]
    for (int i = 1; i <= n; ++i) {
        long long in = inflow[i];
        if (in != a[i]) {
            quitf(_wa, "enemy %d gets damage %lld, expected %lld", i, in, a[i]);
        }
    }

    // 技能：n+1 有额外 515n 输入，其余必须 inflow == outflow
    for (int id = minId; id <= maxId; ++id) {
        long long in = inflow[id];
        long long out = outflow[id];
        if (id == minId) {
            // inflow = sum from other skills + 515n
            // outflow must equal inflow
            if (in != out) {
                quitf(_wa, "skill %d (source) has inflow %lld and outflow %lld, not balanced", id, in, out);
            }
        } else {
            if (in != out) {
                quitf(_wa, "skill %d has inflow %lld and outflow %lld, not balanced", id, in, out);
            }
        }
    }

    // DAG 检查（只在技能子图上）
    {
        vector<int> color(m, 0); // 0=unvisited,1=visiting,2=done
        function<void(int)> dfs = [&](int u) {
            color[u] = 1;
            for (int v : g[u]) {
                if (color[v] == 0) dfs(v);
                else if (color[v] == 1) {
                    quitf(_wa, "cycle detected among skills");
                }
            }
            color[u] = 2;
        };
        for (int i = 0; i < m; ++i) {
            if (color[i] == 0) dfs(i);
        }
    }

    // 计算从 n+1 出发到每个敌人的最长路径长度（按技能数计）
    // 拓扑序 + DP
    // 先对技能图拓扑排序
    vector<int> indeg(m, 0);
    for (int u = 0; u < m; ++u) {
        for (int v : g[u]) indeg[v]++;
    }
    queue<int> q;
    for (int i = 0; i < m; ++i) if (indeg[i] == 0) q.push(i);

    vector<int> topo;
    topo.reserve(m);
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo.push_back(u);
        for (int v : g[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }
    if ((int)topo.size() != m) {
        quitf(_wa, "cycle detected in skills (topo size mismatch)");
    }

    // dpSkill[idIndex] = 从 n+1 到该技能的最长路径长度（技能数）
    vector<int> dpSkill(m, -1000000000);
    int sourceIdx = idToIndex[minId];
    dpSkill[sourceIdx] = 1; // 到 n+1 自己，路径长度计为 1（包含它本身）

    for (int u : topo) {
        if (dpSkill[u] < -1e8) continue;
        for (int v : g[u]) {
            dpSkill[v] = max(dpSkill[v], dpSkill[u] + 1);
        }
    }

    // 对每个敌人，找所有指向它的技能边，取最大路径长度
    vector<int> maxLenEnemy(n + 1, -1000000000);
    for (int i = 0; i < m; ++i) {
        int fromId = skills[i].id;
        int fromIdx = i;
        for (int k = 0; k < (int)skills[i].outs.size(); ++k) {
            int to = skills[i].outs[k];
            if (isEnemy(to)) {
                maxLenEnemy[to] = max(maxLenEnemy[to], dpSkill[fromIdx]);
            }
        }
    }

    for (int i = 1; i <= n; ++i) {
        if (maxLenEnemy[i] != 5) {
            quitf(_wa, "enemy %d has longest chain length %d, expected 5", i, maxLenEnemy[i]);
        }
    }

    quitf(_ok, "OK");
}
