#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m,rt,idx,A[500005],B[500005];
double ans;
struct Splay{
    int ch[500005][2],fa[500005];
	ll sz[500005],cnt[500005];
	double a[500005],asum[500005],sum[500005];
	int newnode(double k,int c){
		idx++;
		ch[idx][0]=ch[idx][1]=0;
		a[idx]=k;
		sz[idx]=cnt[idx]=c;
		sum[idx]=asum[idx]=k*c;
		return idx;
	}
    void push_up(int x){
        sz[x]=sz[ch[x][0]]+sz[ch[x][1]]+cnt[x];
		sum[x]=sum[ch[x][0]]+sum[ch[x][1]]+asum[x];
    }
    int get(int x){
        return ch[fa[x]][1]==x;
    }
    void rotate(int x){
        int y=fa[x];
        int z=fa[y];
        int tp=get(x),tp2=get(y);
        ch[y][tp]=ch[x][tp^1];
        fa[ch[x][tp^1]]=y;
        ch[x][tp^1]=y;
        fa[y]=x;
        fa[x]=z;
        if(z) ch[z][tp2]=x;
        push_up(y);
        push_up(x);
    }
    void splay(int x,int tar){
        for(int fth;(fth=fa[x])!=tar;rotate(x)){
            if(fa[fth]!=tar){
                if(get(x)==get(fth)) rotate(fth);
                else rotate(x);
            }
        }
        if(!tar) rt=x;
    }
    ll query_rnk(int x,double k){
		ll sum=0;
		while(x){
			if(a[x]==k) return sum+sz[ch[x][0]]+1;
			if(a[x]>k) x=ch[x][0];
			else{
				sum+=sz[ch[x][0]]+cnt[x];
				x=ch[x][1];
			}
		}
		return sum+1;
    }
    int query_num(int x,ll k){
        while(x){
            if(sz[ch[x][0]]>=k) x=ch[x][0];
            else if(sz[ch[x][0]]+cnt[x]<k){
                k-=sz[ch[x][0]]+cnt[x];
                x=ch[x][1];
            }
            else{
                splay(x,0);
                return x;
            }
        }
    }
    void insert(double x,int k){
        ll rk=query_rnk(rt,x);
		if(rk==1){
			fa[rt]=newnode(x,k);
			ch[fa[rt]][1]=rt;
			rt=fa[rt];
			push_up(rt);
			return;
		}
		if(rk==sz[rt]+1){
			fa[rt]=newnode(x,k);
			ch[fa[rt]][0]=rt;
			rt=fa[rt];
			push_up(rt);
			return;
		}
		int x1=query_num(rt,rk-1),x2=query_num(rt,rk);
        splay(x1,0),splay(x2,x1);
        ch[x2][0]=newnode(x,k);
        fa[ch[x2][0]]=x2;
        splay(ch[x2][0],0);
    }
	double query(int x){
		ll lcnt=0,nn=sz[rt];
		while(x){
			lcnt+=sz[ch[x][0]];
			if(lcnt*2<=nn&&(lcnt+cnt[x])*2>nn){
				splay(x,0);
				return sum[ch[x][1]]-sum[ch[x][0]]+a[x]*(nn/2-(nn-sz[ch[x][0]]-cnt[x]))-a[x]*(nn/2-sz[ch[x][0]]);
			}
			if(lcnt*2>nn){
				lcnt-=sz[ch[x][0]];
				x=ch[x][0];
			}
			else{
				lcnt+=cnt[x];
				x=ch[x][1];
			}
		}
	}
}BST;
int main(){
    cin>>n;
	for(int i=1;i<=n;i++){
		scanf("%d",&A[i]);
	}
	for(int i=1;i<=n;i++){
		scanf("%d",&B[i]);
		if(A[i]==0){
			ans+=abs(B[i]);
		}
		if(A[i]<0){
			A[i]=-A[i];
			B[i]=-B[i];
		}
		if(A[i]) BST.insert(double(-B[i])/A[i],A[i]);
		printf("%.8lf\n",BST.query(rt)+ans);
	}
    return 0;
}