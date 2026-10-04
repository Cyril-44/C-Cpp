#include <bits/stdc++.h>
using namespace std;

const int N=1e5+5,M=2e6+5;
struct DSU{
    int f[N],siz[N],n;
    void init(){for(int i=0;i<=n;i++) siz[i]=1,f[i]=i;}
    int find(int x){if (x==f[x]) return x;else return f[x]=find(f[x]);}
    void merge(int u,int v){
        int x=find(u),y=find(v);
        if (x!=y){
            if (siz[x]<siz[y]) swap(x,y);
            siz[x]+=siz[y];f[y]=x;
        }
    }
}dsu;
void merge(int A[],int B[],int &la,int lb){
    int C[20],lc=0;
    int i=0,j=0;
    while(i<la&&j<lb){
        if(A[i]<B[j]) C[lc++]=A[i++];
        else if(A[i]>B[j]) C[lc++]=B[j++];
        else {i++;j++;}
    }
    while(i<la) C[lc++]=A[i++];
    while(j<lb) C[lc++]=B[j++];
    la=lc;
    for(int k=0;k<lc;k++) A[k]=C[k];
}
struct Jii{
    int p[20],cnt;
    int w[20][20],len[20];
    void init(){
        cnt=0;
        for(int i=0;i<=16;i++){
            p[i]=0;
            len[i]=0;
        }
    }
    void nor(){
        for(int i=16;i>=0;i--){
            if(p[i]){
                for(int j=i-1;j>=0;j--){
                    if(p[j]&&((p[i]>>j)&1)){
                        p[i]^=p[j];
                        merge(w[i],w[j],len[i],len[j]);
                    }
                }
            }
        }
        for(int i=0;i<=16;i++){
            if(p[i]){
                for(int j=i+1;j<=16;j++){
                    if(p[j]&&((p[j]>>i)&1)){
                        p[j]^=p[i];
                        merge(w[j],w[i],len[j],len[i]);
                    }
                }
            }
        }
    }
    void add(int x,int wt){
        int cur[20],siz=1;
        cur[0]=wt;
        for(int i=16;i>=0;i--){
            if(x&(1<<i)){
                if(!p[i]){
                    p[i]=x;
                    cnt++;
                    len[i]=siz;
                    for(int j=0;j<siz;j++) w[i][j]=cur[j];
                    break;
                }
                x^=p[i];
                merge(cur,w[i],siz,len[i]);
            }
        }
    }
    int query(int x){
        for(int i=16;i>=0;i--){
            if((x^p[i])<x) x^=p[i];
        }
        return x;
    }
    void rq(int x,int res[],int &siz){
        siz=0;
        for(int i=16;i>=0;i--){
            if((p[i]^x)<x){
                x^=p[i];
                merge(res,w[i],siz,len[i]);
            }
        }
    }
}rt,rtp;
int a[N],b[20],val[1<<17],cost[1<<17],dp[1<<17];
bool vis[N];
int main(){
    int n;
    cin>>n;
    if(n==1){
        cout<<0<<endl;
        return 0;
    }
    for(int i=1;i<=n;i++) cin>>a[i];
    sort(a+1,a+1+n);
    dsu.n=n;
    dsu.init();
    rt.init();
    for(int i=1;i<=n;i++){
        int w=i;
        while(i<n&&a[i]==a[i+1]) dsu.merge(0,i),i++;
        if(w!=i){
            dsu.merge(0,i);
            rt.add(a[i],i);
            for(int j=w;j<=i;j++) vis[j]=1;
        }else{
            if(a[i]==0){
                dsu.merge(0,i);
                vis[i]=1;
            }
        }
    }
    rt.nor();
    long long ans=0;
    while(true){
        bool flag=true;
        while(flag){
            flag=false;
            for(int i=1;i<=n;i++){
                if(!vis[i]&&rt.query(a[i])==0){
                    dsu.merge(0,i);
                    rt.add(a[i],i);
                    vis[i]=true;
                    flag=true;
                }
            }
            if(flag) rt.nor();
        }
        int rem=0;
        for(int i=1;i<=n;i++) if(!vis[i]) rem++;
        if(rem==0) break;
        rtp.init();
        bool zero_subset=false;
        int zero_pts[20],siz=0;
        for(int i=1;i<=n;i++){
            if(!vis[i]){
                int x=a[i];
                int cur[20],len=1;
                cur[0]=i;
                bool inserted=false;
                for(int bit=16;bit>=0;bit--){
                    if(x&(1<<bit)){
                        if(!rtp.p[bit]){
                            rtp.p[bit]=x;
                            rtp.cnt++;
                            rtp.len[bit]=len;
                            for(int j=0;j<len;j++) rtp.w[bit][j]=cur[j];
                            inserted=true;
                            break;
                        }
                        x^=rtp.p[bit];
                        merge(cur,rtp.w[bit],len,rtp.len[bit]);
                    }
                }
                if(!inserted){
                    zero_subset=true;
                    siz=len;
                    for(int j=0;j<len;j++) zero_pts[j]=cur[j];
                    break;
                }
            }
        }
        if(zero_subset){
            for(int j=0;j<siz;j++){
                int u=zero_pts[j];
                dsu.merge(0,u);
                rt.add(a[u],u);
                vis[u]=true;
            }
            rt.nor();
            continue;
        }
        int k=0;
        for(int i=1;i<=n;i++) if(!vis[i]) b[k++]=rt.query(a[i]);
        int tot=(1<<k);
        for(int i=0;i<tot;i++) dp[i]=(1<<18);
        for(int i=1;i<tot;i++){
            int bit=__builtin_ctz(i);
            val[i]=val[i^(1<<bit)]^b[bit];
        }
        for(int i=0;i<tot;i++) cost[i]=val[i];
        for(int i=0;i<k;i++){
            for(int j=0;j<tot;j++){
                if(!(j&(1<<i))) cost[j]=min(cost[j],cost[j|(1<<i)]);
            }
        }
        dp[0]=0;
        for(int i=1;i<tot;i++){
            int bit=i&(-i),x=i^bit,j=x;
            while(true){
                int sub=j|bit;
                dp[i]=min(dp[i],cost[sub]+dp[i^sub]);
                if(j==0) break;
                j=(j-1)&x;
            }
        }
        ans=dp[tot-1];
        break;
    }
    cout<<ans<<endl;
    return 0;
}
