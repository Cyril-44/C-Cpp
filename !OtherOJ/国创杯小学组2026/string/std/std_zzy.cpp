#include<bits/stdc++.h>
using namespace std;
const int N=5e6+5,inf=1e9+7;
int n,dp[N],f[N];
namespace PAM{
    // const int N=2e5+5;
    int cnt,lst,tot;
    struct PAM_node{int ch[26],len,fail,slink,df;}tr[N];
    char s[N],t[N];
    int New(int l){
        tr[++cnt].len=l;
        memset(tr[cnt].ch,0,sizeof(int)*26);
        return cnt;
    }
    void clear(){
        cnt=-1,s[tot=0]='#',lst=0;
        f[0]=f[1]=inf;
        New(0),New(-1),tr[0].fail=1;
    }
    int getfail(int u){
        if(s[tot]>s[tot-1]) return 1;
        while(s[tot]!=s[tot-tr[u].len-1]) u=tr[u].fail;return u;
    }
    int ins(char c){
        s[++tot]=c;
        int now=getfail(lst);
        if(!tr[now].ch[c-'a']){
            int x=New(tr[now].len+2);
            tr[x].fail=tr[getfail(tr[now].fail)].ch[c-'a'];
            tr[now].ch[c-'a']=x;
            tr[x].df=tr[x].len-tr[tr[x].fail].len;
            tr[x].slink=tr[x].df==tr[tr[x].fail].df?tr[tr[x].fail].slink:tr[x].fail;
        }
        return lst=tr[now].ch[c-'a'];
    }
    void wk(){
        clear();
        // int n;
        // scanf("%d", &n);
        // static int arr[N];
        // for (int i = 1; i <= n; i++) scanf("%d", &arr[i]);
        // for (int i = 1; i <= n; i++) t[i] = arr[i] + 'a' - 1;
        scanf("%s", t+1);
        n = strlen(t+1);
        for(int i=1;i<=n;++i){
            ins(t[i]);
            dp[i]=inf;
            for(int u=lst;u>1;u=tr[u].slink){
                f[u]=dp[i-tr[tr[u].slink].len-tr[u].df];
                if(tr[u].slink!=tr[u].fail) f[u]=min(f[u],f[tr[u].fail]);
                dp[i]=min(dp[i],f[u]+1);
            }
        }
        printf("%d\n",dp[n]);
    }
}
int main(){
    int t;scanf("%*d%d",&t);
    while(t--) PAM::wk();
    return 0;
}
