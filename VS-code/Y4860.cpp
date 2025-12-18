#include<bits/stdc++.h>
using namespace std;
int n,m,a[505],b[505];
bool cmp(int x,int y){
    return x>y;
}
int main(){
    cin>>n>>m;
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
    }
    for(int i=1;i<=n;i++){
        scanf("%d",&b[i]);
    }
    printf("%d\n1 %d I\n",n,n);
    sort(a+1,a+n+1);
    for(int i=1;i<n;i++){
        int mi=a[i],ma=a[i];
        for(int j=i;j<=n;j++){
            mi=min(mi,a[j]);
            ma=max(ma,a[j]);
            if(a[j]==b[i]){
                if(mi==a[j]){
                    printf("%d %d I\n",i,j);
                    sort(a+i,a+j+1);
                }
                else{
                    printf("%d %d D\n",i,j);
                    sort(a+i,a+j+1,cmp);
                }
                break;
            }
        }
    }
    return 0;
}