#include "testlib.h"
#include <bits/stdc++.h>
#define ru(i, l, r) for (int i = l; i <= r; i++)
using namespace std;
int n, k;
bitset<505> f[505];
int main(int argc, char *argv[]) {
	registerTestlibCmd(argc, argv);
	n = inf.readInt(1, 381), k = ouf.readInt();
	ru(i, 1, k) ru(j, 1, n) f[i][j] = ouf.readInt(0,1);
	ru(i, 1, k) ensuref(f[i].count() == f[1].count(), "Count Mismatch!");
	ru(i, 1, k) ru(j, i + 1, k) ensuref(f[i] != f[j], "f[%d] != f[%d]", i, j), ensuref((f[i] & f[j]).count() == 1, "f[%d] & f[%d].count() != 1", i, j);
	quitf(_ok, "Accepted");
	return 0;
}
