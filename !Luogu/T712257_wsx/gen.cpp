#include <bits/stdc++.h>
using namespace std;

using ll = long long;

// 生成 [l, r] 的随机整数
ll rnd_ll(ll l, ll r, mt19937_64 &rng) {
    uniform_int_distribution<ll> dist(l, r);
    return dist(rng);
}

int main(int argc, char* argv[]) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    if (argc != 5) {
        cerr << "Usage: " << argv[0] << " n treeType valueType seed\n";
        cerr << "treeType: 1=random, 2=chain, 3=star, 4=layered, 5=centroid-like\n";
        cerr << "valueType: 1=random[1,1e9], 2=small[1,100], 3=long-runs, 4=depth-based, 5=almost-equal\n";
        return 1;
    }

    int n = atoi(argv[1]);
    int treeType = atoi(argv[2]);
    int valueType = atoi(argv[3]);
    unsigned long long seed = strtoull(argv[4], nullptr, 10);

    mt19937_64 rng(seed);

    if (n < 1) n = 1;

    vector<pair<int,int>> edges;
    edges.reserve(max(0, n-1));

    // 1. 生成树结构
    if (n == 1) {
        // 单点图，无边
    } else if (treeType == 2) {
        // 链式树：1-2-3-...-n
        for (int i = 2; i <= n; ++i) {
            edges.push_back({i-1, i});
        }
    } else if (treeType == 3) {
        // 星型：1 与所有其他点相连
        for (int i = 2; i <= n; ++i) {
            edges.push_back({1, i});
        }
    } else if (treeType == 4) {
        // 分层 / 接近完全树：每个点随机连向前面某个点，但尽量倾向小编号
        // 可以记作 BFS 树结构
        for (int i = 2; i <= n; ++i) {
            int parent = rnd_ll(max(1, i/2), i-1, rng); // 倾向于前半部分
            edges.push_back({parent, i});
        }
    } else if (treeType == 5) {
        // 重心型：先生成一个“主干”链，再向主干随机挂子树
        int backbone = max(1, n / 3);
        // 主干
        for (int i = 2; i <= backbone; ++i) {
            edges.push_back({i-1, i});
        }
        // 剩余节点挂在主干上
        for (int i = backbone + 1; i <= n; ++i) {
            int parent = rnd_ll(1, backbone, rng);
            edges.push_back({parent, i});
        }
    } else {
        // 默认：随机树（Prufer 序或简单随机连向前面节点）
        // 这里用“每个新点连向前面随机一个点”的经典方式
        for (int i = 2; i <= n; ++i) {
            int parent = rnd_ll(1, i-1, rng);
            edges.push_back({parent, i});
        }
    }

    // 2. 生成权值 s_i
    const ll MAXV = (ll)1e9;
    vector<ll> s(n+1);

    if (valueType == 2) {
        // 小范围 [1,100]
        for (int i = 1; i <= n; ++i) {
            s[i] = rnd_ll(1, 100, rng);
        }
    } else if (valueType == 3) {
        // 长连续段 + 少量噪声：
        // 思路：先随机几个“段起点”，构造多段 [x, x+len-1] 的连续区间，
        // 再在部分位置打乱一下。
        int numBlocks = max(1, (int)log2(max(2, n))); // O(log n) 段
        vector<int> pos(n);
        iota(pos.begin(), pos.end(), 1);
        shuffle(pos.begin(), pos.end(), rng);

        int used = 0;
        ll baseVal = rnd_ll(1, MAXV / 2, rng);
        for (int b = 0; b < numBlocks && used < n; ++b) {
            int len = rnd_ll(1, (ll)(n / numBlocks + 5), rng);
            len = min(len, n - used);
            ll startVal = baseVal + rnd_ll(0, 1000, rng);
            for (int j = 0; j < len && used < n; ++j) {
                int id = pos[used++];
                s[id] = startVal + j; // 一段连续
            }
        }
        // 剩余的（如果有）随机填
        while (used < n) {
            int id = pos[used++];
            s[id] = rnd_ll(1, MAXV, rng);
        }
        // 再对少量点打“噪声”
        int noiseCnt = n / 20; // 5%
        for (int k = 0; k < noiseCnt; ++k) {
            int id = rnd_ll(1, n, rng);
            s[id] = rnd_ll(1, MAXV, rng);
        }
    } else if (valueType == 4) {
        // 随树深度递增：深度越深，值越大，容易形成很多局部连续段
        vector<vector<int>> g(n+1);
        for (auto &e : edges) {
            int u = e.first, v = e.second;
            g[u].push_back(v);
            g[v].push_back(u);
        }
        vector<int> depth(n+1, -1);
        queue<int> q;
        depth[1] = 0;
        q.push(1);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : g[u]) if (depth[v] == -1) {
                depth[v] = depth[u] + 1;
                q.push(v);
            }
        }
        // 深度最大值
        int maxd = 0;
        for (int i = 1; i <= n; ++i) maxd = max(maxd, depth[i]);

        // 给每个深度段一个基值，再加少量随机偏移
        vector<ll> depthBase(maxd+1);
        for (int d = 0; d <= maxd; ++d) {
            depthBase[d] = rnd_ll(1, max(1LL, MAXV - 1000000), rng);
        }
        for (int i = 1; i <= n; ++i) {
            ll base = depthBase[depth[i]];
            // 让同深度附近点容易形成连续 >>
            ll offset = rnd_ll(0, 1000, rng);
            s[i] = base + offset;
        }
    } else if (valueType == 5) {
        // 全相同或几乎相同：测试算法在大量重复值下的行为
        ll base = rnd_ll(1, MAXV, rng);
        for (int i = 1; i <= n; ++i) {
            s[i] = base;
        }
        // 5% 的点随机扰动，打断部分连续段
        int diff = max(1, n / 20);
        for (int k = 0; k < diff; ++k) {
            int id = rnd_ll(1, n, rng);
            s[id] = base + rnd_ll(-5, 5, rng); // 仍然在附近
            if (s[id] < 1) s[id] = 1;
            if (s[id] > MAXV) s[id] = MAXV;
        }
    } else {
        // valueType == 1 默认：完全随机 [1, 1e9]
        for (int i = 1; i <= n; ++i) {
            s[i] = rnd_ll(1, MAXV, rng);
        }
    }

    // 3. 打乱节点编号（可选）
    // 这样可以避免“1 天然是根”的偏见；但题目固定根为 1，
    // 如果想保持 1 为根，可不打乱。
    // 这里的做法：先构造映射 perm: old -> new，重标。
    // 如果你不想打乱，直接注释掉这一整个 block 即可。
    {
        vector<int> perm(n+1), rperm(n+1);
        for (int i = 1; i <= n; ++i) perm[i] = i;
        shuffle(perm.begin() + 1, perm.end(), rng);
        for (int i = 1; i <= n; ++i) rperm[perm[i]] = i;

        // 重新标号边
        for (auto &e : edges) {
            e.first = rperm[e.first];
            e.second = rperm[e.second];
        }
        // s 也要随编号变换
        vector<ll> new_s(n+1);
        for (int i = 1; i <= n; ++i) {
            new_s[rperm[i]] = s[i];
        }
        s.swap(new_s);
    }

    // 4. 输出
    cout << n << "\n";
    for (auto &e : edges) {
        cout << e.first << " " << e.second << "\n";
    }
    for (int i = 1; i <= n; ++i) {
        cout << s[i] << (i == n ? '\n' : ' ');
    }

    return 0;
}
