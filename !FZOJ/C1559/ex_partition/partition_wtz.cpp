#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
const int N=1000010;
int n; char s[N];
bool a[N];
int pre[2][N];
int cnt[2][N];
bool havesol(int len){
    memset(cnt[0],0,sizeof(int)*(n+1));
    memset(cnt[1],0,sizeof(int)*(n+1));
    cnt[0][0]=1; int val;
    for(int i=1;i<=n;i++){
        cnt[0][i]=cnt[0][i-1];
        cnt[1][i]=cnt[1][i-1];
        val=cnt[a[i]^1][i-1];
        if(i>len) val-=cnt[a[i]^1][i-len-1];
        if(val) cnt[a[i]][i]++;
    }
    return cnt[0][n]+cnt[1][n]-cnt[0][n-1]-cnt[1][n-1];
}
int solve(int len){
    int cur[2]={0,pre[1][len]},ncur[2],ans=0;
    // printf("(0,%d)",pre[1][len]);
    if(pre[1][len]<0) return -1;
    while(max(cur[0],cur[1])<n){
        ncur[0]=pre[0][min(cur[1]+len,n)];
        ncur[1]=pre[1][min(ncur[0]+len,n)];
        // printf("(%d,%d)",ncur[0],ncur[1]);
        if(cur[0]==ncur[0]&&cur[1]==ncur[1]) return -1;
        // if(ncur[0]>ncur[1]) return -1;
        cur[0]=ncur[0]; cur[1]=ncur[1];
        ans+=2;
    }
    return a[n]?ans+1:ans;
}
void work(){
    scanf("%s",s+1);
    n=strlen(s+1);
    for(int i=1;i<=n;i++)
        a[i]=a[i-1]^(s[i]&1);
    pre[0][0]=0;
    for(int i=1;i<=n;i++){
        if(a[i]) pre[0][i]=pre[0][i-1];
        else pre[0][i]=i;
    }
    pre[1][0]=INT_MIN;
    for(int i=1;i<=n;i++){
        if(a[i]) pre[1][i]=i;
        else pre[1][i]=pre[1][i-1];
    }
    // for(int i=0;i<=n;i++){
    //     printf("(%d,%d)\n",pre[0][i],pre[1][i]);
    // }
    int l=1,r=n,m;
    while(l<=r){
        int m=l+r>>1;
        if(havesol(m)) r=m-1;
        else l=m+1;
    }
    for(int i=1;i<=r;i++) printf("-1 ");
    for(int i=l;i<=n;i++) printf("%d ",solve(i)); putchar('\n');
}
int main(){
    int t;
    scanf("%d",&t);
    while(t--) work();
    return 0;
}