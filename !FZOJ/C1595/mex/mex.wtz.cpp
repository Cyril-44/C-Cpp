
#include<bits/stdc++.h>
using namespace std;

const int maxn = 1 << 20;
char in[maxn],out[maxn],*p1=in,*p2=in,*p3=out;
#define getchar() (p1==p2&&(p2=(p1=in)+fread(in,1,maxn,stdin),p1==p2)?EOF:*p1++)
#define flush() (fwrite(out,1,p3-out,stdout))
#define putchar(x) (p3==out+maxn&&(flush(),p3=out),*p3++=(x))
template<typename type>
void read(type &x)
{
    x = 0;
    int f = 1;
    char c = getchar();
    while (c < 48 || c > 57) 
    {
        if (c == '-') f = -1;
        c = getchar();
    }
    while (c >= 48 && c <= 57)
        x = x * 10 + c - 48, c = getchar();
    x *= f;
}

template<typename type, typename ...T>
void read(type &x, T &...y)
{
    read(x), read(y...);
}

template<typename type>
inline void write(type x,bool mode=1)//0为空格，1为换行
{
    x < 0 ? x =- x, putchar('-') : 0;
    static short Stack[50], top(0);
    do Stack[++top]=x%10,x/=10; while(x);
    while(top) putchar(Stack[top--]|48);
    mode?putchar('\n'):putchar(' ');
}

const int N=400005,V=400001;
int n,a[N],cnt[N];
struct segtree{
    int a[N<<2],b[N<<2];
    #define ls (u<<1)
    #define rs (u<<1|1)
    void pushup(int u){
        a[u]=a[ls]+a[rs];
        b[u]=min(b[ls],b[rs]);
    }
    void update(int l,int r,int t,int u,int x){
        if(l==r){
            a[u]+=x; b[u]=a[u];
            return;
        }
        int m=l+r>>1;
        if(t<=m) update(l,m,t,ls,x);
        else update(m+1,r,t,rs,x);
        pushup(u);
    }
    int query(int l,int r,int tl,int tr,int u){
        if(tl<=l&&r<=tr) return a[u];
        int m=l+r>>1,ans=0;
        if(tl<=m) ans+=query(l,m,tl,tr,ls);
        if(m+1<=tr) ans+=query(m+1,r,tl,tr,rs);
        return ans;
    }
    int find(int l,int r,int u,int x){
        // printf("find %d %d %d %d\n",l,r,u,x);
        if(l==r) return l;
        int m=l+r>>1;
        if(b[rs]<x) return find(m+1,r,rs,x);
        else return find(l,m,ls,x);
    }
    int find(int l,int r,int tl,int tr,int u,int x){
        if(tl<=l&&r<=tr) return b[u]<x?find(l,r,u,x):-1;
        int m=l+r>>1,res=-1;
        if(m+1<=tr) res=find(m+1,r,tl,tr,rs,x);
        if(tl<=m&&res==-1) res=find(l,m,tl,tr,ls,x);
        return res;
    }
}seg;
bool check(int x){
    int req=1,pos=x-1,add=seg.query(0,V,x,V,1);
    while(pos){
        int tmp=cnt[pos]<req?pos:seg.find(0,V,0,pos,1,req);
        // printf("(%d,%d,%d,%d)",x,pos,req,tmp);
        if(tmp==0) break;
        if(tmp==-1) return 1;
        if(pos!=tmp) add+=seg.query(0,V,tmp+1,pos,1)-req*(pos-tmp);
        req+=req-cnt[tmp]; pos=tmp-1;
        // printf("(%d,%d)",add,req);
        // exit(0);
        if(req>n) return 0;
    }
    return req<=add+cnt[0];
}
int main(){
    read(n);
    int ans=1;
    for(int i=1;i<=n;i++){
        read(a[i]);
        seg.update(0,V,a[i],1,1); cnt[a[i]]++;
        if(i==1){
            write(max(a[i],1),0);
            continue;
        }
        while(check(ans)) ans++;//,printf("\n");
        // printf("\n");
        write(ans-1,0);
    }
    flush();
    return 0;
}