#include <bits/stdc++.h>
#include "findstring.h"
namespace GRADER
{
    using namespace std;
    const int N = 10000010, L = 10000000;
    int str[N], n, qrycnt;
    int compare(int x, int y)
    {
        qrycnt++;
        if (x < 1 || y < 1 || x > n || y > n || qrycnt > L)
        {
            cout << "Invalid query." << endl;
            exit(1);
        }
        return str[x] < str[y] ? -1 : (str[x] == str[y] ? 0 : 1);
    }
    bool ansa[N], ansb[N];
    int cnta, cntb;
    void work()
    {
        ios::sync_with_stdio(0);
        cin.tie(0);
        cout.tie(0);
        int c, t, x, maxl = 0;
        cin >> c >> t;
        init(c, t);
        pair<vector<int>, vector<int>> res;
        while (t--)
        {
            cin >> n;
            qrycnt = 0;
            for (int i = 1; i <= n; i++)
                cin >> str[i];
            res = find_string(n);
            maxl = max(maxl, qrycnt);
            cout << res.first.size() << ' ';
            for (int x : res.first)
                ansa[x] = 1;
            for (int i = 1; i <= n; i++)
                if (ansa[i])
                    cout << i << ' ', ansa[i] = 0;
            cout << endl;
            cout << res.second.size() << ' ';
            for (int x : res.second)
                ansb[x] = 1;
            for (int i = 1; i <= n; i++)
                if (ansb[i])
                    cout << i << ' ', ansb[i] = 0;
            cout << endl;
        }
        cerr << "Max query: " << maxl << endl;
    }
};
int compare(int x, int y) { return GRADER::compare(x, y); }
int main()
{
    GRADER::work();
    return 0;
}