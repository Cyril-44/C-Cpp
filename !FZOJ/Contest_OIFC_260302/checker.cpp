#include "testlib.h"
#include <bits/stdc++.h>

using namespace std;

const int N = 1.1e6 + 5, M = 1e8;
int a[25], sum[N];
int main(int argc, char** argv)
{
	int T, t, i, n, k;
	registerTestlibCmd(argc, argv);
	inf.readInt();
	T = inf.readInt();
	for(t = 1; t <= T; t++)
	{
		setTestCase(t);
		n = inf.readInt();
		k = inf.readInt();
		map<int, int> mp;
		for(i = k; i < (1 << n); i++)
			mp[inf.readInt()]--;
		for(i = 0; i < n; i++)
			a[i] = ouf.readInt(-M, M, "a[i]");
		for(i = 1; i < (1 << n); i++)
			sum[i] = sum[i & (i - 1)] + a[__lg(i & -i)];
		for(i = 0; i < (1 << n); i++)
			mp[sum[i]]++;
		for(auto o: mp)
			ensuref(o.second >= 0, "subset sum must contain all of S");
	}
	quitf(_ok, "correct!\n");
	return 0;
}