#include "testlib.h"

using namespace std;

int main(int argc, char* argv[]) {
	registerTestlibCmd(argc, argv);
	
	int T = inf.readInt();
	int correct = 1;
	for (int testc = 1; testc <= T; testc++) {
		cerr << "Parsing test case " << testc << endl;
		
		int n = inf.readInt(), m = inf.readInt(), k = inf.readInt();
		
		map<pair<int, int>, int> edge;
		for (int i = 1; i < n; ++i) {
			int x = inf.readInt();
			int y = inf.readInt();
			edge[make_pair(x, y)] = 1;
			edge[make_pair(y, x)] = 1;
		}
		
		string usr_result = ouf.readToken();
		string std_result = ans.readToken();
		if (usr_result != std_result) {
			quitf(_wa, " on test case %d: YesNo diff.", testc);
		}
		
		if (usr_result == "Yes") {
			vector<int> v(m), vis(n + 1);
			for (int i = 0; i < m; ++i) {
				v[i] = ouf.readInt();
				if (v[i] < 1 || v[i] > n) {
					quitf(_wa, " on test case %d: %d found.", testc, v[i]);
				} else {
					vis[v[i]] = 1;
				}
			}
			for (int i = 1; i < m; ++i) {
				if (!edge[make_pair(v[i - 1], v[i])]) quitf(_wa, " on test case %d: no such edge <%d,%d>.", testc, v[i], v[i-1]);
			}
			int cnt = 0;
			for (int i = 1; i <= n; ++i) if (vis[i]) ++cnt;
			if (cnt != k) quitf(_wa, " on test case %d: wish %d coverd, found only %d.", testc, k, cnt);
		}
		if (!correct) {
			quitf(_wa, " on test case %d", testc);
		} else {
			quitf(_ok, "");
		}
	}
	
	return 0;
}
