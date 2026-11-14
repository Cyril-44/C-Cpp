#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long S;
    cin >> N >> S;

    vector<long long> A(N);
    for (auto &x : A) cin >> x;

    vector<int> left(N), right(N), st;

    // Nearest board to the left with a strictly smaller height.
    for (int i = 0; i < N; ++i) {
        while (!st.empty() && A[st.back()] >= A[i]) st.pop_back();
        left[i] = st.empty() ? -1 : st.back();
        st.push_back(i);
    }

    st.clear();
    // Nearest board to the right with a strictly smaller height.
    for (int i = N - 1; i >= 0; --i) {
        while (!st.empty() && A[st.back()] >= A[i]) st.pop_back();
        right[i] = st.empty() ? N : st.back();
        st.push_back(i);
    }

    long long answer = LLONG_MAX;
    for (int i = 0; i < N; ++i) {
        long long span = right[i] - left[i] - 1;
        long long needed = (S + A[i] - 1) / A[i];
        if (needed <= span) answer = min(answer, needed);
    }

    cout << (answer == LLONG_MAX ? -1 : answer) << '\n';
    return 0;
}
