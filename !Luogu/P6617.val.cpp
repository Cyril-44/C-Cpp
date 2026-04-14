#include <bits/stdc++.h>
using namespace std;
int main() {
    mt19937_64 rng(random_device{}());
    ofstream fin("P6617.in"), fUnEncrypted("P6617.unlock.in"), fans("P6617.ans"), fhelp("P6617.help");
    int n = 6, m = 3, w = 5;
    uniform_int_distribution<int> wGen(1, w), nGen(1, n), optGen(1, 2);
    fin << n << ' ' << m << ' ' << w << '\n';
    vector<int> a(n);
    for (int &x : a) {
        x = wGen(rng);
        fin << x << ' ';
    } fin << '\n';
    for (int i = 1, cnt = 0; i <= m; i++) {
        int opt = optGen(rng);
        fin << opt << ' ';
        if (opt == 2) {
            int l = nGen(rng), r = nGen(rng);
            if (l > r) swap(l, r);
            fin << (l^cnt) << ' ' << (r^cnt) << '\n';
            fUnEncrypted << "Query " << i << ": " << l << ' ' << r << endl;
            bool flg = false;
            for (int j = l; !flg && j < r; j++)
                for (int k = j+1; !flg && k <= r; k++)
                    if (a[j-1] + a[k-1] == w) {
                        fhelp << "At Query " << i << ", pos " << j << " and " << k << "'s sum is w\n";
                        flg = true;
                    }
            fans << (flg ? "Yes" : "No") << '\n';
            cnt += flg;
        } else {
            int p = nGen(rng), v = wGen(rng);
            fin << p << ' ' << v << '\n';
            a[p-1] = v;
            fhelp << "After Modify " << i << ",";
            for (int x : a) fhelp << ' ' << x;
            fhelp << '\n';
        }
    }
    return 0;
}