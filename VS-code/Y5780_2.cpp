#include<bits/stdc++.h>
using namespace std;
int t,n;
double ans;
struct node{
    int x,y,c;
}a[3005];
int main(){
    freopen("1.in","r",stdin);
    freopen("2.out","w",stdout);
    cin>>t;
    while(t--){
        cin>>n;
        for(int i=1;i<=n;i++){
            scanf("%d%d%d",&a[i].x,&a[i].y,&a[i].c);
        }
        ans=0;
        for(int i=1;i<=n;i++){
            if(a[i].c==0){
                for(int j=1;j<=n;j++){
                    if(a[j].c==1){
                        for(int k=1;k<=n;k++){
                            if(a[k].c==2){
                                long long cnt=1ll*a[k].x*(a[i].y-a[j].y)-1ll*a[k].y*(a[i].x-a[j].x)+1ll*a[i].x*a[j].y-1ll*a[j].x*a[i].y;
                                ans=max(ans,abs(cnt)/2.0);
                            }
                        }
                    }
                }
            }
        }
        printf("%.1lf\n",ans);
    }
    return 0;
}