#include <iostream>
#include <vector>
#include <queue>

using namespace std;

typedef long long ll;

// 使用结构体存储西瓜，方便在优先队列中排序
struct Watermelon {
    int id;
    ll cost;
    // 小根堆重载运算符
    bool operator>(const Watermelon& other) const {
        return cost > other.cost;
    }
};

void solve() {
    ll n;
    int m;
    if (!(cin >> n >> m)) return;

    vector<ll> c(m + 1);
    vector<int> x(m + 1);
    vector<vector<int>> children(m + 1);

    for (int i = 1; i <= m; ++i) cin >> c[i];
    for (int i = 1; i <= m; ++i) {
        cin >> x[i];
        if (x[i] != 0) {
            children[x[i]].push_back(i);
        }
    }

    // 小根堆，存储当前可选的西瓜
    priority_queue<Watermelon, vector<Watermelon>, greater<Watermelon>> pq;

    // 将所有没有依赖限制的西瓜入堆
    for (int i = 1; i <= m; ++i) {
        if (x[i] == 0) {
            pq.push({i, c[i]});
        }
    }

    int count = 0;
    while (!pq.empty()) {
        Watermelon current = pq.top();
        pq.pop();

        if (n >= current.cost) {
            n -= current.cost;
            count++;
            // 种下当前西瓜后，解锁它的所有子节点
            for (int childId : children[current.id]) {
                pq.push({childId, c[childId]});
            }
        } else {
            // 连最便宜的都种不下了，直接退出
            break;
        }
    }

    cout << count << endl;
}

int main() {
    // 优化 IO
    ios::sync_with_stdio(false);
    cin.tie(0);

    int t;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}