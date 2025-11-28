#include <bits/stdc++.h>
using namespace std;
const int W = 20;
const int X = 100;
const int Y = 1000;
const int Z = 500000;
const int M = 1000000000;
int n[30] = {0, 90, 92, 95, 99, Z, Z, Z, Z, W, W, W, W, Y, Y, Y, Y, Z, Z, Z, Z, Z, Z, Z, Z, Z};
int r[30] = {0, 90, 92, 95, 99, M, M, M, M, M, M, M, M, Y, Y, Y, Y, M, M, M, M, M, M, M, M, M};
int s[30] = {0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
string inputName(int id) { return "datU470736/upanddown" + to_string(id) + ".in"; }
string outputName(int id) { return "datU470736/upanddown" + to_string(id) + ".out"; }
void runCode(string i, string o)
{
    string s = "./U470736<" + i + ">" + o;
    system(s.data());
}
unordered_map<int, bool> vis;
int a[500010];
int main()
{
    string i, o;
    FILE *f;
    mt19937 rnd(time(0));
    for (int T = 1; T <= 16; T++)
    {
        printf("Generating Test %d\n", T);
        vis.clear();
        vis[0] = 1;
        i = inputName(T);
        o = outputName(T);
        f = fopen(i.data(), "w");
        fprintf(f, "%d\n", n[T]);
        memset(a, 0, sizeof(a));
        if (s[T])
        {
            if (n[T] != r[T])
            {
                printf("0\n");
                for (int i = 1; i <= n[T]; i++)
                {
                    while (vis.count(a[i]))
                        a[i] = rnd() % r[T] + 1;
                    vis[a[i]] = 1;
                }
                sort(a + 1, a + n[T] + 1);
            }
            else
                for (int i = 1; i <= n[T]; i++)
                    a[i] = i;
        }
        else
        {
            if (n[T] != r[T])
            {
                printf("1\n");
                for (int i = 1; i <= n[T]; i++)
                {
                    while (vis.count(a[i]))
                        a[i] = rnd() % r[T] + 1;
                    vis[a[i]] = 1;
                }
            }
            else
            {
                for (int i = 1; i <= n[T]; i++)
                    a[i] = i;
                shuffle(a + 1, a + n[T] + 1, rnd);
            }
        }
        for (int i = 1; i <= n[T]; i++)
            fprintf(f, "%d ", a[i]);
        fclose(f);
        runCode(i, o);
    }
    return 0;
}