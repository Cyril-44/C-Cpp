#include<bits/stdc++.h>
#define ll long long
using namespace std;
int n,m,q,idx;
void read(int &x){
	x=0;
	char ch=getchar();
	int f=1;
	while(ch<'0'||ch>'9'){
		if(ch=='-') f=-1;
		ch=getchar();
	}
	while(ch>='0'&&ch<='9'){
		x=(x<<3)+(x<<1)+ch-48;
		ch=getchar();
	}
	x*=f;
}
int ch[2110005][2],fa[2110005],sz[2110005],cnt[2110005];
ll id[2110005];
int newnode(int c,ll k){
	idx++;
	ch[idx][0]=ch[idx][1]=0;
	sz[idx]=cnt[idx]=c;
	id[idx]=k;
	return idx;
}
struct Splay{
	int rt,mi;
	void push_up(int x){
		sz[x]=sz[ch[x][0]]+sz[ch[x][1]]+cnt[x];
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
	void merge(int x,int y){
		while(x){
			if(!ch[x][1]) break;
			x=ch[x][1];
		}
		splay(x,0);
		ch[x][1]=y;
		fa[y]=x;
		push_up(x);
	}
	void erase(int x){
		splay(x,0);
		fa[ch[x][0]]=fa[ch[x][1]]=0;
		merge(ch[x][0],ch[x][1]);
	}
	int query_rnk(int x){
		splay(x,0);
		return sz[ch[x][0]]+cnt[x];
	}
	int query_num(int x,int k){
		if(k==0) return mi;
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
		assert(x);
	}
	void insert(int x,int k,ll id){
		int x1=query_num(rt,x);
		int x2=query_num(rt,x+1);
		splay(x1,0),splay(x2,x1);
		int chd=newnode(k,id);
		ch[x2][0]=chd;
		fa[chd]=x2;
		splay(chd,0);
	}
	void modify(int x,int k,ll idd){
		splay(x,0);
		cnt[x]=k;
		id[x]=idd;
		push_up(x);
	}
}BST[300005];
int main(){
	read(n),read(m),read(q);
	for(int i=1;i<=n;i++){
		BST[i].rt=newnode(m-1,1ll*(i-1)*m+1);
		ch[BST[i].rt][0]=BST[i].mi=newnode(0,0);
		ch[BST[i].rt][1]=newnode(1,0);
		fa[ch[BST[i].rt][0]]=fa[ch[BST[i].rt][1]]=BST[i].rt;
		BST[i].push_up(BST[i].rt);
	}
	BST[0].rt=BST[0].mi=newnode(0,0);
	int nw=BST[0].rt;
	for(int i=1;i<=n;i++){
		ch[nw][1]=newnode(1,1ll*i*m);
		fa[ch[nw][1]]=nw;
		nw=ch[nw][1];
	}
	ch[nw][1]=newnode(1,0);
	fa[ch[nw][1]]=nw;
	nw=ch[nw][1];
	BST[0].splay(nw,0);
	while(q--){
		int x,y,w,z;
		read(x),read(y);
		if(y<m){
			w=BST[x].query_num(BST[x].rt,y);
			ll ans=id[w]+y-sz[ch[w][0]]-1;
			printf("%lld\n",ans);
			int lft=y-sz[ch[w][0]]-1,rgt=cnt[w]-(y-sz[ch[w][0]]);
			if(lft){
				BST[x].modify(w,lft,id[w]);
				if(rgt) BST[x].insert(sz[ch[w][0]]+cnt[w],rgt,ans+1);
			}
			else{
				if(rgt) BST[x].modify(w,rgt,id[w]+1);
				else BST[x].erase(w);
			}
			z=BST[0].query_num(BST[0].rt,x);
			BST[x].insert(m-2,1,id[z]);
			BST[0].erase(z);
			BST[0].insert(n-1,1,ans);
		}
		else{
			z=BST[0].query_num(BST[0].rt,x);
			ll ans=id[z]+x-sz[ch[z][0]]-1;
			printf("%lld\n",ans);
			BST[0].erase(z);
			BST[0].insert(n-1,1,ans);
		}
	}
	return 0;
}