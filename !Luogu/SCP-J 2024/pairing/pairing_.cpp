#include <bits/stdc++.h>
using namespace std;

int a[500010], dp[500010][3];

unordered_map<int, int> f;
int mx1, my1, mx2, my2;

int main() {
#ifndef ONLINE_JUDGE
    freopen("pairing.in", "r", stdin);
    freopen("pairing.out", "w", stdout);
#endif
	ios::sync_with_stdio(false);
	int n;
	cin >> n;
	dp[0][1] = -1e9;
	for (int i = 1; i <= n; i++) {
		cin >> a[i];
		dp[i][0] = max(dp[i - 1][0], dp[i - 1][2]);
		dp[i][2] = max(dp[i][2], dp[f[a[i]]][1] + 1);
		if (mx1 == a[i]) {
			dp[i][1] = dp[my2][2] + 1;
		} else {
			dp[i][1] = dp[my1][2] + 1;
		}
		if (mx1 == a[i]) {
			if (dp[my1][2] < dp[i][2]) {
				my1 = i;
			}
		} else if (mx2 == a[i]) {
			if (dp[my2][2] < dp[i][2]) {
				my2 = i;
			}
			if (dp[my2][2] > dp[my1][2]) {
				swap(my1, my2);
				swap(mx1, mx2);
			}
		} else {
			if (dp[i][2] > dp[my1][2]) {
				mx2 = mx1;
				my2 = my1;
				mx1 = a[i];
				my1 = i;
			} else if (dp[i][2] > dp[my2][2]) {
				mx2 = a[i];
				my2 = i;
			}
		}
		f[a[i]] = i;
	}
	cout << max(dp[n][0], dp[n][2]) << endl;
}