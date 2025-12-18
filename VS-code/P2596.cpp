#include<bits/stdc++.h>
using namespace std;
int n,m,rt,z[500005],idx,ans;
priority_queue<int,vector<int>,greater<int> > q;
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
void out(int x){
    if(!x) return;
    out(x/10);
    putchar(x%10+48);
}
struct Splay{
    int ch[500005][2],fa[500005],sz[500005],cnt[500005];
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
    void build(){
        rt=n+1;
        sz[n+1]=cnt[n+1]=0;
        z[0]=n+1;
        for(int i=1;i<=n;i++){
            ch[z[i-1]][1]=z[i];
            fa[z[i]]=z[i-1];
            sz[i]=cnt[i]=1;
        }
        z[n+1]=n+2;
        ch[z[n]][1]=z[n+1];
        fa[z[n+1]]=z[n];
        sz[n+2]=cnt[n+2]=1;
        splay(n+2,0);
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
        merge(ch[x][0],ch[x][1]);
    }
    int query_rnk(int x){
        splay(x,0);
        return sz[ch[x][0]]+cnt[x];
    }
    int query_num(int x,int k){
        if(k==0) return n+1;
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
    void insert(int x,int k){
        int x1=query_num(rt,x),x2=query_num(rt,x+1);
        splay(x1,0),splay(x2,x1);
        ch[x2][0]=k;
        fa[k]=x2;
        sz[k]=cnt[k]=1;
        ch[k][0]=ch[k][1]=0;
        splay(k,0);
    }
    void movetop(int x){
        erase(x);
        insert(0,x);
    }
    void movebottom(int x){
        erase(x);
        insert(n-1,x);
    }
    void move(int x,int t){
        int xx=query_rnk(x);
        erase(x);
        insert(xx-1+t,x);
    }
}BST;
int main(){
    read(n),read(m);
    for(int i=1;i<=n;i++){
        read(z[i]);
    }
    BST.build();
    while(m--){
        char op[10];
        scanf("%s",op);
        if(op[0]=='T'){
            int x;
            read(x);
            BST.movetop(x);
        }
        else if(op[0]=='B'){
            int x;
            read(x);
            BST.movebottom(x);
        }
        else if(op[0]=='I'){
            int x,t;
            read(x),read(t);
            BST.move(x,t);
        }
        else if(op[0]=='A'){
            int x;
            read(x);
            printf("%d\n",BST.query_rnk(x)-1);
        }
        else{
            int x;
            read(x);
            printf("%d\n",BST.query_num(rt,x));
        }
    }
    return 0;
}