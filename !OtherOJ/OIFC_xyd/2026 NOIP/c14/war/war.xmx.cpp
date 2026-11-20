#include<bits/stdc++.h>
using namespace std;
int tid,T;
int n;
pair<int,int> srt[5005];
vector<int> g[5005];
int gn;
int li[5005],ri[5005];
int s[5005],sn;
int ls[5005][5005],lsn[5005];
int buc[5005];
int dt[5005][5005],dp[5005][5005];
int ldp[5005][5005];
int sz[5005];
void dfs(int x)
{
    int l=g[x].size();
    sz[x]=1;
    for(int i=0;i<l;i++)
        dfs(g[x][i]),sz[x]+=sz[g[x][i]];
    for(int i=1;i<=n;i++)
        buc[i]=0;
    buc[1]=ri[x]-li[x]+1;
    for(int i=0;i<l;i++)
    {
        int y=g[x][i];
        buc[1]-=lsn[y];
        for(int j=1;j<=lsn[y];j++)
            buc[ls[y][j]+1]++;
    }
    lsn[x]=0;
    for(int i=1;i<=n;i++)
    {
        while(buc[i]--)
            ls[x][++lsn[x]]=i;
    }
    int cnt=0;
    for(int i=lsn[x]-1;i>=0;i--)
    {
        cnt+=ls[x][lsn[x]-i];
        dt[x][i]=cnt;
    }
    for(int i=lsn[x];i<=n;i++)
        dt[x][i]=0;
    ldp[0][0]=0;
    cnt=0;
    for(int i=1;i<=l;i++)
    {
        int y=g[x][i-1];
        for(int j=0;j<=cnt+sz[y];j++)
            ldp[i][j]=1e9;
        for(int j=0;j<=cnt;j++)
            for(int k=0;k<=sz[y];k++)
                ldp[i][j+k]=min(ldp[i][j+k],ldp[i-1][j]+dp[y][k]);
        cnt+=sz[y];
    }
    dp[x][0]=1e9;
    for(int i=0;i<=cnt;i++)
        dp[x][i+1]=ldp[l][i];
    for(int i=cnt+2;i<=n+1;i++)
        dp[x][i]=0;
    if(x==0)
        return;
    ldp[0][0]=lsn[x];
    cnt=0;
    for(int i=1;i<=l;i++)
    {
        int y=g[x][i-1];
        for(int j=0;j<=cnt+sz[y];j++)
            ldp[i][j]=1e9;
        for(int j=0;j<=cnt;j++)
            for(int k=0;k<=sz[y];k++)
                ldp[i][j+k]=min(ldp[i][j+k],ldp[i-1][j]+min(dp[y][k],dt[y][k]-min(k,lsn[y])));
        cnt+=sz[y];
    }
    for(int i=0;i<=sz[x];i++)
        dp[x][i]=min(dp[x][i],ldp[l][i]);
}
int main()
{
    //freopen("8.in","r",stdin);
    //freopen("8.out","w",stdout);
    ios::sync_with_stdio(false);
    cin.tie(0);
    cin>>tid>>T;
    while(T--)
    {
        cin>>n;
        for(int i=1;i<=n;i++)
        {
            int l,r;
            cin>>l>>r;
            srt[i]=make_pair(l,-r);
            g[i].clear();
        }
        g[0].clear();
        sort(srt+1,srt+n+1);
        gn=0;
        li[0]=1,ri[0]=n;
        sn=1,s[sn]=0;
        for(int i=1;i<=n;i++)
        {
            srt[i].second=-srt[i].second;
            while(ri[s[sn]]<srt[i].first)
                sn--;
            gn++;
            li[gn]=srt[i].first,ri[gn]=srt[i].second;
            g[s[sn]].push_back(gn);
            s[++sn]=gn;
        }
        /*for(int i=0;i<=n;i++)
        {
            cout<<li[i]<<' '<<ri[i]<<"  ";
            for(int j=0;j<g[i].size();j++)
                cout<<g[i][j]<<' ';
            cout<<'\n';
        }
        cout<<endl;*/
        dfs(0);
        for(int i=1;i<=n+1;i++)
            cout<<dp[0][i]<<' ';
        cout<<'\n';
    }
    return 0;
}
