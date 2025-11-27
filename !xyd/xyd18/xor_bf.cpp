#include <stdio.h>
#include <string.h>
#include <bitset>
#include <iostream>
#include <bits/stdc++.h>
constexpr int B = 10;
bool vis[1 << B];
int pre[1 << B];
int main() {
	FILE* in = fopen("xor.in", "w"), *out = fopen("xor.ans", "w");
	freopen("xor.txt", "w", stdout);
	for (int i = 1; i < (1<<B); i++)
		pre[i] = pre[i-1] ^ i;
	int T;
	int L, R;
	fprintf(in, "%d\n", 21 * 22 / 2);
	// scanf("%d", &T);
	// while (T--) {
		// scanf("%d%d", &L, &R);
	for (int L = 0; L <= 20; ++L)
	for (int R = L; R <= 20; ++R) {
		fprintf(in, "%d %d\n", L, R);
		std::cout << "===============" << std::bitset<B>(L) << "(" << L << ")====" << std::bitset<B>(R) << '(' << R << ")============\n";
		memset(vis, 0, sizeof vis);
		for (int l = L; l <= R; ++l)
			for (int r = l; r <= R; ++r)
				vis[pre[r] ^ (l ? pre[l-1] : 0)] = true;
		int cnt = 0;
		for (int i = 0; i < (1<<B); i++) if (vis[i]) {
			std::cout << std::bitset<B>(i) << "(" << i << ")\n" ;
			++cnt;
		}
		fprintf(out, "%d\n", cnt);
	}

	return 0;
}