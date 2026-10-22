// Official special judge checker. Compile with the repository's testlib.h.
#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;

long long optimum(int n) {
    long long z = 1LL * n * n;
    if (n & 1) return (z - 1) / 2;
    if (n % 4 == 2) return (z + n - 2) / 2;
    if (n == 4 || n == 8) return (z + n - 4) / 2;
    return (z + n - 6) / 2;
}

void verify(InStream& stream, const vector<int>& ns, bool jury) {
    TResult verdict = jury ? _fail : _wa;
    const char* who = jury ? "jury" : "contestant";
    const string symbols = "QWEADZXC";
    const int dr[] = {-1,-1,-1,0,0,1,1,1};
    const int dc[] = {-1,0,1,-1,1,-1,0,1};
    for (int tc = 0; tc < (int)ns.size(); ++tc) {
        int n = ns[tc];
        long long value = stream.readLong();
        if (value != optimum(n))
            quitf(verdict, "%s case %d: claimed %lld, optimum %lld", who, tc+1, value, optimum(n));
        string s = stream.readToken();
        if (s.size() != size_t(n*n-1))
            quitf(verdict, "%s case %d: length %zu, expected %d", who, tc+1, s.size(), n*n-1);
        vector<unsigned char> seen(n*n, 0);
        seen[0] = 1;
        int r = 0, c = 0, right = 0;
        for (int step = 0; step < (int)s.size(); ++step) {
            size_t d = symbols.find(s[step]);
            if (d == string::npos)
                quitf(verdict, "%s case %d: invalid symbol at step %d", who, tc+1, step+1);
            r += dr[d]; c += dc[d];
            if (r < 0 || r >= n || c < 0 || c >= n)
                quitf(verdict, "%s case %d: outside board at step %d", who, tc+1, step+1);
            if (seen[r*n+c]++)
                quitf(verdict, "%s case %d: revisited a cell at step %d", who, tc+1, step+1);
            right += s[step] == 'D';
        }
        if (r != n-1 || c != n-1)
            quitf(verdict, "%s case %d: wrong endpoint", who, tc+1);
        if (right != value)
            quitf(verdict, "%s case %d: counted %d right steps, claimed %lld", who, tc+1, right, value);
    }
    if (!stream.seekEof()) quitf(verdict, "%s has extra output tokens", who);
}

int main(int argc, char* argv[]) {
    registerTestlibCmd(argc, argv);
    int t = inf.readInt(1, 5, "T");
    vector<int> ns(t);
    for (int& n : ns) n = inf.readInt(2, 1000, "N");
    if (!inf.seekEof()) quitf(_fail, "extra input tokens");
    verify(ans, ns, true);
    verify(ouf, ns, false);
    quitf(_ok, "%d valid optimal Hamilton paths", t);
}
