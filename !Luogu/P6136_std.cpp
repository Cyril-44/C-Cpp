#include<stdio.h>
inline char nc()
{
	static char buf[99999],*l,*r;
	return l==r&&(r=(l=buf)+fread(buf,1,99999,stdin),l==r)?EOF:*l++;
}
inline void read(int&x)
{
	char c=nc();for(;c<'0'||'9'<c;c=nc());
	for(x=0;'0'<=c&&c<='9';x=(x<<3)+(x<<1)+(c^48),c=nc());
}
struct node
{
    node*l,*r;int n,cnt,sz,maxn;
}*rt;
int n,m,ans,lstans;
inline void pu(node*i)
{
    i->sz=i->cnt;
    if(i->l)i->sz+=i->l->sz;
    if(i->r)i->sz+=i->r->sz,i->maxn=i->r->maxn;
    else i->maxn=i->n;
}
inline node*L(node*i){node*j=i->r;i->r=j->l;j->l=i;pu(i);pu(j);return j;}
inline node*R(node*i){node*j=i->l;i->l=j->r;j->r=i;pu(i);pu(j);return j;}
inline char lslay(node*&i,int x)
{
    if(i->l)if(x<=i->l->maxn)
    {
        char o=lslay(i->l,x);
        if(o&1){i=R(i);i=R(i);return 0;}
        if(o&2){i->l=L(i->l);i=R(i);return 0;}
        return 1;
    }
    if(x<=i->n||!i->r)return 0;
    char o=lslay(i->r,x);
    if(o&2){i=L(i);i=L(i);return 0;}
    if(o&1){i->r=R(i->r);i=L(i);return 0;}
    return 2;
}
inline void lsplay(node*&i,int x)
{
    char o=lslay(i,x);
    if(o&1)i=R(i);
    if(o&2)i=L(i);
}
inline void isplay(node*&i,int x)
{
    lsplay(i,x);
    if(i->n==x)++i->cnt,++i->sz;
    else
    {
        lsplay(i->l,1<<30);
        i->l->r=new node{0,0,x,1,1,x};
        pu(i->l);pu(i);
    }
}
inline char slay(node*&i,int x)
{
    if(i->l)if(x<=i->l->sz)
    {
        char o=slay(i->l,x);
        if(o&1){i=R(i);i=R(i);return 0;}
        if(o&2){i->l=L(i->l);i=R(i);return 0;}
        return 1;
    }
    if(i->l)x-=i->l->sz;
    if(x<=i->cnt)return 0;
    x-=i->cnt;
    char o=slay(i->r,x);
    if(o&2){i=L(i);i=L(i);return 0;}
    if(o&1){i->r=R(i->r);i=L(i);return 0;}
    return 2;
}
inline void splay(node*&i,int x)
{
    char o=slay(i,x);
    if(o&1)i=R(i);
    if(o&2)i=L(i);
}
main()
{
    freopen("P6136_11.in", "r", stdin);
    read(n);read(m);
    rt=new node{0,0,-1,1,1,-1};rt=new node{rt,0,1<<30,1,0,0};pu(rt);
    for(int x;n--;read(x),isplay(rt,x));
    for(int o,x;m--;)switch(read(o),read(x),x^=lstans,o)
    {
        case 1:isplay(rt,x);break;
        case 2:lsplay(rt,x);--rt->cnt;
            if(!rt->cnt)splay(rt->r,1),rt->r->l=rt->l,rt=rt->r;break;
        case 3:lsplay(rt,x);ans^=lstans=rt->l->sz;break;
        case 4:splay(rt,x+1);ans^=lstans=rt->n;break;
        case 5:lsplay(rt,x);lsplay(rt->l,1<<30);ans^=lstans=rt->l->n;break;
        case 6:lsplay(rt,x+1);ans^=lstans=rt->n;
    }
    printf("%d",ans);
}