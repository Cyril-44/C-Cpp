#include "testlib.h"
#include <bits/stdc++.h>
using namespace std;
typedef long long ull;
constexpr int TestCases = 50;
constexpr ull AMAX = 5000;
constexpr ull BMAX = 5000000;
constexpr ull FMAX = 10000000000000000ll;
constexpr ull CMAX = 2000000000000000000ll;
constexpr std::pair<int,ull> N[] = {
    {100,CMAX},{100,CMAX},{100,CMAX},{100,CMAX},                            //subtask 1 (4 testcases) 
    {1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},//subtask 2 (6 testcases) 
    {1600,AMAX},{1600,AMAX},{1600,AMAX},{1600,AMAX},                        //subtask 3 (4 testcases) 
    {1600,BMAX},{1600,BMAX},{1600,BMAX},{1600,BMAX},{1600,BMAX},{1600,BMAX},//subtask 4 (6 testcases) 
    {1600,FMAX},{1600,FMAX},{1600,FMAX},{1600,FMAX},{1600,FMAX},{1600,FMAX},//subtask 5 (6 tastcases) 
    {1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},
    {1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},
    {1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},
    {1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},{1600,CMAX},//subtask 6 (24 testcases) 
};
void printTestCase(int testCase, const std::vector<ull> &b)
{
    ++testCase;
    std::ofstream os("sequence" + std::to_string(testCase) + ".in");
    os << b.size() << std::endl;
    for (ull c : b)
        os << c << ' ';
    os << std::endl;
    os.close();
    os.open("sequence" + to_string(testCase) + ".out");
    os.close();
}
int main(int argc, char **argv)
{
    registerGen(argc, argv, 1);
    for (int i = 0; i < TestCases; i++)
    {
        int n = N[i].first;
        vector<ull> res;
        if (i >= 4 && i < 10)
        {
            int x;
            unordered_map<ull, int> mp;
            vector<int> ss;
            for (int i = 0; i < 100; i++)
            {
                while (mp.count(x = rnd.next(n - 1) + 1))
                    ;
                mp[x] = 1;
                ss.push_back(x);
            }
            ull ori = rnd.next(CMAX);
            int tm = rnd.any(ss.begin(), ss.end());
            for (int i = 0; i < n; i++)
            {
                if (mp.count(i))
                    ori = rnd.next(0ll, tm == i ? 0ll : CMAX);
                res.push_back(ori);
            }
        }
        else
        {
            if (N[i].second == AMAX)
            {
                for (int j = 0; j < n; j++)
                    res.push_back(rnd.next(1ll, N[i].second));
            }
            else
            {
                int tm = rnd.next(n);
                for (int j = 0; j < n; j++)
                    res.push_back(rnd.next(0ll, tm == j ? 0ll : N[i].second));
            }
        }
        printTestCase(i, res);
    }
    return 0;
}