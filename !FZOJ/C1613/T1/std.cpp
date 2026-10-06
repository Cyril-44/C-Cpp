#include<bits/stdc++.h>
using namespace std;

const int maxn = 1 << 20;
char in[maxn],out[maxn],*p1=in,*p2=in,*p3=out;
#define getchar() (p1==p2&&(p2=(p1=in)+fread(in,1,maxn,stdin),p1==p2)?EOF:*p1++)
#define flush() (fwrite(out,1,p3-out,stdout))
#define putchar(x) (p3==out+maxn&&(flush(),p3=out),*p3++=(x))
template<typename type>
void read(type &x)
{
    x = 0;
    int f = 1;
    char c = getchar();
    while (c < 48 || c > 57) 
    {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= 48 && c <= 57)
        x = x * 10 + c - 48, c = getchar();
    x *= f;
}

template<typename type, typename ...T>
void read(type &x, T &...y)
{
    read(x), read(y...);
}

template<typename type>
inline void write(type x,bool mode=1)//0为空格，1为换行
{
    x < 0 ? x =- x, putchar('-') : 0;
    static short Stack[50], top(0);
    do Stack[++top]=x%10,x/=10; while(x);
    while(top) putchar(Stack[top--]|48);
    mode?putchar('\n'):putchar(' ');
}

typedef long long ll;
const int N=500010,mod=1e9+7;
int n,b[N<<1];
struct Node{
    int l,r;
}a[N];
int cnt[N<<1];
ll pw[N<<1];
int st[N<<1];
int main(){
    read(n);
    for(int i=1;i<=n;i++){
        read(a[i].l,a[i].r);
        b[2*i-1]=a[i].l; b[2*i]=a[i].r;
    }
    pw[0]=1;
    for(int i=1;i<=n;i++) pw[i]=pw[i-1]*2%mod;
    sort(b+1,b+n+n+1);
    int m=unique(b+1,b+n+n+1)-b-1;
    for(int i=1;i<=n;i++){
        a[i].l=lower_bound(b+1,b+m+1,a[i].l)-b;
        a[i].r=lower_bound(b+1,b+m+1,a[i].r)-b;
        cnt[a[i].l]++; cnt[a[i].r+1]--;
        st[a[i].l]++;
    }
    for(int i=1;i<=m+1;i++) cnt[i]+=cnt[i-1];
    ll ans=0;
    for(int i=1;i<=m;i++)
        (ans+=(pw[st[i]]+mod-1)*pw[n-cnt[i]])%=mod;
    printf("%lld",ans);
    return 0;
}