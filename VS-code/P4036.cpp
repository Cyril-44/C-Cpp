#include<bits/stdc++.h>
#define ull unsigned long long
using namespace std;
ull mul[100005];
int n,m,rt,idx;
char s[100005];
struct Splay{
    int a[100005],ch[100005][2],fa[100005],sz[100005],cnt[100005];
    ull hsh[100005];
    int newnode(int x,int c){
        idx++;
        ch[idx][0]=ch[idx][1]=0;
        a[idx]=hsh[idx]=x,sz[idx]=cnt[idx]=c;
        return idx;
    }
    void push_up(int x){
        sz[x]=sz[ch[x][0]]+sz[ch[x][1]]+cnt[x];
        hsh[x]=hsh[ch[x][0]]*mul[sz[ch[x][1]]+cnt[x]]+a[x]*mul[sz[ch[x][1]]]+hsh[ch[x][1]];
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
        rt=newnode(0,0);
        for(int i=2;i<=n+1;i++){
            int x=newnode(s[i]-'a'+1,1);
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
    void modify(int x,int c){
        int xx=query_num(rt,x);
        splay(xx,0);
        a[xx]=c;
        push_up(xx);
    }
    void update(int x,int c){
        int xx=query_num(rt,x);
        splay(xx,0);
        int x2=newnode(c,1);
        ch[x2][1]=ch[xx][1];
        fa[ch[x2][1]]=x2;
        ch[xx][1]=x2;
        fa[x2]=xx;
        push_up(x2);
        push_up(xx);
        splay(x2,0);
    }
    ull query(int l,int r){
        int x1=query_num(rt,l-1);
        int x2=query_num(rt,r+1);
        splay(x1,0);
        splay(x2,x1);
        return hsh[ch[x2][0]];
    }
}BST;
bool check(int x,int y,int mid){
    ull hsh1=BST.query(x,x+mid-1);
    ull hsh2=BST.query(y,y+mid-1);
    return hsh1==hsh2;
}
int main(){
    mul[0]=1;
    for(int i=1;i<=100004;i++){
        mul[i]=mul[i-1]*31;
    }
    scanf("%s%d",s+2,&m);
    n=strlen(s+2);
    BST.build();
    while(m--){
        char op[2];
        scanf("%s",op);
        if(op[0]=='Q'){
            int x,y;
            scanf("%d%d",&x,&y);
            int l=0,r=min(n-y+1,n-x+1),mid;
            while(l<=r){
                mid=l+r>>1;
                if(check(x,y,mid)) l=mid+1;
                else r=mid-1;
            }
            printf("%d\n",l-1);
        }
        else if(op[0]=='R'){
            int x;
            scanf("%d%s",&x,op);
            BST.modify(x,op[0]-'a'+1);
        }
        else{
            int x;
            scanf("%d%s",&x,op);
            BST.update(x,op[0]-'a'+1);
            n++;
        }
    }
    return 0;
}