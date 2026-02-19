#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
template<class T>
void in(T &x){
	char c=getchar();T f=1; x=0;
	while(c<'0'||c>'9'){
		if(c=='-') f=-1;
		c=getchar();
	}
	while(c>='0'&&c<='9'){
		x=x*10+c-'0';
		c=getchar();
	}
	x*=f;
}
const int N=2510,Mod=1e9+7;
ll ksm(ll a,ll b){
    ll res=1;
    while(b){
        if(b&1) res=res*a%Mod;
        a=a*a%Mod; b>>=1;
    }
    return res;
}
const int M=80;
int ms;
struct Mat{
    ll a[M][M];
    void O(){
        for(int i=1;i<=ms;i++)
            memset(a[i],0,sizeof(ll)*(ms+1));
    }
    Mat(){O();}
    void E(){
        O();
        for(int i=1;i<=ms;i++)
            a[i][i]=1;
    }
};
Mat operator*(const Mat &a,const Mat &b){
    Mat res;
    for(int i=1;i<=ms;i++)
        for(int j=1;j<=ms;j++)
            for(int k=1;k<=ms;k++)
                (res.a[i][j]+=a.a[i][k]*b.a[k][j])%=Mod;
    return res;
}
Mat operator^(Mat a,ll b){
    Mat res; res.E();
    while(b){
        if(b&1) res=res*a;
        a=a*a; b>>=1;
    }
    return res;
}


int n,u,v; ll t;
vector<int> g[N];
int dis[N][N],cnt[N],mxdis[N];
void dfs(int st,int u,int fa){
    for(int v:g[u]){
        if(v==fa) continue;
        dis[st][v]=dis[st][u]+1;
        dfs(st,v,u);
    }
}
int getsize(int u,int fa){
    int sz=1;
    for(int v:g[u]){
        if(v==fa) continue;
        sz+=getsize(v,u);
    }
    return sz;
}
int getdep(int u,int fa){
    int dep=0;
    for(int v:g[u]){
        if(v==fa) continue;
        dep=max(dep,getdep(v,u)+1);
    }
    return dep;
}
int depcnt(int u,int fa,int tar){
    int ans=(tar==0);
    for(int v:g[u]){
        if(v==fa) continue;
        ans+=depcnt(v,u,tar-1);
    }
    return ans;
}
int szcnt[N],rszcnt[N],lis[N];

void init(){
    for(int i=1;i<=n;i++) g[i].clear();
    memset(szcnt,0,sizeof(szcnt));
    memset(rszcnt,0,sizeof(rszcnt));
    memset(lis,0,sizeof(lis));
    memset(cnt,0,sizeof(cnt));
    memset(mxdis,0,sizeof(mxdis));
    ms=0;
}

void work(){
    init();
    in(n); in(t);
    if(n==1){
        printf("1\n");
        return;
    }
    for(int i=1;i<n;i++){
        in(u); in(v);
        g[u].push_back(v);
        g[v].push_back(u);
    }
    int cen=1,acen=2;
    for(int i=1;i<=n;i++){
        dfs(i,i,0);
        for(int j=1;j<=n;j++) mxdis[i]=max(mxdis[i],dis[i][j]);
        for(int j=1;j<=n;j++) cnt[i]+=(dis[i][j]==mxdis[i]);
        if(mxdis[i]<mxdis[cen]){
            acen=cen;
            cen=i;
        }else if(mxdis[i]<mxdis[acen])
            acen=i;
        // printf("%d %d\n",mxdis[i],cnt[i]);
    }
    // printf("%d %d\n",cen,acen);
    if(mxdis[acen]==mxdis[cen]){
        // printf("TYPE A HERE LOL\n");
        int sz=getsize(cen,acen),asz=getsize(acen,cen);
        if(t&1) printf("%lld\n",(sz*cnt[cen]+asz*cnt[acen])*ksm(cnt[acen]*cnt[cen],t>>1)%Mod);
        else printf("%lld\n",n*ksm(cnt[acen]*cnt[cen],t>>1)%Mod);
    }else{
        //remember to set variable "ms" here!!!
        // printf("TYPE B HERE LOL\n");
        // int tot=0;
        for(int v:g[cen]){
            // if(getdep(v,cen)+1==mxdis[cen]){
                // tot++;
            int type=depcnt(v,cen,mxdis[cen]-1);
            szcnt[type]++;
            rszcnt[type]+=getsize(v,cen);
            // }
        }
        for(int i=1;i<=n;i++)
            if(szcnt[i]) lis[++ms]=i;//,printf("%d %d %d\n",i,szcnt[i],rszcnt[i]);
        Mat st,tr;
        for(int i=1;i<=ms;i++) st.a[1][i]=(n*(ll)szcnt[lis[i]]-rszcnt[lis[i]])*lis[i]%Mod;
        // for(int i=1;i<=ms;i++){
        //     for(int j=1;j<=ms;j++)
        //         printf("%lld ",st.a[i][j]);
        //     putchar('\n');
        // }
        // printf("%lld\n",st.a[1][1]);
        for(int i=1;i<=ms;i++){
            for(int j=1;j<=ms;j++){
                if(i==j) tr.a[i][j]=(szcnt[lis[i]]-1)*lis[i];
                else tr.a[i][j]=szcnt[lis[j]]*lis[j];
            }
        }
        // printf("%lld\n",tr.a[1][1]);
        tr=tr^(t-1);
        st=st*tr;
        ll ans=0;
        for(int i=1;i<=ms;i++) ans+=st.a[1][i];
        printf("%lld\n",ans%Mod);
    }
}
int main(){
    int tid,t;
    in(tid); in(t);
    while(t--) work();
}