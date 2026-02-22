#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const int N=100010;
int n,a[N],cnt[N]; ll k;
vector<int> lis[N];
int rgl[N],rgr[N];
int getcnt(int val,int cnt){
    int lst=0; ll ans=0;
    for(int i=0;i<lis[val].size()-cnt+1;i++){
        for(int j=lst+1;j<=lis[val][i];j++)
            ans+=max(rgr[j]-max(lis[val][i+cnt-1],rgl[j])+1,0);
        lst=lis[val][i];
    }
    // printf("%d %d %lld\n",val,cnt,ans);
    return ans;
}
int main(){
    scanf("%d%lld",&n,&k);
    for(int i=1;i<=n;i++){
        scanf("%d",&a[i]);
        lis[a[i]].push_back(i);
        cnt[i]=lis[a[i]].size();
    }
    for(int i=1;i<=n;i++) rgl[i]=i,rgr[i]=n;
    for(int x=1;x<=n;x++){
        int l=1,r=lis[x].size(),m;
        while(l<=r){
            int m=l+r>>1;
            if(getcnt(x,m)<=k) r=m-1;
            else l=m+1;
        }
        for(int j=1;j<=r;j++) printf("%d ",x);
        k-=getcnt(x,l);
        int lst=0;
        for(int i=0;i<lis[x].size();i++){
            for(int j=lst+1;j<=lis[x][i];j++){
                rgr[j]=min(rgr[j],(i+r>=lis[x].size()?n+1:lis[x][i+r])-1);
                rgl[j]=max(rgl[j],(i+r-1>=lis[x].size()?n+1:lis[x][i+r-1]));
            }
            lst=lis[x][i];
        }
    }
    return 0;
}