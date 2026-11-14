#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    vector<long long> skills(N);
    for (auto &x : skills) cin >> x;
    sort(skills.begin(), skills.end(), greater<long long>());

    vector<pair<long long, int>> projects(M);
    for (int j = 0; j < M; ++j) {
        long long priority;
        int required;
        cin >> priority >> required;
        projects[j] = {priority, required};
    }

    vector<int> order(M);
    iota(order.begin(), order.end(), 0);
    sort(order.begin(), order.end(), [&](int a, int b) {
        if (projects[a].first != projects[b].first)
            return projects[a].first > projects[b].first;
        return a < b;
    });

    vector<long long> prefix(N + 1, 0);
    for (int i = 0; i < N; ++i) prefix[i + 1] = prefix[i] + skills[i];

    vector<long long> answer(M);
    int used = 0;
    for (int j : order) {
        int required = projects[j].second;
        if (used + required > N) {
            cout << -1 << '\n';
            return 0;
        }
        answer[j] = prefix[used + required] - prefix[used];
        used += required;
    }

    for (long long x : answer) cout << x << '\n';
    return 0;
}
