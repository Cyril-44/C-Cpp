#include<bits/stdc++.h>
using namespace std;
int n,m,rt,idx,ans,lst;
struct Splay{
    int a[1100005],ch[1100005][2],cnt[1100005],fa[1100005],sz[1100005];
    int newnode(int x,int c){
        idx++;
        ch[idx][0]=ch[idx][1]=0;
        a[idx]=x,sz[idx]=cnt[idx]=c;
        return idx;
    }
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
    void insert(int x,int k){
        while(x){
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
    void erase(int x,int k){
        while(x){
            if(a[x]==k){
                splay(x,0);
                if(cnt[x]>1){
                    cnt[x]--;
                    sz[x]--;
                    return;
                }
                fa[ch[x][0]]=fa[ch[x][1]]=0;
                merge(ch[x][0],ch[x][1]);
                return;
            }
            x=ch[x][k>a[x]];
        }
    }
    int query_rnk(int x,int k){
        int sum=0;
        while(x){
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
        while(x){
            if(sz[ch[x][0]]>=k) x=ch[x][0];
            else if(sz[ch[x][0]]+cnt[x]<k){
                k-=sz[ch[x][0]]+cnt[x];
                x=ch[x][1];
            }
            else{
                splay(x,0);
                return a[x];
            }
        }
    }
    int query_pre(int k){
        int rk=query_rnk(rt,k);
        return query_num(rt,rk-1);
    }
    int query_suf(int k){
        int rk=query_rnk(rt,k+1);
        return query_num(rt,rk);
    }
}BST;
int main(){
    cin>>n>>m;
    rt=BST.newnode(INT_MAX,1);
    BST.ch[rt][0]=BST.newnode(INT_MIN,1);
    BST.fa[BST.ch[rt][0]]=rt;
    for(int i=1;i<=n;i++){
        int x;
        scanf("%d",&x);
        BST.insert(rt,x);
    }
    while(m--){
        int op,x;
        scanf("%d%d",&op,&x);
        x^=lst;
        if(op==1){
            BST.insert(rt,x);
        }
        else if(op==2){
            BST.erase(rt,x);
        }
        else if(op==3){
            lst=BST.query_rnk(rt,x)-1;
            ans^=lst;
        }
        else if(op==4){
            lst=BST.query_num(rt,x+1);
            ans^=lst;
        }
        else if(op==5){
            lst=BST.query_pre(x);
            ans^=lst;
        }
        else if(op==6){
            lst=BST.query_suf(x);
            ans^=lst;
        }
    }
    cout<<ans;
    return 0;
}