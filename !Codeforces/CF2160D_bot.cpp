#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;
        int m = 2 * n;
        int NB = 10;
        vector<vector<int>> S1(NB), S0(NB);
        for (int i = 1; i <= m; i++) {
            for (int b = 0; b < NB; b++) {
                if (i & (1 << b))
                    S1[b].push_back(i);
                else
                    S0[b].push_back(i);
            }
        }
        vector<int> cur_m1(NB), cur_m0(NB);
        auto query = [&](const vector<int> &set) -> int {
            if (set.empty())
                return 0;
            cout << "? " << set.size();
            for (int j : set)
                cout << " " << j;
            cout << endl;
            cout.flush();
            int res;
            cin >> res;
            return res;
        };
        for (int b = 0; b < NB; b++) {
            cur_m1[b] = query(S1[b]);
            cur_m0[b] = query(S0[b]);
        }
        vector<int> a(m + 1, 0);
        for (int vv = n; vv >= 1; vv--) {
            vector<int> both1, both0, mixed;
            for (int b = 0; b < NB; b++) {
                if (cur_m1[b] == vv)
                    both1.push_back(b);
                else if (cur_m0[b] == vv)
                    both0.push_back(b);
                else
                    mixed.push_back(b);
            }
            long long A = 0;
            for (int b : both1)
                A |= (1LL << b);
            long long X = 0;
            for (int b : mixed)
                X |= (1LL << b);
            int d = mixed.size();
            vector<pair<int, int>> f;
            int mask = 1 << d;
            for (int i = 0; i < mask; i++) {
                long long s = 0;
                for (int i = 0; i < d; i++)
                    if (i & (1 << i))
                        s |= (1LL << mixed[i]);
                long long p = A + s;
                long long q = A + (X - s);
                if (p > q)
                    swap(p, q);
                if (p >= 1 && q <= m && p != q) {
                    f.push_back({(int)p, (int)q});
                }
            }
            sort(f.begin(), f.end());
            int l = 0, r = f.size() - 1;
            while (l < r) {
                int mid = (l + r) / 2;
                set<int> used;
                vector<int> S;
                for (int i = 0; i <= mid; i++) {
                    int p = f[i].first;
                    int q = f[i].second;
                    if (used.find(p) == used.end()) {
                        S.push_back(p);
                        used.insert(p);
                    }
                    if (used.find(q) == used.end()) {
                        S.push_back(q);
                        used.insert(q);
                    }
                }
                int res = query(S);
                if (res == vv) {
                    r = mid;
                } else {
                    l = mid + 1;
                }
            }
            pair<int, int> pir = f[l];
            a[pir.first] = vv;
            a[pir.second] = vv;
            int p1 = pir.first;
            int p2 = pir.second;
            for (int b = 0; b < NB; b++) {
                bool b1 = (p1 & (1 << b)) && (p2 & (1 << b));
                bool b0 = !(p1 & (1 << b)) && !(p2 & (1 << b));
                if (b1 && cur_m1[b] == vv) {
                    vector<int> new_s;
                    for (int j : S1[b])
                        if (j != p1 && j != p2)
                            new_s.push_back(j);
                    cur_m1[b] = query(new_s);
                }
                if (b0 && cur_m0[b] == vv) {
                    vector<int> new_s;
                    for (int j : S0[b])
                        if (j != p1 && j != p2)
                            new_s.push_back(j);
                    cur_m0[b] = query(new_s);
                }
            }
        }
        cout << "! ";
        for (int i = 1; i <= m; i++)
            cout << a[i] << " ";
        cout << endl;
        cout.flush();
    }
    return 0;
}