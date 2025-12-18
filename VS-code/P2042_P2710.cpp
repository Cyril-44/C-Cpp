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
    int a[500005],ch[500005][2],fa[500005],sz[500005],cnt[500005];
    int pre[500005],suf[500005],sum[500005],mxs[500005];
    int tag[500005],rev[500005];
    int newnode(int x,int c){
        idx=q.top();
        q.pop();
        ch[idx][0]=ch[idx][1]=0;
        a[idx]=mxs[idx]=sum[idx]=x,sz[idx]=cnt[idx]=c;
        tag[idx]=3001,rev[idx]=0;
        pre[idx]=suf[idx]=max(0,x);
        return idx;
    }
    void clear(int x){
        q.push(x);
        if(ch[x][0]){
            clear(ch[x][0]);
        }
        if(ch[x][1]){
            clear(ch[x][1]);
        }
    }
    void modify(int x,int op){
        a[x]=op;
        sum[x]=op*sz[x];
        pre[x]=suf[x]=max(0,sum[x]);
        mxs[x]=max(op,sum[x]);
    }
    void operate(int x,int op1,int op2){
        if(op1!=3001){
            tag[x]=op1;
            modify(x,op1);
            rev[x]=0;
            return;
        }
        if(op2&&tag[x]==3001){
            rev[x]^=1;
            swap(pre[x],suf[x]);
            swap(ch[x][0],ch[x][1]);
        }
    }
    void push_up(int x){
        sum[x]=sum[ch[x][0]]+a[x]+sum[ch[x][1]];
        sz[x]=sz[ch[x][0]]+sz[ch[x][1]]+cnt[x];
        pre[x]=max(pre[ch[x][0]],sum[ch[x][0]]+a[x]+pre[ch[x][1]]);
        suf[x]=max(suf[ch[x][1]],sum[ch[x][1]]+a[x]+suf[ch[x][0]]);
        if(ch[x][0]){
            if(ch[x][1]){
                mxs[x]=max(max(mxs[ch[x][0]],mxs[ch[x][1]]),suf[ch[x][0]]+a[x]+pre[ch[x][1]]);
            }
            else{
                mxs[x]=max(mxs[ch[x][0]],suf[ch[x][0]]+a[x]);
            }
        }
        else if(ch[x][1]){
            mxs[x]=max(mxs[ch[x][1]],pre[ch[x][1]]+a[x]);
        }
        else{
            mxs[x]=a[x];
        }
    }
    void push_down(int x){
        if(ch[x][0]) operate(ch[x][0],tag[x],rev[x]);
        if(ch[x][1]) operate(ch[x][1],tag[x],rev[x]);
        tag[x]=3001,rev[x]=0;
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
        rt=newnode(-3001,0);
        for(int i=1;i<=n;i++){
            int x=newnode(z[i],1);
            ch[x][0]=rt;
            fa[rt]=x;
            rt=x;
        }
        int x=newnode(3001,1);
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
    int work(int x,int y){
        int x1=query_num(rt,x),x2=query_num(rt,y);
        splay(x1,0);
        splay(x2,x1);
        push_down(x1),push_down(x2);
        return x2;
    }
    void erase(int l,int r){
        int x2=work(l-1,r+1);
        clear(ch[x2][0]);
        ch[x2][0]=0;
        splay(x2,0);
    }
    void insert(int x,int k){
        int nw=work(x+1,x);
        for(int i=1;i<=k;i++){
            read(x);
            int xx=newnode(x,1);
            ch[nw][1]=xx;
            fa[xx]=nw;
            nw=xx;
        }
        splay(nw,0);
    }
    void update(int l,int r,int x){
        int x2=work(l-1,r+1);
        operate(ch[x2][0],x,0);
    }
    void reverse(int l,int r){
        int x2=work(l-1,r+1);
        operate(ch[x2][0],3001,1);
    }
    int queryx(int x){
        int xx=query_num(rt,x);
        return a[xx];
    }
    int querysum(int l,int r){
        int x2=work(l-1,r+1);
        return sum[ch[x2][0]];
    }
    int querymxs(int l,int r){
        int x2=work(l-1,r+1);
        return mxs[ch[x2][0]];
    }
}BST;
int main(){
    for(int i=1;i<=500005;i++){
        if(i<500005) BST.tag[i]=3001;
        q.push(i);
    }
    read(n),read(m);
    for(int i=1;i<=n;i++){
        read(z[i]);
    }
    BST.build();
    while(m--){
        char op[10];
        scanf("%s",op);
        if(op[0]=='I'){
            int w,k;
            read(w),read(k);
            BST.insert(w,k);
            n+=k;
        }
        else if(op[0]=='D'){
            int w,k;
            read(w),read(k);
            BST.erase(w,w+k-1);
            n-=k;
        }
        else if(op[2]=='K'){
            int w,k,x;
            read(w),read(k),read(x);
            BST.update(w,w+k-1,x);
        }
        else if(op[0]=='R'){
            int w,k;
            read(w),read(k);
            BST.reverse(w,w+k-1);
        }
        else if(op[0]=='G'&&op[3]=='-'){
            int w,k;
            read(w),read(k);
            if(k==0){
            	puts("0");
            	continue;
			}
            printf("%d\n",BST.querysum(w,w+k-1));
        }
        else if(op[0]=='G'){
            int x;
            read(x);
            printf("%d\n",BST.queryx(x));
        }
        else{
            int w,k;
            read(w),read(k);
            printf("%d\n",BST.querymxs(w,w+k-1));
        }
    }
    return 0;
}