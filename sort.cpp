#include <bits/stdc++.h>
using namespace std;

const int N=100005;
const int oo=1000000000;

struct Node
{int pos;//位置 
 int val;//高度 
}a[N];
int n,root;
int ch[N][2],f[N],sz[N],rev[N];

inline int read()
{int x=0,f=1;char ch=getchar();
 while(ch<'0'||ch>'9'){if(ch=='-')f=-1;ch=getchar();}
 while(ch>='0'&&ch<='9'){x=x*10+ch-'0';ch=getchar();}
 return x*f;
}

bool cmp(Node a,Node b)
{if (a.val==b.val)
   return a.pos<b.pos;
 else
   return a.val<b.val;
}

int get(int x)//判断x是否是父亲结点的右儿子
{
 return ch[f[x]][1]==x;
}

void pushup(int x)//标记上传 
{
 sz[x]=sz[ch[x][1]]+sz[ch[x][0]]+1;
}

void pushdown(int x)//标记下传 
{if (rev[x])
   {if (ch[x][0]) rev[ch[x][0]]^=1;
    if (ch[x][1]) rev[ch[x][1]]^=1;
    swap(ch[x][0],ch[x][1]);
    rev[x]=0;
   }
}

void rotate(int x) //旋转
{int y=f[x],z=f[y]; //y为x的父亲节点,z为x的祖先节点
 int kind=get(x); //求出x是y的儿子类型,左儿子返回0,否则返回1
 pushdown(y); pushdown(x);//标记下传
 ch[y][kind]=ch[x][kind^1];  //选x的不与y->x同侧的儿子给y,保证BST
 ch[x][kind^1]=y; //y为x的不与y->x同侧的儿子
 f[ch[y][kind]]=y; f[y]=x;f[x]=z; //调整各自的父亲节点
 if (z) ch[z][ch[z][1]==y]=x; //调整x作为z->y这一侧的儿子
 pushup(y); pushup(x); //标记上传
}

void splay(int x,int anc) //通过一系列旋转将x调整为结点anc的子结点 
{for (int fa;(fa=f[x])!=anc;rotate(x))
   {pushdown(f[fa]); //祖父结点标记下传
    pushdown(fa); //父亲结点标记下传
    if (f[fa]!=anc) (get(x)==get(fa)) ? rotate(fa) : rotate(x);
    //祖父结点与父亲结点在同一侧,应先调整父亲结点,提高效率
   }
 if (!anc) root=x;
}

void build(int l,int r,int fa)//建splay 
{if (l>r) return;
 int mid=l+r>>1;
 if (mid<fa) ch[fa][0]=mid;
 else ch[fa][1]=mid;
 sz[mid]=1;
 f[mid]=fa;
 if(l==r) return;
 build(l,mid-1,mid);
 build(mid+1,r,mid);
 pushup(mid);
}

int findkth(int k)//查找第k个元素
{int now=root;
 while(1)
   {if (rev[now]) pushdown(now);
    if (k<=sz[ch[now][0]] && ch[now][0])
      now=ch[now][0];
    else
      {k-=sz[ch[now][0]]+1;
       if (k==0) return now;
       else now=ch[now][1];
      }
   }
}

void init()
{n=read();
 a[1].pos=1,a[1].val=-oo;
 a[n+2].pos=n+2,a[n+2].val=oo;//头尾设两个哨兵,防止越界 
 for(int i=2;i<=n+1;i++)
   {a[i].val=read();
    a[i].pos=i;
   }
 sort(a+1,a+n+3,cmp);//按权值排序从小到大,权值相同按位置从小到大排序 
}

void rever(int L,int R)//将区间[L,R]翻转 
{int x=findkth(L-1),y=findkth(R+1);
 splay(x,0);
 splay(y,x);
 rev[ch[y][0]]^=1;
}

void work()
{build(1,n+2,0);
 root=n+3>>1;
 for(int i=2;i<=n+1;i++)
   {splay(a[i].pos,0);
    int ans=sz[ch[root][0]]+1;
    printf("%d ",ans-1);//减1是因为前面有一个哨兵,整体后移了一位 
    rever(i,ans);//区间[i,ans]翻转
   }
}

int main()
{
 init();
 work();
 return 0;  
}
