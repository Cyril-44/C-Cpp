#include<bits/stdc++.h>
using namespace std;
#define ll unsigned long long
#define lll __int128
lll C[150][150];
void out(lll x){
    if(!x) return;
    out(x/10);
    putchar(x%10+48);
}
void init()
{
    C[0][0]=1;
    for(int i=1;i<=128;i++)
    {
        C[i][0]=1;
        for(int j=1;j<=i;j++)
            C[i][j]=C[i-1][j-1]+C[i-1][j];
    }
}
int t;
lll cal(int k,ll m)
{
    lll ans=0;
    while(k)
    {
        int pos=k;
        while(C[pos][k]<=m){
            ++pos;
        }
        --pos;
        ans|=(lll)(1)<<pos;
        m-=C[pos][k];
        k--;
    }
    return ans;
}
int main()
{
    init();
    scanf("%d",&t);
    while(t--)
    {
        ll m;int k;
        scanf("%llu%d",&m,&k);
        --k;
        if(!k)
            printf("1 -1\n");
        else
        {
            lll r=cal(k,m),l=0;
            if(m) l=cal(k,m-1);
            out(l+1);
            putchar(' ');
            out(r-l);
            putchar('\n');
        }
    }
    return 0;
}
