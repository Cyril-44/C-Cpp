#include <bits/stdc++.h>
using namespace std;

struct node{
    long long l,r;
}a[500005];
int main(){
    freopen("candy.in","r",stdin);
    freopen("candy.out","r",stdout);
    long long sum1=0,sum2=0;
    long long n,m;
    scanf("%lld%lld",&n,&m);
    for(int i=1;i<=n;i++) scanf("%lld%lld",&a[i].l,&a[i].r);
    for(int i=1;i<=n;i++){
        sum1+=a[i].l;
        sum2+=a[i].r;
    }
    if (sum1>m||sum2<m) printf("-1\n");
    else{
        long long x=0,ans=0;
        for(int i=1;i<=n;i++){
            if (a[i].r<m/n) x+=a[i].r,ans+=(m/n-a[i].r);
            else if (a[i].l>m/n) x+=a[i].l,ans+=(a[i].l-m/n);
            else x+=(m/n);
        }
        cout<<ans+abs(x-m)<<endl;
    }
    return 0;
}
