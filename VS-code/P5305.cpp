#include<bits/stdc++.h>
#define ll long long
using namespace std;
const int mod=998244353;
int n,m,k,dfn[50005],dep[50005],fa[50005],sz[50005],hvson[50005],top[50005],rk[50005],idx;
ll ans[50005],a[50005];
vector<int> g[50005];
struct node{
	int x,id;
};
vector<node> vr[50005];
ll power(ll x,ll kk){
    if(kk==0) return 1;
    if(kk==1) return x;
    if(kk%2){
        ll y=power(x,kk/2);
        return y*y%mod*x%mod;
    }
    else{
        ll y=power(x,kk/2);
        return y*y%mod;
    }
}
void dfs(int x){
	sz[x]=1;
    dep[x]=dep[fa[x]]+1;
	for(int y:g[x]){
		dfs(y);
		sz[x]+=sz[y];
	}
}
void dfs2(int x,int tp){
	dfn[x]=++idx;
    rk[idx]=x;
	top[x]=tp;
	int mx=0,mxid=0;
	for(int y:g[x]){
		if(mx<sz[y]){
			mx=sz[y];
			mxid=y;
		}
	}
	hvson[x]=mxid;
	if(mx) dfs2(mxid,tp);
	for(int y:g[x]){
		if(y!=mxid){
			dfs2(y,y);
		}
	}
}
struct segtree{
	ll tr[200005],lzy[200005],key[200005];
	void push_up(int x){
		tr[x]=(tr[x<<1]+tr[x<<1|1])%mod;
	}
	void push_down(int x,int l,int r){
		int mid=l+r>>1;
		(tr[x<<1]+=lzy[x]*key[x<<1])%=mod;
		(tr[x<<1|1]+=lzy[x]*key[x<<1|1])%=mod;
		(lzy[x<<1]+=lzy[x])%=mod;
		(lzy[x<<1|1]+=lzy[x])%=mod;
		lzy[x]=0;
	}
    void build(int x,int l,int r){
        if(l==r){
            key[x]=a[rk[l]];
            return;
        }
        int mid=l+r>>1;
        build(x<<1,l,mid);
        build(x<<1|1,mid+1,r);
        key[x]=(key[x<<1]+key[x<<1|1])%mod;
    }
	void update(int x,int l,int r,int L,int R,ll k){
		if(l>=L&&r<=R){
			(tr[x]+=k*key[x])%=mod;
			(lzy[x]+=k)%=mod;
			return;
		}
		push_down(x,l,r);
		int mid=l+r>>1;
		if(mid>=L) update(x<<1,l,mid,L,R,k);
		if(mid<R) update(x<<1|1,mid+1,r,L,R,k);
		push_up(x);
	}
	ll query(int x,int l,int r,int L,int R){
		if(l>=L&&r<=R){
			return tr[x];
		}
		push_down(x,l,r);
		int mid=l+r>>1;
        ll cnt=0;
		if(mid>=L) (cnt+=query(x<<1,l,mid,L,R))%=mod;
		if(mid<R) (cnt+=query(x<<1|1,mid+1,r,L,R))%=mod;
		return cnt;
	}
}seg;
void update(int x){
	while(x){
		seg.update(1,1,n,dfn[top[x]],dfn[x],1);
		x=fa[top[x]];
	}
}
ll query(int x){
	ll cnt=0;
	while(x){
		(cnt+=seg.query(1,1,n,dfn[top[x]],dfn[x]))%=mod;
		x=fa[top[x]];
	}
	return cnt;
}
int main(){
	cin>>n>>m>>k;
	for(int i=2;i<=n;i++){
		int x;
		scanf("%d",&x);
		g[x].push_back(i);
		fa[i]=x;
	}
	dfs(1);
	dfs2(1,1);
    for(int i=1;i<=n;i++){
        a[i]=(power(dep[i],k)-power(dep[i]-1,k)+mod)%mod;
    }
    seg.build(1,1,n);
	for(int i=1;i<=m;i++){
		int r,x;
		scanf("%d%d",&r,&x);
		vr[r].push_back((node){x,i});
	}
	for(int i=1;i<=n;i++){
		update(i);
		for(node x:vr[i]){
			ans[x.id]=(ans[x.id]+query(x.x))%mod;
		}
	}
	for(int i=1;i<=m;i++){
		printf("%lld\n",ans[i]);
	}
	return 0;
}