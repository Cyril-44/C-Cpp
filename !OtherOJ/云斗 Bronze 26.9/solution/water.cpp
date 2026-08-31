#include <bits/stdc++.h>
using namespace std;

vector<int>a[200005];
int n;
int rt[200005];
int t[200005],s[200005];
void dfs(int w,int f,int x){
    rt[w]=1;
    for(int i=0;i<a[w].size();i++){
        if (a[w][i]==f)  continue;
        dfs(a[w][i],w,x);
        if (rt[a[w][i]]==0) continue;
        rt[w]+=(rt[a[w][i]]+1);
    }
    if (t[w]<=x) rt[w]=max(0,rt[w]-s[w]);
}
bool check(int x){
    memset(rt,0,sizeof(rt));
    dfs(1,0,x);
    return rt[1];
}
int main(){
    freopen("water.in","r",stdin);
    freopen("water.out","w",stdout);
    ios::sync_with_stdio(false); 
    cin>>n;
    for(int i=1;i<n;i++){
        int u,v;
        cin>>u>>v;
        a[u].push_back(v);
        a[v].push_back(u);
    }
    for(int i=1;i<=n;i++){
        cin>>t[i]>>s[i];
    }
    int l=1,r=n;
    while(l<r){
        int mid=(l+r+1)/2;
        if(check(mid)) l=mid;
        else r=mid-1; 
    }
    cout<<l+1<<endl;
    return 0;
}
