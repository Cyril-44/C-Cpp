#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m,a[100005],b[100005],c[100005];
ll f[1005][105][105],ans;
int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    bool ok=1;
    for(int i=1;i<=n;i++){
        scanf("%d",&b[i]);
        if(b[i]<b[i-1]) ok=0;
    }
    if(ok){
        for(int i=1;i<=n;i++){
            c[i]=a[i]+b[i];
        }
        sort(c+1,c+n+1);
        for(int i=1;i<=m;i++){
            ans+=c[i];
        }
        cout<<ans;
        return 0;
    }
    memset(f,0x3f,sizeof(f));
    f[0][0][0]=0;
    for(int i=1;i<=n;i++){
        for(int j=0;j<=m&&j<=i;j++){
            for(int k=0;k<=m&&k<=n-i;k++){
                f[i][j][k]=min(f[i-1][j][k],f[i-1][j][k+1]+b[i]);
                if(j){
                    f[i][j][k]=min(f[i][j][k],f[i-1][j-1][k]+a[i]+b[i]);
                    if(k) f[i][j][k]=min(f[i][j][k],f[i-1][j-1][k-1]+a[i]);
                }
            }
        }
    }
    cout<<f[n][m][0];
    return 0;
}
/*
f[i][j][k]=f[i-1][j][k],f[i-1][j][k+1]+b[i],f[i-1][j-1][k]+a[i]+b[i],f[i-1][j-1][k-1]+a[i]

b1<=b2<=b3<=b4……<=bn

36pts
*/