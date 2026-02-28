#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,a[30],c[30],f[1<<23];
ll cnt[1<<23];
int main(){
    cin>>n;
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
        c[i]=a[i]-a[i-1];
    }
    for(int i=0;i<(1<<n);i++){
        for(int j=1;j<=n;j++){
            if((i>>j-1)&1){
                cnt[i]+=c[j];
            }
        }
    }
    for(int i=0;i<(1<<n);i++){
        if(i&&!cnt[i]) f[i]++;
        for(int j=1;j<=n;j++){
            if(!((i>>j-1)&1)){
                f[i|(1<<j-1)]=max(f[i|(1<<j-1)],f[i]);
            }
        }
    }
    cout<<n-f[(1<<n)-1];
    return 0;
}