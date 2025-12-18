#include<bits/stdc++.h>
using namespace std;
int n,m,rt,z[100005],idx;
char s[100005];
struct Splay{
    int a[100005],ch[100005][2],fa[100005],sz[100005],cnt[100005];
    int presmx[100005],presmn[100005],sufsmx[100005],sufsmn[100005],sum[100005];
    int tag[100005],rev[100005],inv[100005];
    int newnode(int x,int c){
        idx++;
        ch[idx][0]=ch[idx][1]=0;
        a[idx]=x,sz[idx]=cnt[idx]=c;
        return idx;
    }
    void modify(int x,int op){
        a[x]=op;
        sum[x]=op*sz[x];
        presmx[x]=sufsmx[x]=max(0,sum[x]);
        presmn[x]=sufsmn[x]=min(0,sum[x]);
    }
    void oppo(int x){
        presmx[x]=-presmx[x];
        presmn[x]=-presmn[x];
        sufsmx[x]=-sufsmx[x];
        sufsmn[x]=-sufsmn[x];
        sum[x]=-sum[x];
        a[x]=-a[x];
    }
    void operate(int x,int op1,int op2,int op3){
        if(op1){
            tag[x]=op1;
            modify(x,op1);
            rev[x]=inv[x]=0;
            return;
        }
        if(op2&&!tag[x]){
            rev[x]^=1;
            swap(presmx[x],sufsmx[x]);
            swap(presmn[x],sufsmn[x]);
            swap(ch[x][0],ch[x][1]);
        }
        if(op3){
            if(tag[x]){
                tag[x]=-tag[x];
                modify(x,tag[x]);
            }
            else{
                inv[x]^=1;
                swap(presmx[x],presmn[x]);
                swap(sufsmx[x],sufsmn[x]);
                oppo(x);
            }
        }
    }
    void push_up(int x){
        sum[x]=sum[ch[x][0]]+a[x]+sum[ch[x][1]];
        sz[x]=sz[ch[x][0]]+sz[ch[x][1]]+cnt[x];
        presmx[x]=max(presmx[ch[x][0]],sum[ch[x][0]]+a[x]+presmx[ch[x][1]]);
        presmn[x]=min(presmn[ch[x][0]],sum[ch[x][0]]+a[x]+presmn[ch[x][1]]);
        sufsmx[x]=max(sufsmx[ch[x][1]],sum[ch[x][1]]+a[x]+sufsmx[ch[x][0]]);
        sufsmn[x]=min(sufsmn[ch[x][1]],sum[ch[x][1]]+a[x]+sufsmn[ch[x][0]]);
    }
    void push_down(int x){
        if(ch[x][0]) operate(ch[x][0],tag[x],rev[x],inv[x]);
        if(ch[x][1]) operate(ch[x][1],tag[x],rev[x],inv[x]);
        tag[x]=rev[x]=inv[x]=0;
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
        for(int fth;(fth=fa[x])!=tar;rotate(x)){
            if(fa[fth]!=tar){
                if(get(x)==get(fth)) rotate(fth);
                else rotate(x);
            }
        }
        if(!tar) rt=x;
    }
    void build(){
        rt=newnode(0,0);
        for(int i=2;i<=n+1;i++){
            int x=newnode(z[i],1);
            ch[x][0]=rt;
            fa[rt]=x;
            rt=x;
        }
        int x=newnode(0,1);
        ch[x][0]=rt;
        fa[rt]=x;
        rt=x;
        splay(1,0);
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
    void update(int l,int r,int op1,int op2,int op3){
        int x1=query_num(rt,l-1);
        int x2=query_num(rt,r+1);
        splay(x1,0);
        splay(x2,x1);
        push_down(x1),push_down(x2);
        operate(ch[x2][0],op1,op2,op3);
    }
    int query(int l,int r){
        int x1=query_num(rt,l-1);
        int x2=query_num(rt,r+1);
        splay(x1,0);
        splay(x2,x1);
        push_down(x1),push_down(x2);
        int x=ch[x2][0];
        return (-presmn[x]+1)/2+(sufsmx[x]+1)/2;
    }
}BST;
int main(){
    scanf("%d%d%s",&n,&m,s+2);
    for(int i=2;i<=n+1;i++){
        z[i]=(s[i]=='('? 1:-1);
    }
    BST.build();
    while(m--){
        int l,r;
        scanf("%s%d%d",s,&l,&r);
        if(s[0]=='R'){
            char rpl[2];
            scanf("%s",rpl);
            if(rpl[0]=='('){
                BST.update(l,r,1,0,0);
            }
            else{
                BST.update(l,r,-1,0,0);
            }
        }
        else if(s[0]=='S'){
            BST.update(l,r,0,1,0);
        }
        else if(s[0]=='I'){
            BST.update(l,r,0,0,1);
        }
        else{
            printf("%d\n",BST.query(l,r));
        }
    }
    return 0;
}