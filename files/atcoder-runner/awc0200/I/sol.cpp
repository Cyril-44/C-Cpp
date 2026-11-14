#include <bits/stdc++.h>
using namespace std;

struct Flight {
    int a, b, l, r, c, h, d;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K;
    long long G;
    if (!(cin >> N >> M >> K >> G)) return 0;
    int S, T;
    cin >> S >> T;

    vector<long long> P(N), Q(N);
    for (int i = 0; i < N; ++i) cin >> P[i] >> Q[i];

    vector<Flight> flights(M);
    for (auto &e : flights) {
        cin >> e.a >> e.b >> e.l >> e.r >> e.c >> e.h >> e.d;
        --e.a;
        --e.b;
    }

    const long long INF = (1LL << 62);
    long long answer = INF;

    for (int U = 0; U <= K; ++U) {
        for (int V = U; V <= K; ++V) {
            if (U > S || S > V || U > T || T > V) continue;

            const int width = V - U + 1;
            const int states = N * width;
            auto id = [width, U](int base, int altitude) {
                return base * width + (altitude - U);
            };
            vector<vector<pair<int, long long>>> graph(states);

            for (int i = 0; i < N; ++i) {
                for (int x = U; x <= V; ++x) {
                    int from = id(i, x);
                    if (x < V) graph[from].push_back({id(i, x + 1), P[i]});
                    if (x > U) graph[from].push_back({id(i, x - 1), Q[i]});
                }
            }
            for (const auto &e : flights) {
                int lo = max(U, e.l), hi = min(V, e.r);
                for (int x = lo; x <= hi; ++x) {
                    long long cost = (long long)e.c + (long long)e.d * abs(x - e.h);
                    graph[id(e.a, x)].push_back({id(e.b, x), cost});
                }
            }

            vector<long long> dist(states, INF);
            priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
            int source = id(0, S);
            dist[source] = 0;
            pq.push({0, source});
            while (!pq.empty()) {
                auto [du, v] = pq.top();
                pq.pop();
                if (du != dist[v]) continue;
                if (v == id(N - 1, T)) break;
                for (auto [to, w] : graph[v]) {
                    if (du + w < dist[to]) {
                        dist[to] = du + w;
                        pq.push({dist[to], to});
                    }
                }
            }
            long long path = dist[id(N - 1, T)];
            if (path != INF) answer = min(answer, path + G * (V - U));
        }
    }

    if (answer == INF) cout << "NA\n";
    else cout << answer << '\n';
    return 0;
}
