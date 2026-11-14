#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long K;
    cin >> N >> K;

    vector<long long> A(N);
    long long sum = 0;
    for (auto &x : A) {
        cin >> x;
        sum += x;
    }

    int outstanding = 0;
    for (long long x : A) {
        if ((__int128)x * (N - 1) >= (__int128)K * (sum - x)) {
            ++outstanding;
        }
    }
    cout << outstanding << '\n';
    return 0;
}
