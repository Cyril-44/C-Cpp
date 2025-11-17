#include <bits/stdc++.h>
#include <random>
constexpr int N = 5005;
using int8 = signed char;
int8 a[N]; // -1, 0, 1
int cnt[N], sum[N], sum1[N], q[N], qzha[N], qzhb[N], zero[N];
inline int bruteForce(int n, int k) {
    k <<= 1;
    int ans = -1;
    for (int l = 0; l < n; l++)
        for (int r = l+1; r <= n; r++) {
            int zeros = cnt[r] - cnt[l];
            ans = std::max(ans, sum[r] - sum[l-1] + std::min(zeros, k - zeros));
        }
    return ans;
}
inline int solve(int n, int k) {
    int ans = 0;
    int l = 0, r = 0; // a[q[i]] 单调递增，取 q[l] 为最小的 sum1 位置
    for (int i = 0; i <= n; i++) {
        while (l < r && cnt[q[l]] + k < cnt[i]) ++l;
        while (l < r && sum1[q[r-1]] >= sum1[i]) --r;
        q[r++] = i;
        ans = std::max(ans, sum1[i] - sum1[q[l]]);
    }
    return ans;
}
inline int standard(int n, int k) {
    memset(q, 0, sizeof q);
    for (int i = 1; i <= n; i++) {
        qzha[i] = qzha[i - 1] + (a[i] == -1 ? -1 : 1);
        qzhb[i] = qzhb[i - 1] + (a[i] == 1 ? 1 : -1);
        zero[i] = zero[i - 1] + (a[i] == 0);
        // printf("%d %d %d\n",qzha[i],qzhb[i],zero[i]);
    }
    int lcur = 0;
    int l = 1, r = 1;
    q[1] = 0;
    int mn = 0;
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        while (zero[lcur] + k < zero[i]) {
            if (q[l] == lcur)
                l++;
            mn = std::min(mn, qzhb[lcur]);
            lcur++;
        }
        while (l <= r && qzha[q[r]] > qzha[i])
            r--;
        q[++r] = i;
        // printf("*   %d %d\n",qzha[i],qzha[q[l]]);
        ans = std::max(ans, qzha[i] - qzha[q[l]]);
        if (zero[i] > k) {
            ans = std::max(ans, qzha[i] - (zero[i] - k) * 2 - mn);
            // printf("**  %d %d\n",qzha[i]-(zero[i]-k)*2,mn);
        }
    }
    return ans;
}
int main() {
    std::mt19937_64 rng(time(nullptr));
    std::uniform_int_distribution<int> rgn(1, 5000), rgai(-1, 1);
    int n, k, ansexp, ansget;
start:
    n = rgn(rng), k = std::uniform_int_distribution<int>(1, n)(rng);
    for (int i = 1; i <= n; i++) {
        a[i] = rgai(rng);
        sum[i] = sum[i-1] + a[i];
        cnt[i] = cnt[i-1] + !a[i];
        sum1[i] = sum1[i-1] + (a[i] ? a[i] : 1);
    }
    ansget = solve(n, k);
    ansexp = standard(n, k);
    if (ansexp != ansget) {
        std::cerr << "Error!" << std::endl;
        freopen("ohno_hack.in", "w", stdout);
        printf("%d %d\n", n, k);
        for (int i = 1; i <= n; i++)
            printf("%d ", a[i]);
        freopen("ohno_hack.ans", "w", stdout);
        printf("%d\nFound: %d\n", ansexp, ansget);
        fclose(stdout);
        throw;
    }
    goto start;
    return 0;
}