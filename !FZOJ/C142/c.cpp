#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
const int N=1000010,ma=1000000007,mb=998244353;
char str[N<<1]; int n;
char ans[N];
ll ksm(ll a,ll b,ll mod){
    ll res=1;
    while(b){
        if(b&1) res=res*a%mod;
        a=a*a%mod; b>>=1;
    }
    return res;
}
ll mi31[N],mi131[N];
ll hsa[N<<1],hsb[N<<1];
int main(){
    // freopen("c.in","r",stdin);
    // freopen("c.out","w",stdout);
    mi31[0]=mi131[0]=1;
    for(int i=1;i<=1000000;i++)
        mi31[i]=mi31[i-1]*31%ma,mi131[i]=mi131[i-1]*131%mb;
    for(int i=1;~scanf("%s",str+1);i++){
        printf("Case %d: ",i);
        n=strlen(str+1);
        for(int j=n+1;j<=(n<<1);j++)
            str[j]=str[j-n];
        for(int j=0;j<n;j++) ans[j]='0';
        ans[n]='\0';
        for(int j=1;j<=n;j++){
            hsa[j]=(hsa[j-1]*31+str[j])%ma;
            hsb[j]=(hsb[j-1]*131+str[j])%mb;
        }
        vector<int> g;
        for(int i=1;i<n;i++)
            if(str[i]==str[i+1]) g.push_back(i);
        if(str[1]==str[n]) g.push_back(n);
        if(g.empty()){
            int len=-1;
            for(int j=1;j<=n;j++){
                if(n%j) continue;
                if(hsa[j]*(mi31[n]+ma-1)%ma*ksm(mi31[j]+ma-1,ma-2,ma)%ma==hsa[n])
                    if(hsb[j]*(mi131[n]+mb-1)%mb*ksm(mi131[j]+mb-1,mb-2,mb)%mb==hsb[n]){
                        len=j;
                        break;
                    }
            }
            if(len==-1)
                for(int j=0;j<n;j++) putchar('1');
            else
                for(int j=0;j<n;j++){
                    if(j==n-1||j%len!=len-1) putchar('1');
                    else putchar('0');
                }
        }else{
            for(int i=0;i<g.size();i++){
                int l=g[i]+1,r=(i==g.size()-1)?g[0]+n:g[i+1];
                bool tag=0;
                if(str[l]==str[r]) r--,tag=1;
                if(l>r) continue;
                // printf("(%d,%d)",l,r);
                hsa[l-1]=hsb[l-1]=0;
                for(int j=l;j<=r;j++){
                    hsa[j]=(hsa[j-1]*31+str[j])%ma;
                    hsb[j]=(hsb[j-1]*131+str[j])%mb;
                }
                ans[n-(r-l+1)]='1';
                for(int j=l;j<=r;j++){
                    if(hsa[j]!=(hsa[r]+ma-hsa[r+l-j-1]*mi31[j-l+1]%ma)%ma)
                        ans[n-(r-j)-1]='1';//,printf("%d ",j);
                    else if(hsb[j]!=(hsb[r]+mb-hsb[r+l-j-1]*mi131[j-l+1]%mb)%mb)
                        ans[n-(r-j)-1]='1';//,printf("%d ",j);
                    // else printf("len %d bad\n",j-l+1);
                }
                if(tag){
                    l++; r++;
                hsa[l-1]=hsb[l-1]=0;
                for(int j=l;j<=r;j++){
                    hsa[j]=(hsa[j-1]*31+str[j])%ma;
                    hsb[j]=(hsb[j-1]*131+str[j])%mb;
                }
                ans[n-(r-l+1)]='1';
                for(int j=l;j<=r;j++){
                    if(hsa[j]!=(hsa[r]+ma-hsa[r+l-j-1]*mi31[j-l+1]%ma)%ma)
                        ans[n-(r-j)-1]='1';//,printf("%d ",j);
                    else if(hsb[j]!=(hsb[r]+mb-hsb[r+l-j-1]*mi131[j-l+1]%mb)%mb)
                        ans[n-(r-j)-1]='1';//,printf("%d ",j);
                    // else printf("len %d bad\n",j-l+1);
                }
                }
            }
            ans[n]='\0';
            ans[n-1]='1';
            printf("%s",ans);
        }
        putchar('\n');
    }
}