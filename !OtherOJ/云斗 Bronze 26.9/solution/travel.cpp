#include <bits/stdc++.h>
using namespace std;

int a[1000005];
int main(){
   freopen("travel.in","r",stdin);
   freopen("travel.out","w",stdout);
    int n,k;
    cin>>n>>k; 
    for(int i=1;i<=n;i++) cin>>a[i];
    long long cnt=0;
    for(int i=1;i<=n;i++) cnt=cnt+max(0,(a[i]^k)-a[i]);
    cout<<cnt<<endl;
    return 0;
}