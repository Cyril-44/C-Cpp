#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long K;
    cin >> N >> K;

    vector<long long> S(N), B(N), pref(N + 1, 0);
    for (int i = 0; i < N; ++i) {
        cin >> S[i] >> B[i];
        pref[i + 1] = pref[i] + S[i];
    }

    // left[i]: nearest index to the left with B <= B[i].
    // right[i]: nearest index to the right with B < B[i].
    // These boundaries assign every interval to its leftmost minimum.
    vector<int> left(N), right(N), st;
    for (int i = 0; i < N; ++i) {
        while (!st.empty() && B[st.back()] > B[i]) st.pop_back();
        left[i] = st.empty() ? -1 : st.back();
        st.push_back(i);
    }
    st.clear();
    for (int i = N - 1; i >= 0; --i) {
        while (!st.empty() && B[st.back()] >= B[i]) st.pop_back();
        right[i] = st.empty() ? N : st.back();
        st.push_back(i);
    }

    long long answer = 0;
    for (int i = 0; i < N; ++i) {
        // If l and r are zero-based section indices, the sum is pref[r+1]-pref[l].
        int xl = left[i] + 1, xr = i;
        int yl = i + 1, yr = right[i];
        long long need = K - B[i];
        int xCount = xr - xl + 1;
        int yCount = yr - yl + 1;

        if (xCount <= yCount) {
            for (int x = xl; x <= xr; ++x) {
                long long target = pref[x] + need;
                auto it = lower_bound(pref.begin() + yl,
                                      pref.begin() + yr + 1, target);
                answer += (pref.begin() + yr + 1) - it;
            }
        } else {
            for (int y = yl; y <= yr; ++y) {
                long long limit = pref[y] - need;
                auto it = upper_bound(pref.begin() + xl,
                                      pref.begin() + xr + 1, limit);
                answer += it - (pref.begin() + xl);
            }
        }
    }

    cout << answer << '\n';
    return 0;
}
