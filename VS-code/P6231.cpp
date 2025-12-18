#include<bits/stdc++.h>
using namespace std;
typedef pair<int,int> p;
int n,m,idx;
struct Splay{
    int a[300005],ch[300005][2],cnt[300005],fa[300005],sz[300005],sum[300005];
	bool rev[300005];
	int newnode(int k,int c){
		idx++;
		ch[idx][0]=ch[idx][1]=0;
		cnt[idx]=sz[idx]=c,sum[idx]=a[idx]=k;
		return idx;
	}
	void operate(int x){
		swap(ch[x][0],ch[x][1]);
	}
	void push_up(int x){
		sz[x]=sz[ch[x][0]]+sz[ch[x][1]]+cnt[x];
		sum[x]=sum[ch[x][0]]+sum[ch[x][1]]+a[x];
	}
	void push_down(int x){
		if(!rev[x]) return;
		if(ch[x][0]){
			operate(ch[x][0]);
			rev[ch[x][0]]^=1;
		}
		if(ch[x][1]){
			operate(ch[x][1]);
			rev[ch[x][1]]^=1;
		}
		rev[x]=0;
	}
	int getrt(int x){
		if(fa[x]) return getrt(fa[x]);
		return x;
	}
    int get(int x){
        return ch[fa[x]][1]==x;
    }
    void rotate(int x){
        int y=fa[x];
        int z=fa[y];
        int tp=get(x),tp2=get(y);
		push_down(y),push_down(x);
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
		if(!x) return;
        for(int fth;(fth=fa[x])!=tar;rotate(x)){
            if(fa[fth]!=tar){
                if(get(x)==get(fth)) rotate(fth);
                else rotate(x);
            }
        }
    }
	bool insert(int x,int y,int z){
		splay(y,0);
		push_down(y);
		splay(x,0);
		push_down(x);
		if((ch[x][0]&&ch[x][1])||(ch[y][0]&&ch[y][1])||(getrt(y)==x)) return 0;
		if(ch[x][1]){
			operate(x);
			rev[x]=1;
		}
		if(ch[y][0]){
			operate(y);
			rev[y]=1;
		}
		int xx=newnode(z,1);
		ch[xx][0]=x,ch[xx][1]=y;
		fa[x]=fa[y]=xx;
		push_up(xx);
		return 1;
	}
	bool erase(int x,int y){
		splay(y,0);
		push_down(y);
		splay(x,0);
		push_down(x);
		if(getrt(y)!=x) return 0;
		splay(y,x);
		if(sz[ch[y][0]]==1){
			ch[y][0]=0;
			fa[y]=0;
			push_up(y);
			ch[x][1]=0;
			push_up(x);
		}
		else if(sz[ch[y][1]]==1){
			ch[y][1]=0;
			fa[y]=0;
			push_up(y);
			ch[x][0]=0;
			push_up(x);
		}
		else return 0;
		return 1;
	}
	bool modify(int x,int y,int z){
		splay(y,0);
		push_down(y);
		splay(x,0);
		push_down(x);
		if(getrt(y)!=x) return 0;
		splay(y,x);
		if(sz[ch[y][0]]==1){
			a[ch[y][0]]=sum[ch[y][0]]=z;
			push_up(y);
			push_up(x);
		}
		else if(sz[ch[y][1]]==1){
			a[ch[y][1]]=sum[ch[y][1]]=z;
			push_up(y);
			push_up(x);
		}
		else return 0;
		return 1;
	}
	bool check(int x,int y){
		splay(y,0);
		push_down(y);
		splay(x,0);
		push_down(x);
		return getrt(y)==x;
	}
	p query(int x,int y){
		splay(y,x);
		if(ch[x][0]==y){
			int ti=sum[y];
			push_down(y);
			while(ch[y][0]){
				y=ch[y][0];
				push_down(y);
			}
			return make_pair(y,ti);
		}
		else{
			int ti=sum[y];
			push_down(y);
			while(ch[y][1]){
				y=ch[y][1];
				push_down(y);
			}
			return make_pair(y,ti);
		}
	}
}BST;
int main(){
	cin>>n>>m;
	for(int i=1;i<=n;i++){
		BST.newnode(0,1);
	}
	while(m--){
		char op[10];
		scanf("%s",op);
		if(op[0]=='a'){
			int x,y,z;
			scanf("%d%d%d",&x,&y,&z);
			bool ok=BST.insert(x,y,z);
			printf("%s\n",ok? "OK":"ERROR");
		}
		else if(op[2]=='l'){
			int x,y;
			scanf("%d%d",&x,&y);
			bool ok=BST.erase(x,y);
			printf("%s\n",ok? "OK":"ERROR");
		}
		else if(op[0]=='c'){
			int x,y,z;
			scanf("%d%d%d",&x,&y,&z);
			bool ok=BST.modify(x,y,z);
			printf("%s\n",ok? "OK":"ERROR");
		}
		else if(op[0]=='r'){
			int x,y;
			scanf("%d%d",&x,&y);
			bool ok=BST.check(x,y);
			printf("%s\n",ok? "YES":"NO");
		}
		else{
			int x,y;
			scanf("%d%d",&x,&y);
			bool ok=BST.check(x,y);
			if(!ok){
				printf("ERROR\n");
				continue;
			}
			p ans=BST.query(x,y);
			printf("%d %d\n",ans.first,ans.second);
		}
	}
    return 0;
}