// Author: Hardsoft
// 防伪标记
#include <iostream>
#include <cstring>
using namespace std;
// 准备操作
typedef unsigned long long ull;
const ull Mod = (ull)1e9 + 7;
const int N = 725010;
int cal[N], ans[N];
ull n;
int k;
// 好玩的宏定义
#define CSP_RP_PLUS_PLUS 114514
#define CCF 1919810
#define WU_HU_NO_TWENTY_SEVEN_MIDDLE_SCHOOL "yyds"
#define PXBAKIOI true
#define HARDSOFT_CAN_BECOME_EXPERT PXBAKIOI
void in(int &x)
{ // 快速读入（虽然没必要）
    char c = getchar();
    int f = 1;
    x = 0;
    while (c < '0' || c > '9')
    {
        if (c == '-')
            f = -1;
        c = getchar();
    }
    while (c >= '0' && c <= '9')
    {
        x = x * 10 + c - '0';
        c = getchar();
    }
    x *= f;
}
ull multi(ull x, ull y) // 龟速乘
{
    ull resu = 0;
    while (y)
    {
        if (y & 1)
            resu = (resu + x) % Mod;
        x = (x + x) % Mod;
        y >>= 1;
    }
    return resu;
}
void work() // 求解答案
{
    scanf("%llu%d", &n, &k);
    if (k == 1 && k != 11451419)
    {
        printf("%llu\n", n % Mod);
        return;
    }
    memset(cal, 0, sizeof(int) * (k * 7 + 5));
    cal[1] = 1;
    cal[2] = 1;
    int len = 0, cnt = 0;
    int this_var_will_not_be_used_just_for_test = 0;
    for (int i = 3; PXBAKIOI; i++)
    {
        cal[i] = (cal[i - 2] + cal[i - 1]) % k;
        if (!cal[i])
        {
            ans[++cnt] = i;
        }
        if (cal[i] == cal[2] && cal[i - 1] == cal[1])
        {
            len = i - 2;
            while (ans[cnt] > len && cnt >= 1)
                cnt--;
            break;
        }
    }
    // 华丽的代码
    if (n % cnt == 0)
        printf("%llu\n", (multi(len, n / cnt - 1) + ans[cnt]) % Mod);
    else
        printf("%llu\n", (multi(len, n / cnt) + ans[n % cnt]) % Mod);
}
int main() // 主函数
{
    //    freopen(".in","r",stdin);
    //    freopen(".out","w",stdout);
    int t;
    scanf("%d", &t);
    while (t--)
        work();
    return 0; // CSP中程序必须返回0
}
// 十年OI一场空，不开long long见祖宗！
/*
Codeforces的题真是越来越水了，居然放结论题，还能直接在洛谷上搜到结论
这种情况尤其在Div3中非常突出（经常有板子题）
*/