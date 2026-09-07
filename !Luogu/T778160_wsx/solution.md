显然这个题是可以 $O(n)$ 做的。但是读入卡不掉就没有弄 $O(n)$ 的数据了。不然容易被喷。这里放的 std 是一个 $O(n \log n)$ 的做法。

讲述一个暴力的 $O(n \log n)$ 做法。你考虑一个显而易见的结论，就是对于每个点作为右端点只能有一个合法区间。你考虑先拉出前缀和然后建 ST 表，在 ST 表上二分第一个前缀和小于 $s_i$ 的位置。然后你考虑这样你就求出了第一批符合要求的区间。然后你考虑因为这些区间已经是对于这些右端点的最近区间了，我们只需要对这些区间依次 check 可不可以即可。这样的话是二维数点问题，你用树状数组解决是简单的。时间复杂度 $O(n \log n)$。你用单调栈显然可以做到 $O(n)$ 处理前面后面的部分可以直接线性前缀和优化做到更优。

```cpp
#include <bits/stdc++.h>
using namespace std;

const int N=1e6+5;
long long mn[N],a[N],s[N];
long long nxt[N],lo[N],st[N][21],top;
int q(int x){
    int s=1,ans=0;
    while(s<=x) ans++,s*=2;
    return ans-1;
}
long long query(int l,int r){
    int k=lo[r-l+1];
    return min(st[l][k],st[r-(1<<k)+1][k]);
}
struct BIT{
    int s[N],n;
    int lowbit(int x){return x&-x;}
    void add(int w,int x){for(int i=w;i<=n;i+=lowbit(i)) s[i]+=x;}
    int query(int w){
        int sum=0;
        for(int i=w;i;i-=lowbit(i)) sum+=s[i];
        return sum;
    }
}bit;
int main(){
    ios::sync_with_stdio(false);
    cin.tie(0);cout.tie(0);
    int n,k;
    cin>>n>>k;
    bit.n=n;
    for(int i=1;i<=n;i++) lo[i]=q(i);
    for(int i=1;i<=n;i++) cin>>a[i];
    for(int i=1;i<=n;i++) s[i]=s[i-1]+a[i];
    for(int i=0;i<=n;i++) st[i][0]=s[i];
    for(int j=1;(1<<j)<=n+1;j++){
        for(int i=0;i+(1<<j)-1<=n;i++){
            st[i][j]=min(st[i][j-1],st[i+(1<<(j-1))][j-1]);
        }
    }
    for(int i=1;i<=n;i++){
        // i-k 
        int l=0,r=i-k;
        if (l>r||query(l,r)>s[i]) nxt[i]=-1;
        else{
            while(l<r){
                int mid=(l+r+1)/2;
                if (query(mid,i-k)<=s[i]) l=mid;
                else r=mid-1;
            }
            nxt[i]=l+1;
        } 
    }
    int ans=0;
    for(int i=1;i<=n;i++){
        if (nxt[i]==-1) continue;
        if (bit.query(i)-bit.query(nxt[i]-1)==0) ans++;
        bit.add(nxt[i],1);
    }
    cout<<ans<<endl;
    return 0;
}

```