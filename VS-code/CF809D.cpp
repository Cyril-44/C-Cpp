#include<bits/stdc++.h>
using namespace std;
int n,m,rt,idx,ans;
struct Splay{
    int a[600005],ch[600005][2],cnt[600005],fa[600005],sz[600005],lzy[600005];
    int newnode(int x,int c){
        idx++;
        ch[idx][0]=ch[idx][1]=0;
        a[idx]=x,sz[idx]=cnt[idx]=c;
        return idx;
    }
    void push_up(int x){
        sz[x]=sz[ch[x][0]]+sz[ch[x][1]]+cnt[x];
    }
	void push_down(int x){
		if(ch[x][0]){
			a[ch[x][0]]+=lzy[x];
			lzy[ch[x][0]]+=lzy[x];
		}
		if(ch[x][1]){
			a[ch[x][1]]+=lzy[x];
			lzy[ch[x][1]]+=lzy[x];
		}
		lzy[x]=0;
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
			push_down(fth);
			push_down(x);
            if(fa[fth]!=tar){
                if(get(x)==get(fth)) rotate(fth);
                else rotate(x);
            }
        }
        if(!tar) rt=x;
    }
    void insert(int x,int k){
		ans++;
        while(x){
			push_down(x);
            if(a[x]==k){
                cnt[x]++;
                sz[x]++;
                splay(x,0);
                return;
            }
            if(!ch[x][k>a[x]]){
                ch[x][k>a[x]]=newnode(k,1);
                fa[idx]=x;
                splay(idx,0);
                return;
            }
            x=ch[x][k>a[x]];
        }
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
		ans--;
		splay(x,0);
		if(cnt[x]>1){
			sz[x]--;
			cnt[x]--;
			return;
		}
        fa[ch[x][0]]=fa[ch[x][1]]=0;
        merge(ch[x][0],ch[x][1]);
		ch[x][0]=ch[x][1]=0;
    }
    int query_rnk(int x,int k){
        int sum=0;
        while(x){
			push_down(x);
            if(k>=a[x]) sum+=sz[ch[x][0]];
            if(k==a[x]){
                splay(x,0);
                return sum+1;
            }
            if(k>a[x]){
                sum+=cnt[x];
                if(!ch[x][1]){
                    splay(x,0);
                    return sum+1;
                }
                x=ch[x][1];
            }
            if(k<a[x]){
                if(!ch[x][0]){
                    splay(x,0);
                    return sum+1;
                }
                x=ch[x][0];
            }
        }
        return sum+1;
    }
    int query_num(int x,int k){
		if(k==0) return 1;
        for(push_down(x);;push_down(x)){
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
    int query_pre(int k){
		int x=rt,tmp=1,nw=rt;
		while(x){
			push_down(x);
			nw=x;
			if(a[x]<k){
				tmp=x;
				x=ch[x][1];
			}
			else x=ch[x][0];
		}
		splay(nw,0);
		return tmp;
    }
    int query_suf(int k){
		int x=rt,tmp=2,nw=rt;
		while(x){
			push_down(x);
			nw=x;
			if(a[x]>k){
				tmp=x;
				x=ch[x][0];
			}
			else x=ch[x][1];
		}
		splay(nw,0);
		return tmp;
    }
	void update(int l,int r){
		int x1=query_suf(l-1),x2=query_pre(r+1);
		if(a[x1]>a[x2]) return;
		if(x1==x2){
			a[x1]++;
			return;
		}
		splay(x1,0);
		splay(x2,x1);
		push_down(x1),push_down(x2);
		a[x1]++;
		a[x2]++;
		if(ch[x2][0]){
			a[ch[x2][0]]++;
			lzy[ch[x2][0]]++;
			splay(ch[x2][0],0);
		}
	}
}BST;
int main(){
    cin>>n;
	int x1=BST.newnode(-2e9,0),x2=BST.newnode(2e9,1);
	rt=BST.newnode(0,1);
	BST.ch[rt][0]=x1,BST.ch[rt][1]=x2;
	BST.fa[x1]=BST.fa[x2]=rt;
	BST.push_up(rt);
	for(int i=1;i<=n;i++){
		int l,r;
		scanf("%d%d",&l,&r);
		int x=BST.query_suf(r-1);
		if(x&&x!=1&&x!=2) BST.erase(x);
		BST.update(l,r-1);
		BST.insert(rt,l);
	}
	cout<<ans;
    return 0;
}