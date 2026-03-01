#include<bits/stdc++.h>
using namespace std;
int t,n,m,fa1[300005],fa2[300005],a[300005],ans[3][300005];
int find1(int x){
    if(x==fa1[x]) return x;
    return fa1[x]=find1(fa1[x]);
}
int find2(int x){
    if(x==fa2[x]) return x;
    return fa2[x]=find2(fa2[x]);
}
int main(){
    cin>>t;
    while(t--){
        scanf("%d%d",&n,&m);
        for(int i=1;i<=n;i++){
            fa1[i]=fa2[i]=i;
        }
        int cntt=0;
        for(int i=1;i<=m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            int fx=find1(x),fy=find1(y);
            if(fx!=fy){
                cntt++;
                fa1[fy]=fx;
            }
            else{
                fx=find2(x),fy=find2(y);
                if(fx==fy) continue;
                fa2[fy]=fx;
            }
        }
        int cnt=0,cnt1=0,cnt2=0,cnt3=0;
        if(cntt<n-1){
            for(int i=1;i<=n;i++){
                if(fa1[i]==i){
                    a[++cnt]=i;
                }
            }
            for(int i=1;i<=n;i++){
                int fx=find1(i);
                if(fx==a[1]) ans[0][++cnt1]=i;
                else ans[1][++cnt2]=i;
            }
            if(cnt1>1){
                printf("%d %d\n%d ",1,ans[0][1],cnt1-1);
                for(int i=2;i<=cnt1;i++){
                    printf("%d ",ans[0][i]);
                }
                printf("\n%d ",cnt2);
                for(int i=1;i<=cnt2;i++){
                    printf("%d ",ans[1][i]);
                }
                printf("\n");
            }
            else{
                printf("%d ",cnt1);
                for(int i=1;i<=cnt1;i++){
                    printf("%d ",ans[0][i]);
                }
                printf("\n%d %d\n%d ",1,ans[1][1],cnt2-1);
                for(int i=2;i<=cnt2;i++){
                    printf("%d ",ans[1][i]);
                }
                printf("\n");
            }
            continue;
        }
        for(int i=1;i<=n;i++){
            if(fa2[i]==i) a[++cnt]=i;
        }
        for(int i=1;i<=n;i++){
            int fx=find2(i);
            if(fx==a[1]) ans[0][++cnt1]=i;
            else if(fx==a[2]) ans[1][++cnt2]=i;
            else ans[2][++cnt3]=i;
        }
        printf("%d ",cnt1);
        for(int i=1;i<=cnt1;i++){
            printf("%d ",ans[0][i]);
        }
        printf("\n");
        printf("%d ",cnt2);
        for(int i=1;i<=cnt2;i++){
            printf("%d ",ans[1][i]);
        }
        printf("\n");
        printf("%d ",cnt3);
        for(int i=1;i<=cnt3;i++){
            printf("%d ",ans[2][i]);
        }
        printf("\n");
    }
    return 0;
}