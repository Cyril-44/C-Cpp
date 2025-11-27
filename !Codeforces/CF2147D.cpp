#include <bits/stdc++.h>
using namespace std;
constexpr int N = 200005;
int a[N];
struct NumCnt {
    int n, cnt;
    inline bool operator< (const NumCnt &rhs) const {
        return n < rhs.n;
    }
    inline bool operator> (const NumCnt &rhs) const {
        return cnt > rhs.cnt || cnt == rhs.cnt && n < rhs.n;
    }
} b[N];
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T, n;
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 1; i <= n; i++)
            cin >> a[i];
        sort(a+1, a+1 + n);
        int top = 0;
        for (int i = 1; i <= n; i++) {
            if (b[top].n != a[i]) b[++top] = {a[i], 0};
            ++b[top].cnt;
        }

        int round = 0; // 0 for Alice, 1 for Bob
        long long s1(0), s2(0);
        for (int i = 1; i <= top; i++) {
            long long tot = (int64_t)(b[i].n >> 1) * b[i].cnt;
            s1 += tot, s2 += tot;
            if (b[i].n & 1) {
                if (round) s2 += b[i].cnt;
                else s1 += b[i].cnt;
                round ^= 1;
            }
        }
        printf("%lld %lld\n", s1, s2);
    }
    return 0;
}