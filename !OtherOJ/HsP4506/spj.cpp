#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

struct Skill {
    int id;
    int type;
    int num; // for type 2
    vector<int> outs;
    vector<long long> vals;
};

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);

    int n = inf.readInt();
    vector<long long> a(n + 1);
    long long sumA = 0;
    for (int i = 1; i <= n; ++i) {
        a[i] = inf.readLong();
        sumA += a[i];
    }

    long long totalResin = 515LL * n;

    if (ouf.seekEof()) {
        quitf(_wa, "No output");
    }

    int m = ouf.readInt();
    if (m < 1 || m > 5 * n) {
        quitf(_wa, "Invalid m = %d, must be in [1, %d]", m, 5 * n);
    }

    // id range: [n+1, n+m]
    map<int, int> idToIdx;
    vector<Skill> skills(m);
    for (int i = 0; i < m; ++i) {
        Skill s;
        s.num = 0;
        s.outs.clear();
        s.vals.clear();

        if (ouf.seekEof())
            quitf(_wa, "Unexpected EOF while reading skill %d", i + 1);

        s.id = ouf.readInt();
        if (s.id < n + 1 || s.id > n + m)
            quitf(_wa, "Skill id %d out of range [%d, %d]", s.id, n + 1, n + m);
        if (idToIdx.count(s.id))
            quitf(_wa, "Duplicate skill id %d", s.id);
        idToIdx[s.id] = i;

        s.type = ouf.readInt();
        if (s.type < 1 || s.type > 3)
            quitf(_wa, "Invalid type %d for skill id %d", s.type, s.id);

        if (s.type == 1) {
            // type 1: out-degree = 1, need one out and one val
            int out = ouf.readInt();
            long long val = ouf.readLong();
            s.outs.push_back(out);
            s.vals.push_back(val);
        } else if (s.type == 2) {
            // type 2: read num, then num outs, then one val (same for all)
            s.num = ouf.readInt();
            if (s.num <= 0)
                quitf(_wa, "Skill %d type 2 has non-positive num = %d", s.id, s.num);
            s.outs.resize(s.num);
            for (int j = 0; j < s.num; ++j) {
                s.outs[j] = ouf.readInt();
            }
            long long val = ouf.readLong();
            if (val <= 0)
                quitf(_wa, "Skill %d type 2 has non-positive val = %lld", s.id, val);
            s.vals.assign(s.num, val);
        } else {
            // type 3: out-degree = 2, two outs and two vals
            int out1 = ouf.readInt();
            int out2 = ouf.readInt();
            long long v1 = ouf.readLong();
            long long v2 = ouf.readLong();
            s.outs = {out1, out2};
            s.vals = {v1, v2};
        }

        // check outs distinct
        {
            set<int> st(s.outs.begin(), s.outs.end());
            if ((int)st.size() != (int)s.outs.size())
                quitf(_wa, "Skill %d has duplicate outputs", s.id);
        }

        // check vals positive
        for (long long v : s.vals) {
            if (v <= 0) {
                string allouts, allvals;
                for (auto i : s.outs) allouts += to_string(i) + ' ';
                for (auto j : s.vals) allvals += to_string(j) + ' ';
                quitf(_wa, "Skill %d (Outs: %s, Vals: %s) has non-positive flow value %lld", s.id, allouts.c_str(), allvals.c_str(), v);
            }
        }

        skills[i] = s;
    }

    // indegree for skills (from other skills)
    vector<int> indegSkill(m, 0);
    // incoming resin for skills, enemies, and air
    vector<long long> inSkill(m, 0);
    vector<long long> inEnemy(n + 1, 0);
    long long inAir = 0;

    // build graph among skills for DAG / depth
    vector<vector<int>> gSkill(m);

    auto skillIndex = [&](int id) -> int {
        auto it = idToIdx.find(id);
        if (it == idToIdx.end()) return -1;
        return it->second;
    };

    for (int i = 0; i < m; ++i) {
        Skill &s = skills[i];
        long long outSum = 0;
        for (int j = 0; j < (int)s.outs.size(); ++j) {
            int to = s.outs[j];
            long long val = s.vals[j];

            if (to == -1) {
                inAir += val;
            } else if (1 <= to && to <= n) {
                inEnemy[to] += val;
            } else if (n + 1 <= to && to <= n + m) {
                int idx = skillIndex(to);
                if (idx == -1)
                    quitf(_wa, "Output target %d of skill %d not defined", to, s.id);
                indegSkill[idx]++;
                inSkill[idx] += val;
                gSkill[i].push_back(idx);
            } else {
                quitf(_wa, "Output target %d of skill %d is invalid", to, s.id);
            }
            outSum += val;
        }
        // outSum will be checked against inSkill later (flow conservation)
    }

    // Only skill id n+1 gets resin from traveler
    int srcIdx = skillIndex(n + 1);
    if (srcIdx == -1)
        quitf(_wa, "Skill with id %d (n+1) not defined", n + 1);

    // add traveler -> (n+1) edge with 515n resin
    inSkill[srcIdx] += totalResin;

    // indegree constraints: each skill has exactly one incoming (from skills or traveler),
    // except n+1 which has indegree 0 from skills but 1 from traveler.
    for (int i = 0; i < m; ++i) {
        int id = skills[i].id;
        if (id == n + 1) {
            if (indegSkill[i] != 0)
                quitf(_wa, "Skill %d (n+1) must not have incoming from other skills", id);
        } else {
            if (indegSkill[i] != 1)
                quitf(_wa, "Skill %d must have exactly one incoming from other skills, got %d", id, indegSkill[i]);
        }
    }

    // flow conservation for each skill
    for (int i = 0; i < m; ++i) {
        Skill &s = skills[i];
        long long outSum = 0;
        for (long long v : s.vals) outSum += v;
        if (outSum != inSkill[i])
            quitf(_wa, "Flow not conserved at skill %d: incoming %lld, outgoing %lld",
                  s.id, inSkill[i], outSum);
    }

    // total resin from traveler must be exactly 515n and fully used
    // This is already enforced by conservation and the single source,
    // but we can double-check that inSkill[srcIdx] == totalResin + incoming from skills (which is 0).
    // Already ensured: indegSkill[srcIdx] == 0, so inSkill[srcIdx] == totalResin.

    if (inSkill[srcIdx] != totalResin)
        quitf(_wa, "Skill %d should receive exactly %lld resin from traveler, got %lld",
              n + 1, totalResin, inSkill[srcIdx]);

    // enemy damage must equal a[i]
    for (int i = 1; i <= n; ++i) {
        if (inEnemy[i] != a[i])
            quitf(_wa, "Enemy %d receives %lld damage, expected %lld", i, inEnemy[i], a[i]);
    }

    // DAG check among skills
    int S = srcIdx;
    int K = m;
    vector<int> color(K, 0); // 0=unvisited,1=visiting,2=done
    function<void(int)> dfs = [&](int u) {
        color[u] = 1;
        for (int v : gSkill[u]) {
            if (color[v] == 0) dfs(v);
            else if (color[v] == 1)
                quitf(_wa, "Cycle detected among skills (e.g., %d -> %d)", skills[u].id, skills[v].id);
        }
        color[u] = 2;
    };
    for (int i = 0; i < K; ++i) {
        if (color[i] == 0) dfs(i);
    }

    // longest path length from traveler to each enemy must be exactly 5
    // Define depthSkill: length (in skills) from traveler to this skill.
    // traveler -> (n+1) is length 1.
    vector<int> depthSkill(K, -1);
    depthSkill[S] = 1;

    // topo order via Kahn
    vector<int> indegTopo(K, 0);
    for (int u = 0; u < K; ++u)
        for (int v : gSkill[u])
            indegTopo[v]++;

    queue<int> q;
    for (int i = 0; i < K; ++i)
        if (indegTopo[i] == 0)
            q.push(i);

    vector<int> topo;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        topo.push_back(u);
        for (int v : gSkill[u]) {
            indegTopo[v]--;
            if (indegTopo[v] == 0)
                q.push(v);
        }
    }
    if ((int)topo.size() != K)
        quitf(_wa, "Cycle detected in skills graph during topo sort");

    vector<int> depthEnemy(n + 1, -1);

    for (int u : topo) {
        if (depthSkill[u] < 0) {
            // unreachable from traveler; depth undefined, but if it leads to enemies, that would be bad
            // because enemies must get all their damage from reachable skills (since a[i] > 0).
        }
        for (int j = 0; j < (int)skills[u].outs.size(); ++j) {
            int to = skills[u].outs[j];
            if (to >= n + 1 && to <= n + m) {
                int v = skillIndex(to);
                if (depthSkill[u] >= 0) {
                    depthSkill[v] = max(depthSkill[v], depthSkill[u] + 1);
                }
            } else if (1 <= to && to <= n) {
                if (depthSkill[u] >= 0) {
                    depthEnemy[to] = max(depthEnemy[to], depthSkill[u] + 1);
                }
            }
        }
    }

    for (int i = 1; i <= n; ++i) {
        if (depthEnemy[i] != 6)
            quitf(_wa, "Enemy %d has longest chain length %d, expected 5", i, depthEnemy[i] - 1);
    }

    // All checks passed
    quitf(_ok, "Accepted");
}
